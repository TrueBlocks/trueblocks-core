package chunk

// Fetching, unzipping, validating and saving both index and bloom chunks

import (
	"bytes"
	"compress/gzip"
	"context"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"os"
	"runtime"
	"strconv"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/panjf2000/ants/v2"
)

// The size of worker pools for downloading and saving data
var poolSize = runtime.NumCPU() * 2

type ChunkType uint

const (
	BloomChunk ChunkType = iota
	IndexChunk
)

type ProgressEvent uint

const (
	ProgressDownloading ProgressEvent = iota
	ProgressUnzipping
	ProgressValidating
	ProgressDone
	ProgressError
	ProgressAllDone
)

type ChunkProgress struct {
	Event   ProgressEvent
	Message string
	Pin     *manifest.PinDescriptor
}

// jobResult type is used to carry both downloaded data and some
// metadata to decompressing/file writing function through a channel
type jobResult struct {
	fileName string
	fileSize int64
	contents io.Reader
	Pin      *manifest.PinDescriptor
}

// fetchResult type make it easier to return both download content and
// download size information (for validation purposes)
type fetchResult struct {
	body      io.ReadCloser
	totalSize int64
}

// Downloads a chunk using HTTP
func fetchChunk(url string) (*fetchResult, error) {
	response, err := http.Get(url)
	body := response.Body
	if response.StatusCode != 200 {
		return nil, fmt.Errorf("wrong status code: %d", response.StatusCode)
	}
	if err != nil {
		return nil, err
	}

	fileSize, err := strconv.ParseInt(response.Header.Get("Content-Length"), 10, 64)
	if err != nil {
		return nil, err
	}

	return &fetchResult{
		body:      body,
		totalSize: fileSize,
	}, nil
}

// For each pin in pins, downloads, unzips and saves the chunk of type indicated by chunkType.
// Progress is reported to progressChannel.
func GetChunksFromRemote(pins []manifest.PinDescriptor, chunkType ChunkType, progressChannel chan<- *ChunkProgress) {
	// Downloaded content will wait for saving in this channel
	writeChannel := make(chan *jobResult, poolSize)
	// Context lets us handle Ctrl-C easily
	ctx, cancel := context.WithCancel(context.Background())
	cacheLayout := &CacheLayout{}
	cacheLayout.New(chunkType)
	var downloadWg sync.WaitGroup
	var writeWg sync.WaitGroup

	defer func() {
		cancel()
	}()

	gatewayUrl := config.ReadBlockScrape().Dev.IpfsGateway

	pool, err := ants.NewPoolWithFunc(poolSize, func(param interface{}) {
		url := gatewayUrl
		pin := param.(manifest.PinDescriptor)

		defer downloadWg.Done()

		select {
		case <-ctx.Done():
			// Cancel
			ants.Reboot()
			return
		default:
			// Perform download => unzip-and-save
			hash := pin.BloomHash

			if chunkType == IndexChunk {
				hash = pin.IndexHash
			}

			url += hash

			progressChannel <- &ChunkProgress{
				Pin:     &pin,
				Event:   ProgressDownloading,
				Message: hash,
			}

			download, err := fetchChunk(url)
			if ctx.Err() != nil {
				progressChannel <- &ChunkProgress{
					Pin:     &pin,
					Event:   ProgressError,
					Message: ctx.Err().Error(),
				}
				return
			}
			if err == nil {
				writeChannel <- &jobResult{
					fileName: pin.FileName,
					fileSize: download.totalSize,
					contents: download.body,
					Pin:      &pin,
				}
			} else {
				progressChannel <- &ChunkProgress{
					Pin:     &pin,
					Event:   ProgressError,
					Message: err.Error(),
				}
			}

			if gatewayUrl == config.DefaultIpfsGateway {
				time.Sleep(time.Second * 5) // Align with Pinata rate limit
			}
		}
	})
	defer pool.Release()
	if err != nil {
		panic(err)
	}

	writePool, err := ants.NewPoolWithFunc(poolSize, func(resParam interface{}) {
		// Take download data from the channel and save it
		res := resParam.(*jobResult)

		defer writeWg.Done()

		select {
		case <-ctx.Done():
			return
		default:
			progressChannel <- &ChunkProgress{
				Pin:   res.Pin,
				Event: ProgressUnzipping,
			}
			err := saveFileContents(res, cacheLayout)

			if err == sigintTrap.ErrInterrupted {
				// User pressed Ctrl-C
				cancel()
				return
			}

			if err != nil && err != sigintTrap.ErrInterrupted {
				progressChannel <- &ChunkProgress{
					Pin:     res.Pin,
					Event:   ProgressError,
					Message: err.Error(),
				}
			}
		}
	})
	defer writePool.Release()
	if err != nil {
		panic(err)
	}

	// Increasing wait group counter to make sure we finished saving all downloaded pins
	writeWg.Add(1)
	go func() {
		for result := range writeChannel {
			// It would be simpler to call Add where we start downloads, but we have no guarantee that
			// we will be saving the same number of pins (e.g. if download failed)
			writeWg.Add(1)
			writePool.Invoke(result)
		}
		// Range over a channel is blocking, so the following line will be executed only
		// when we finished saving pins
		writeWg.Done()
	}()

	pinsToDownload := FilterDownloadedChunks(pins, cacheLayout)
	for _, pin := range pinsToDownload {
		downloadWg.Add(1)
		pool.Invoke(pin)
	}

	downloadWg.Wait()
	close(writeChannel)

	writeWg.Wait()
	progressChannel <- &ChunkProgress{
		Event:   ProgressAllDone,
		Message: fmt.Sprint(len(pins)),
	}
}

