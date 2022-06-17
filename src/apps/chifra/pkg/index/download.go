// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package index

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
	"runtime"
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
	Pin      *manifest.Chunk
}

// fetchResult type make it easier to return both download content and
// download size information (for validation purposes)
type fetchResult struct {
	body      io.ReadCloser
	totalSize int64
}

// WorkerArguments are types meant to hold worker function arguments. We cannot
// pass the arguments directly, because a worker function is expected to take one
// parameter of type interface{}.
type downloadWorkerArguments struct {
	chunkPath       *cache.CachePath
	ctx             context.Context
	downloadWg      *sync.WaitGroup
	gatewayUrl      string
	progressChannel chan<- *progress.Progress
	writeChannel    chan *jobResult
}

type writeWorkerArguments struct {
	cancel          context.CancelFunc
	chunkPath       *cache.CachePath
	ctx             context.Context
	progressChannel chan<- *progress.Progress
	writeWg         *sync.WaitGroup
}

// worker function type as accepted by Ants
type workerFunction func(interface{})

// fetchChunk downloads a chunk using HTTP
func fetchChunk(ctx context.Context, url string) (*fetchResult, error) {
	request, err := http.NewRequestWithContext(ctx, "GET", url, nil)
	if err != nil {
		return nil, err
	}
	response, err := http.DefaultClient.Do(request)
	if err != nil {
		return nil, err
	}
	if response.StatusCode != 200 {
		return nil, fmt.Errorf("wrong status code: %d", response.StatusCode)
	}
	body := response.Body

	fileSize, err := strconv.ParseInt(response.Header.Get("Content-Length"), 10, 64)
	if err != nil {
		return nil, err
	}

	return &fetchResult{
		body:      body,
		totalSize: fileSize,
	}, nil
}

