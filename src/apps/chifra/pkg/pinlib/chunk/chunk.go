package chunk

// Fetching, unzipping, validating and saving both index and bloom chunks

import (
	"bytes"
	"compress/gzip"
	"context"
	"fmt"
	"io"
	"io/ioutil"
	"log"
	"net/http"
	"os"
	"path"
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

type jobResult struct {
	fileName string
	fileSize int64
	contents io.Reader
	Pin      *manifest.PinDescriptor
}

type fetchResult struct {
	body      io.ReadCloser
	totalSize int64
}

// TODO: this has to be moved to some function, so that it doesn't get called
// every time chifra is executing
var gatewayUrl = config.ReadBlockScrape().Dev.IpfsGateway
var outputDir = "/Users/dawid/Library/Application Support/TrueBlocks/unchained/pins-new" // config.ReadGlobal().Settings.IndexPath

type outputConfig struct {
	subdir    string
	extension string
}

func (oc *outputConfig) Build(chunkType ChunkType) {
	subdir := "blooms/"
	extension := ".bloom"
	if chunkType == IndexChunk {
		subdir = "finalized/"
		extension = ".bin"
	}

	oc.subdir = subdir
	oc.extension = extension
}

func (oc *outputConfig) ToPath(fileName string) string {
	return path.Join(outputDir, oc.subdir, fileName+oc.extension)
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
	outConfig := &outputConfig{}
	outConfig.Build(chunkType)
	var wg sync.WaitGroup
	var writeWg sync.WaitGroup

	defer func() {
		cancel()
	}()

	// TODO: remove it
	log.Println("Using gateway", gatewayUrl)
	log.Println("Saving chunks to", outConfig.ToPath(""))

	pool, err := ants.NewPoolWithFunc(poolSize, func(param interface{}) {
		url := gatewayUrl
		pin := param.(manifest.PinDescriptor)

		defer wg.Done()

		select {
		case <-ctx.Done():
			// Cancel
			ants.Reboot()
			return
		default:
			// Perform download => unzip-and-save
			if chunkType == IndexChunk {
				url += pin.IndexHash
			} else {
				url += pin.BloomHash
			}

			progressChannel <- &ChunkProgress{
				Pin:   &pin,
				Event: ProgressDownloading,
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
			err := saveFileContents(res, outConfig)

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

	pinsToDownload := FilterDownloadedChunks(pins, outConfig)
	for _, pin := range pinsToDownload {
		wg.Add(1)
		pool.Invoke(pin)
	}

	wg.Wait()
	close(writeChannel)

	writeWg.Wait()
	progressChannel <- &ChunkProgress{
		Event: ProgressAllDone,
	}
	fmt.Printf("running goroutines: %d\n", pool.Running())
	fmt.Println("pins done:", len(pins))
}

func saveFileContents(res *jobResult, outConfig *outputConfig) error {
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
		return fmt.Errorf("file corrupted: %s (expected %d but got %d bytes)", res.fileName, res.fileSize, read)
	}

	archive, err := gzip.NewReader(buffer)

	if err != nil {
		return fmt.Errorf("error unpacking %s: %s", res.fileName, err)
	}

	defer archive.Close()

	// TODO: should be changed to finalized/file-name.bin for index chunks
	output, err := os.Create(outConfig.ToPath(res.fileName))
	if err != nil {
		return fmt.Errorf("error creating file %s: %s", res.fileName, err)
	}
	defer output.Close()

	// Unzip and save content to a file
	_, werr := io.Copy(output, archive)

	if werr != nil {
		return fmt.Errorf("error copying %s: %s", res.fileName, werr)
	}

	select {
	case <-trapChannel:
		return sigintTrap.ErrInterrupted
	default:
		return nil
	}
}

// Returns new []manifest.PinDescriptor slice with all pins from outputDir removed
func FilterDownloadedChunks(pins []manifest.PinDescriptor, outConfig *outputConfig) []manifest.PinDescriptor {
	fileMap := make(map[string]bool)

	files, err := ioutil.ReadDir(path.Join(outputDir, outConfig.subdir))

	if err != nil {
		return pins
	}

	for _, file := range files {
		pinFileName := strings.Replace(file.Name(), outConfig.extension, "", -1)
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
