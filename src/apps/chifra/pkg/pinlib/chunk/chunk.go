// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunk

// Fetching, unzipping, validating and saving both index and bloom chunks

import (
	"bytes"
	"compress/gzip"
	"context"
	"errors"
	"fmt"
	"io"
	"io/ioutil"
	"net/http"
	"net/url"
	"os"
	"path"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinlib/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	ants "github.com/panjf2000/ants/v2"
)

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

// fetchChunk downloads a chunk using HTTP
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

// GetChunksFromRemote downloads, unzips and saves the chunk of type indicated by chunkType
// for each pin in pins. Progress is reported to progressChannel.
func GetChunksFromRemote(pins []manifest.PinDescriptor, chunkPath *cache.Path, progressChannel chan<- *progress.Progress) {
	poolSize := config.ReadBlockScrape().Dev.MaxPoolSize
	// Downloaded content will wait for saving in this channel
	writeChannel := make(chan *jobResult, poolSize)
	// Context lets us handle Ctrl-C easily
	ctx, cancel := context.WithCancel(context.Background())
	var downloadWg sync.WaitGroup
	var writeWg sync.WaitGroup

	defer func() {
		cancel()
	}()

	gatewayUrl := config.ReadBlockScrape().Dev.IpfsGateway

	downloadPool, err := ants.NewPoolWithFunc(poolSize, func(param interface{}) {
		url, _ := url.Parse(gatewayUrl)
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
			if chunkPath.Type == cache.IndexChunk {
				hash = pin.IndexHash
			}

			url.Path = path.Join(url.Path, hash)

			progressChannel <- &progress.Progress{
				Payload: &pin,
				Event:   progress.Start,
				Message: hash,
			}

			download, err := fetchChunk(url.String())
			if errors.Is(ctx.Err(), context.Canceled) {
				return
			}
			if ctx.Err() != nil {
				progressChannel <- &progress.Progress{
					Payload: &pin,
					Event:   progress.Error,
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
				progressChannel <- &progress.Progress{
					Payload: &pin,
					Event:   progress.Error,
					Message: err.Error(),
				}
			}
		}
	})
	defer downloadPool.Release()
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
			progressChannel <- &progress.Progress{
				Payload: res.Pin,
				Event:   progress.Update,
				Message: "Unzipping",
			}

			err := saveFileContents(res, chunkPath)
			if err != nil && err != sigintTrap.ErrInterrupted {
				progressChannel <- &progress.Progress{
					Payload: res.Pin,
					Event:   progress.Error,
					Message: err.Error(),
				}
				return
			}
			if err == sigintTrap.ErrInterrupted {
				// User pressed Ctrl-C
				cancel()
			}

			progressChannel <- &progress.Progress{
				Payload: res.Pin,
				Event:   progress.Done,
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

	pinsToDownload := FilterDownloadedChunks(pins, chunkPath)
	for _, pin := range pinsToDownload {
		downloadWg.Add(1)
		downloadPool.Invoke(pin)
	}

	downloadWg.Wait()
	close(writeChannel)

	writeWg.Wait()
	progressChannel <- &progress.Progress{
		Event: progress.AllDone,
	}
}

// saveFileContents decompresses the downloaded data and saves it to files
func saveFileContents(res *jobResult, chunkPath *cache.Path) error {
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

	outputFile, err := os.Create(chunkPath.GetFullPath(res.fileName))
	if err != nil {
		return &ErrSavingCreateFile{res.fileName, err}
	}
	defer outputFile.Close()

	// Unzip and save content to a file
	_, werr := io.Copy(outputFile, archive)
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

// FilterDownloadedChunks returns new []manifest.PinDescriptor slice with all pins from RootPath removed
func FilterDownloadedChunks(pins []manifest.PinDescriptor, chunkPath *cache.Path) []manifest.PinDescriptor {
	fileMap := make(map[string]bool)

	files, err := ioutil.ReadDir(chunkPath.String())
	if err != nil {
		return pins
	}

	for _, file := range files {
		pinFileName := strings.Replace(file.Name(), chunkPath.Extension, "", -1)
		fileMap[pinFileName] = true
	}

	return exclude(fileMap, pins)
}

// exclude returns a copy of `from` slice with every pin with a file name present
// in `what` map removed
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