// getDownloadWorker returns a worker function that downloads a chunk
func getDownloadWorker(arguments downloadWorkerArguments) workerFunction {
	progressChannel := arguments.progressChannel
	ctx := arguments.ctx

	return func(param interface{}) {
		url, _ := url.Parse(arguments.gatewayUrl)
		pin := param.(manifest.Chunk)

		defer arguments.downloadWg.Done()

		select {
		case <-ctx.Done():
			// Cancel
			return
		default:
			// Perform download => unzip-and-save
			hash := pin.BloomHash
			if arguments.chunkPath.Type == cache.Index_Final {
				hash = pin.IndexHash
			}

			url.Path = path.Join(url.Path, hash)

			progressChannel <- &progress.Progress{
				Payload: &pin,
				Event:   progress.Start,
				Message: hash,
			}

			download, err := fetchChunk(ctx, url.String())
			if errors.Is(ctx.Err(), context.Canceled) {
				// The request to fetch the chunk was cancelled, because user has
				// pressed Ctrl-C
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
				arguments.writeChannel <- &jobResult{
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
	}
}

// getWriteWorker returns a worker function that writes chunk to disk
func getWriteWorker(arguments writeWorkerArguments) workerFunction {
	progressChannel := arguments.progressChannel
	ctx := arguments.ctx

	return func(resParam interface{}) {
		// Take download data from the channel and save it
		res := resParam.(*jobResult)

		defer arguments.writeWg.Done()

		select {
		case <-ctx.Done():
			return
		default:
			progressChannel <- &progress.Progress{
				Payload: res.Pin,
				Event:   progress.Update,
				Message: "Unzipping",
			}

			trapChannel := sigintTrap.Enable(ctx, arguments.cancel)
			err := saveFileContents(res, arguments.chunkPath)
			sigintTrap.Disable(trapChannel)

			if errors.Is(ctx.Err(), context.Canceled) {
				// Ctrl-C was pressed, cancel
				return
			}

			if err != nil {
				progressChannel <- &progress.Progress{
					Payload: res.Pin,
					Event:   progress.Error,
					Message: err.Error(),
				}
				return
			}

			progressChannel <- &progress.Progress{
				Payload: res.Pin,
				Event:   progress.Done,
			}
		}
	}
}

// GetChunksFromRemote downloads, unzips and saves the chunk of type indicated by chunkType
// for each pin in pins. Progress is reported to progressChannel.
func GetChunksFromRemote(chain string, pins []manifest.Chunk, chunkPath *cache.CachePath, progressChannel chan<- *progress.Progress) {
	poolSize := runtime.NumCPU() * 2
	// Downloaded content will wait for saving in this channel
	writeChannel := make(chan *jobResult, poolSize)
	// Context lets us handle Ctrl-C easily
	ctx, cancel := context.WithCancel(context.Background())
	var downloadWg sync.WaitGroup
	var writeWg sync.WaitGroup

	defer func() {
		cancel()
	}()

	downloadWorkerArgs := downloadWorkerArguments{
		chunkPath:       chunkPath,
		ctx:             ctx,
		downloadWg:      &downloadWg,
		gatewayUrl:      config.GetPinGateway(chain),
		progressChannel: progressChannel,
		writeChannel:    writeChannel,
	}
	downloadPool, err := ants.NewPoolWithFunc(poolSize, getDownloadWorker(downloadWorkerArgs))
	defer downloadPool.Release()
	if err != nil {
		panic(err)
	}

	writeWorkerArgs := writeWorkerArguments{
		cancel:          cancel,
		chunkPath:       chunkPath,
		ctx:             ctx,
		progressChannel: progressChannel,
		writeWg:         &writeWg,
	}
	writePool, err := ants.NewPoolWithFunc(poolSize, getWriteWorker(writeWorkerArgs))
	defer writePool.Release()
	if err != nil {
		panic(err)
	}

	// Increasing wait group counter to make sure we finished saving all downloaded pins
	writeWg.Add(1)
	go func() {
		for result := range writeChannel {
			if ctx.Err() != nil {
				// If the user has pressed Ctrl-C while it was disabled by sigintTrap,
				// we have to drain the channel. Otherwise, we will find ourselves in a deadlock
				continue
			}
			// It would be simpler to call Add where we start downloads, but we have no guarantee that
			// we will be saving the same number of pins (e.g. if download failed)
			writeWg.Add(1)
			writePool.Invoke(result)
		}
		// Range over a channel is blocking, so the following line will be executed only
		// when we finished saving pins
		writeWg.Done()
	}()

	pinsToDownload := filterDownloadedChunks(pins, chunkPath)
	for _, pin := range pinsToDownload {
		downloadWg.Add(1)
		downloadPool.Invoke(pin)
	}

	downloadWg.Wait()
	close(writeChannel)

	writeWg.Wait()

	if errors.Is(ctx.Err(), context.Canceled) {
		progressChannel <- &progress.Progress{
			Event: progress.Cancelled,
		}
		return
	}

	progressChannel <- &progress.Progress{
		Event: progress.AllDone,
	}
}

// saveFileContents decompresses the downloaded data and saves it to files
func saveFileContents(res *jobResult, chunkPath *cache.CachePath) error {
	// We load content to the buffer first to check its size
	buffer := &bytes.Buffer{}
	read, err := buffer.ReadFrom(res.contents)
	if err != nil {
		return err
	}
	if read != res.fileSize {
		return &errSavingCorruptedDownload{res.fileName, res.fileSize, read}
	}

	archive, err := gzip.NewReader(buffer)
	if err != nil {
		return &errSavingCannotDecompress{res.fileName, err}
	}
	defer archive.Close()

	outputFile, err := os.Create(chunkPath.GetFullPath(res.fileName))
	if err != nil {
		return &errSavingCreateFile{res.fileName, err}
	}
	defer outputFile.Close()

	// Unzip and save content to a file
	_, werr := io.Copy(outputFile, archive)
	if werr != nil {
		return &errSavingCopy{res.fileName, werr}
	}

	return nil
}

// filterDownloadedChunks returns new []manifest.Chunk slice with all pins from RootPath removed
func filterDownloadedChunks(pins []manifest.Chunk, chunkPath *cache.CachePath) []manifest.Chunk {
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
func exclude(what map[string]bool, from []manifest.Chunk) []manifest.Chunk {
	result := make([]manifest.Chunk, 0, len(from))

	for _, pin := range from {
		if what[pin.FileName] {
			continue
		}

		result = append(result, pin)
	}

	return result
}
