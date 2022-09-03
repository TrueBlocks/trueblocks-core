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
	"os"
	"path/filepath"
	"runtime"
	"strings"
	"sync"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	ants "github.com/panjf2000/ants/v2"
)

// jobResult type is used to carry both downloaded data and some
// metadata to decompressing/file writing function through a channel
type jobResult struct {
	rng      string
	fileSize int64
	contents io.Reader
	theChunk *manifest.ChunkRecord
}

type progressChan chan<- *progress.Progress

// WorkerArguments are types meant to hold worker function arguments. We cannot
// pass the arguments directly, because a worker function is expected to take one
// parameter of type interface{}.
type downloadWorkerArguments struct {
	chain           string
	chunkType       cache.CacheType
	ctx             context.Context
	progressChannel progressChan
	gatewayUrl      string
	downloadWg      *sync.WaitGroup
	writeChannel    chan *jobResult
	nRetries        int
}

type writeWorkerArguments struct {
	chain           string
	chunkType       cache.CacheType
	ctx             context.Context
	progressChannel progressChan
	cancel          context.CancelFunc
	writeWg         *sync.WaitGroup
	isCompressed    bool
}

// worker function type as accepted by Ants
type workerFunction func(interface{})

// getDownloadWorker returns a worker function that downloads a chunk
func getDownloadWorker(dlwArgs downloadWorkerArguments) workerFunction {
	progressChannel := dlwArgs.progressChannel
	ctx := dlwArgs.ctx

	return func(param interface{}) {
		chunk := param.(manifest.ChunkRecord)

		defer dlwArgs.downloadWg.Done()

		select {
		case <-ctx.Done():
			// Cancel
			return

		default:
			hash := chunk.BloomHash
			if dlwArgs.chunkType == cache.Index_Final {
				hash = chunk.IndexHash
			}

			progressChannel <- &progress.Progress{
				Payload: &chunk,
				Event:   progress.Start,
				Message: hash.String(),
			}

			download, err := pinning.FetchFromGateway(ctx, dlwArgs.gatewayUrl, hash.String())
			time.Sleep(250 * time.Millisecond) // we need to slow things down, otherwise the endpoint rate limits
			if errors.Is(ctx.Err(), context.Canceled) {
				// The request to fetch the chunk was cancelled, because user has
				// pressed Ctrl-C
				return
			}

			if ctx.Err() != nil {
				chunkPath := cache.NewCachePath(dlwArgs.chain, cache.Index_Final)
				removeLocalFile(config.ToIndexPath(chunkPath.GetFullPath(chunk.Range)), "failed download", progressChannel)
				removeLocalFile(config.ToBloomPath(chunkPath.GetFullPath(chunk.Range)), "failed download", progressChannel)
				progressChannel <- &progress.Progress{
					Payload: &chunk,
					Event:   progress.Error,
					Message: ctx.Err().Error(),
				}
				time.Sleep(1 * time.Second)
				return
			}
			if err == nil {
				dlwArgs.writeChannel <- &jobResult{
					rng:      chunk.Range,
					fileSize: download.ContentLen,
					contents: download.Body,
					theChunk: &chunk,
				}
			} else {
				progressChannel <- &progress.Progress{
					Payload: &chunk,
					Event:   progress.Error,
					Message: err.Error(),
				}
			}
		}
	}
}

// getWriteWorker returns a worker function that writes chunk to disk
func getWriteWorker(wwArgs writeWorkerArguments) workerFunction {
	progressChannel := wwArgs.progressChannel
	ctx := wwArgs.ctx

	return func(resParam interface{}) {
		// Take download data from the channel and save it
		res := resParam.(*jobResult)

		defer wwArgs.writeWg.Done()

		select {
		case <-ctx.Done():
			return
		default:
			trapChannel := sigintTrap.Enable(ctx, wwArgs.cancel)
			err := saveFileContents(wwArgs, res)
			sigintTrap.Disable(trapChannel)

			if errors.Is(ctx.Err(), context.Canceled) {
				// Ctrl-C was pressed, cancel
				return
			}

			if err != nil {
				progressChannel <- &progress.Progress{
					Payload: res.theChunk,
					Event:   progress.Error,
					Message: err.Error(),
				}
				return
			}

			progressChannel <- &progress.Progress{
				Payload: res.theChunk,
				Event:   progress.Finished,
				Message: wwArgs.chunkType.String(),
			}
		}
	}
}