// Decompresses the downloaded data and saves it to files
func saveFileContents(res *jobResult, cacheLayout *CacheLayout) error {
	// Postpone Ctrl-C
	trapChannel := sigintTrap.Enable()
	defer sigintTrap.Disable(trapChannel)

	// We load content to the buffer first to check its size
	buffer := &bytes.Buffer{}
	read, err := buffer.ReadFrom(res.contents)
	if err != nil {
		return err
	}
	if read != res.fileSize {
		return &ErrSavingCorruptedDownload{res.fileName, res.fileSize, read}
	}

	archive, err := gzip.NewReader(buffer)
	if err != nil {
		return &ErrSavingCannotDecompress{res.fileName, err}
	}
	defer archive.Close()

	output, err := os.Create(cacheLayout.GetPathTo(res.fileName))
	if err != nil {
		return &ErrSavingCreateFile{res.fileName, err}
	}
	defer output.Close()

	// Unzip and save content to a file
	_, werr := io.Copy(output, archive)
	if werr != nil {
		return &ErrSavingCopy{res.fileName, werr}
	}

	select {
	case <-trapChannel:
		return sigintTrap.ErrInterrupted
	default:
		return nil
	}
}

// Returns new []manifest.PinDescriptor slice with all pins from outputDir removed
func FilterDownloadedChunks(pins []manifest.PinDescriptor, cacheLayout *CacheLayout) []manifest.PinDescriptor {
	fileMap := make(map[string]bool)

	files, err := ioutil.ReadDir(cacheLayout.String())
	if err != nil {
		return pins
	}

	for _, file := range files {
		pinFileName := strings.Replace(file.Name(), cacheLayout.extension, "", -1)
		fileMap[pinFileName] = true
	}

	return exclude(fileMap, pins)
}

func exclude(what map[string]bool, from []manifest.PinDescriptor) []manifest.PinDescriptor {
	result := make([]manifest.PinDescriptor, 0, len(from))

	for _, pin := range from {
		if what[pin.FileName] {
			continue
		}

		result = append(result, pin)
	}

	return result
}