// DownloadChunks downloads, unzips and saves the chunk of type indicated by chunkType
// for each pin in pins. Progress is reported to progressChannel.
func DownloadChunks(chain string, pins []manifest.ChunkRecord, chunkType cache.CacheType, progressChannel progressChan) {
	// If we make this too big, the pinning service chokes
	poolSize := utils.Min(10, (runtime.NumCPU()*3)/2)
	// Downloaded content will wait for saving in this channel
	writeChannel := make(chan *jobResult, poolSize)
	// Context lets us handle Ctrl-C easily
	ctx, cancel := context.WithCancel(context.Background())
	var downloadWg sync.WaitGroup
	var writeWg sync.WaitGroup

	defer func() {
		cancel()
	}()

	dlwArgs := downloadWorkerArguments{
		chain:           chain,
		chunkType:       chunkType,
		ctx:             ctx,
		progressChannel: progressChannel,
		downloadWg:      &downloadWg,
		gatewayUrl:      config.GetIpfsGateway(chain),
		writeChannel:    writeChannel,
		nRetries:        8,
	}
	downloadPool, err := ants.NewPoolWithFunc(poolSize, getDownloadWorker(dlwArgs))
	defer downloadPool.Release()
	if err != nil {
		panic(err)
	}

	wwArgs := writeWorkerArguments{
		chain:           chain,
		chunkType:       chunkType,
		ctx:             ctx,
		progressChannel: progressChannel,
		cancel:          cancel,
		writeWg:         &writeWg,
		isCompressed:    false,
	}
	writePool, err := ants.NewPoolWithFunc(poolSize, getWriteWorker(wwArgs))
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

	itemsToDownload := filterDownloadedChunks(chain, pins, chunkType, progressChannel)
	progressChannel <- &progress.Progress{
		Event:   progress.Statistics,
		Payload: len(itemsToDownload),
	}
	for _, item := range itemsToDownload {
		downloadWg.Add(1)
		downloadPool.Invoke(item)
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

// saveFileContents save the downloaded file and possibly decompress it on the way out
// TODO: The IPFS files are no longer compressed, so the isCompressed code could be deleted.
func saveFileContents(wwArgs writeWorkerArguments, res *jobResult) error {
	var in io.Reader

	if wwArgs.isCompressed {
		// We load content to the buffer first to check its size
		buffer := &bytes.Buffer{}
		read, err := buffer.ReadFrom(res.contents)
		if err != nil {
			return err
		}
		if read != res.fileSize {
			return fmt.Errorf("file corrupted: %s (expected %d but got %d bytes)", res.rng, res.fileSize, read)
		}

		archive, err := gzip.NewReader(buffer)
		if err != nil {
			return fmt.Errorf("error unpacking %s: %s", res.rng, err)
		}
		defer archive.Close()
		in = archive

	} else {
		in = res.contents

	}

	chunkPath := cache.NewCachePath(wwArgs.chain, wwArgs.chunkType)
	outputFile, err := os.OpenFile(chunkPath.GetFullPath(res.rng), os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return fmt.Errorf("error creating file %s: %s", res.rng, err)
	}

	// Unzip and save content to a file
	_, werr := io.Copy(outputFile, in)
	if werr != nil {
		if file.FileExists(outputFile.Name()) {
			// TODO: BOGUS - CHIFRA INIT - PARTIAL FILES
			outputFile.Close()
			os.Remove(outputFile.Name())
			logger.Log(logger.Warning, "Failed download", colors.Magenta, res.rng, colors.Off, "(will retry)", strings.Repeat(" ", 30))
			time.Sleep(1 * time.Second)
		}
		// Information about this error
		// https://community.k6.io/t/warn-0040-request-failed-error-stream-error-stream-id-3-internal-error/777/2
		logger.Log(logger.Info, "Error copying:[", werr, "]")
		return fmt.Errorf("error copying %s: %s", res.rng, werr)
	}

	outputFile.Close()
	return nil
}

// filterDownloadedChunks returns new []manifest.ChunkRecord slice with all chunks from RootPath removed
func filterDownloadedChunks(chain string, chunks []manifest.ChunkRecord, chunkType cache.CacheType, progressChannel progressChan) []manifest.ChunkRecord {
	chunkPath := cache.NewCachePath(chain, chunkType)
	onDiscMap := make(map[string]bool)

	files, err := os.ReadDir(chunkPath.String())
	if err != nil {
		return chunks
	}

	for _, ff := range files {
		name := ff.Name()
		fullPath := chunkPath.GetFullPath(name)
		if !strings.HasSuffix(name, ".bin") && !strings.HasSuffix(name, ".bloom") {
			fullPath = filepath.Join(chunkPath.GetFullPath(""), name)
		}
		onDiscMap[fullPath] = true
	}

	return exclude(chain, chunkType, onDiscMap, chunks, progressChannel)
}

// exclude returns a copy of `from` slice with every item with a file name present in `what` map removed
func exclude(chain string, chunkType cache.CacheType, onDiscMap map[string]bool, chunks []manifest.ChunkRecord, progressChannel progressChan) []manifest.ChunkRecord {
	chunksNeeded := make([]manifest.ChunkRecord, 0, len(chunks))
	for _, item := range chunks {
		fullPath := item.GetFullPath(chain, chunkType)
		if onDiscMap[fullPath] {
			var hashOk bool = true
			if chunkType == cache.Index_Final {
				hashOk, _ = HasValidHeader(chain, fullPath)
			}
			sizeOk := expectedSize(chunkType, item, fullPath)

			if !hashOk {
				if removeLocalFile(fullPath, "invalid header", progressChannel) {
					onDiscMap[fullPath] = false
				}
			} else if !sizeOk {
				if removeLocalFile(fullPath, "unexpected size", progressChannel) {
					onDiscMap[fullPath] = false
				}
			} else {
				onDiscMap[fullPath] = false
				continue
			}
		}
		chunksNeeded = append(chunksNeeded, item)
	}

	// Any file still in this list is not in the manifest and should be removed
	for fullPath, b := range onDiscMap {
		if b {
			removeLocalFile(fullPath, "unknown file", progressChannel)
		}
	}

	progressChannel <- &progress.Progress{
		Event:   progress.Update,
		Message: fmt.Sprintf("Number of %s files to download %d", chunkType, len(chunksNeeded)),
	}
	return chunksNeeded
}

func expectedSize(chunkType cache.CacheType, chunk manifest.ChunkRecord, path string) bool {
	if !file.FileExists(path) {
		return false
	}
	expectedSize := chunk.BloomSize
	if chunkType == cache.Index_Final {
		expectedSize = chunk.IndexSize
	}
	return file.FileSize(path) == expectedSize
}

func removeLocalFile(fullPath, reason string, progressChannel progressChan) bool {
	if file.FileExists(fullPath) {
		err := os.Remove(fullPath)
		if err != nil {
			progressChannel <- &progress.Progress{
				Event:   progress.Error,
				Message: fmt.Sprintf("Failed to remove file %s [%s]", fullPath, err.Error()),
			}
		} else {
			progressChannel <- &progress.Progress{
				Event:   progress.Update,
				Message: fmt.Sprintf("File %s removed [%s]", fullPath, reason),
			}
		}
	}
	return file.FileExists(fullPath)
}
