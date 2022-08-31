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
	"log"
	"net/url"
	"os"
	"path"
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

// WorkerArguments are types meant to hold worker function arguments. We cannot
// pass the arguments directly, because a worker function is expected to take one
// parameter of type interface{}.
type downloadWorkerArguments struct {
	chain           string
	chunkType       cache.CacheType
	ctx             context.Context
	progressChannel chan<- *progress.Progress
	gatewayUrl      string
	downloadWg      *sync.WaitGroup
	writeChannel    chan *jobResult
	nRetries        int
}

type writeWorkerArguments struct {
	chain           string
	chunkType       cache.CacheType
	ctx             context.Context
	progressChannel chan<- *progress.Progress
	cancel          context.CancelFunc
	chunkPath2      *cache.CachePath
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
		url, _ := url.Parse(dlwArgs.gatewayUrl)
		chunk := param.(manifest.ChunkRecord)

		defer dlwArgs.downloadWg.Done()

		select {
		case <-ctx.Done():
			// Cancel
			return
		default:
			// Perform download => unzip-and-save
			hash := chunk.BloomHash
			if dlwArgs.chunkType == cache.Index_Final {
				hash = chunk.IndexHash
			}

			url.Path = path.Join(url.Path, hash.String())

			progressChannel <- &progress.Progress{
				Payload: &chunk,
				Event:   progress.Start,
				Message: hash.String(),
			}

			download, err := pinning.FetchFromGateway(ctx, url.String())
			time.Sleep(250 * time.Millisecond) // we need to slow things down, otherwise the endpoint rate limits
			if errors.Is(ctx.Err(), context.Canceled) {
				log.Fatalln("User hit Control+C in downloadWorker", url)
				// The request to fetch the chunk was cancelled, because user has
				// pressed Ctrl-C
				return
			}

			if ctx.Err() != nil {
				// TODO: BOGUS - Should remove any partially downloaded file
				indexChunkPath := cache.NewCachePath(dlwArgs.chain, cache.Index_Final)
				indexPath := config.ToIndexPath(indexChunkPath.GetFullPath(chunk.Range))
				bloomChunkPath := cache.NewCachePath(dlwArgs.chain, cache.Index_Bloom)
				bloomPath := config.ToIndexPath(bloomChunkPath.GetFullPath(chunk.Range))
				if file.FileExists(indexPath) || file.FileExists(bloomPath) {
					logger.Log(logger.Warning, colors.Yellow, "Removing failed download", indexPath, colors.Off)
					time.Sleep(1 * time.Second)
					if file.FileExists(indexPath) {
						os.Remove(indexPath)
					}
					if file.FileExists(bloomPath) {
						os.Remove(bloomPath)
					}
				}
				log.Fatalln("Error returned by FetchFromGateway in downloadWorker", url, err)
				progressChannel <- &progress.Progress{
					Payload: &chunk,
					Event:   progress.Error,
					Message: ctx.Err().Error(),
				}
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
				log.Fatalln("Error returned by FetchFromGateway in downloadWorker", url, err)
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

			msg := fmt.Sprintf("Downloaded %s%s%s file", colors.Yellow, wwArgs.chunkType, colors.Off)
			if wwArgs.isCompressed {
				msg = fmt.Sprintf("Downloaded and unzipped %s%s%s file", colors.Yellow, wwArgs.chunkType, colors.Off)
			}

			progressChannel <- &progress.Progress{
				Payload: res.theChunk,
				Event:   progress.Update,
				Message: msg,
			}

			progressChannel <- &progress.Progress{
				Payload: res.theChunk,
				Event:   progress.Done,
			}
		}
	}
}

// DownloadChunks downloads, unzips and saves the chunk of type indicated by chunkType
// for each pin in pins. Progress is reported to progressChannel.
func DownloadChunks(chain string, pins []manifest.ChunkRecord, chunkType cache.CacheType, progressChannel chan<- *progress.Progress) {
	chunkPath := cache.NewCachePath(chain, chunkType)

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
		chunkPath2:      &chunkPath,
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

	itemsToDownload := filterDownloadedChunks(chain, pins, chunkType)
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

	outputFile, err := os.OpenFile(wwArgs.chunkPath2.GetFullPath(res.rng), os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return fmt.Errorf("error creating file %s: %s", res.rng, err)
	}

	// Unzip and save content to a file
	_, werr := io.Copy(outputFile, in)
	if werr != nil {
		if file.FileExists(outputFile.Name()) {
			// TODO: BOGUS - Should remove any partially downloaded file
			outputFile.Close()
			os.Remove(outputFile.Name())
			log.Fatal("Failed download", colors.Magenta, res.rng, colors.Off, "(will retry)", strings.Repeat(" ", 30))
			time.Sleep(1 * time.Second)
		}
		return fmt.Errorf("error copying %s: %s", res.rng, werr)
	}

	outputFile.Close()
	return nil
}

// filterDownloadedChunks returns new []manifest.ChunkRecord slice with all chunks from RootPath removed
func filterDownloadedChunks(chain string, chunks []manifest.ChunkRecord, chunkType cache.CacheType) []manifest.ChunkRecord {
	chunkPath := cache.NewCachePath(chain, chunkType)
	fileMap := make(map[string]bool)

	files, err := os.ReadDir(chunkPath.String())
	if err != nil {
		return chunks
	}

	for _, file := range files {
		itemFn := strings.Replace(file.Name(), chunkPath.Extension, "", -1)
		fileMap[itemFn] = true
	}

	return exclude(chain, chunkType, fileMap, chunks)
}

var mm sync.Mutex

// exclude returns a copy of `from` slice with every item with a file name present in `what` map removed
func exclude(chain string, chunkType cache.CacheType, exclusions map[string]bool, chunks []manifest.ChunkRecord) []manifest.ChunkRecord {
	mm.Lock()
	result := make([]manifest.ChunkRecord, 0, len(chunks))
	for _, item := range chunks {
		if exclusions[item.Range] {
			chunkPath := cache.NewCachePath(chain, chunkType)
			path := chunkPath.GetFullPath(item.Range)
			expectedSize := item.BloomSize
			if chunkType == cache.Index_Final {
				expectedSize = item.IndexSize
			}
			if file.FileExists(path) {
				if file.FileSize(path) == expectedSize {
					// logger.Log(logger.Info, "Range", item.Range, "exists and is okay. Skipping.")
					continue
				} else {
					logger.Log(logger.Info, chunkType, "file for range", item.Range, "exists but is the wrong size", file.FileSize(path), "-", expectedSize, ". Re-downloading...")
					os.Remove(path)
				}
			}
			// } else {
			// 	logger.Log(logger.Info, chunkType, "file for range", item.Range, "does not exist. Downloading...")
		}
		result = append(result, item)
	}
	mm.Unlock()
	return result
}
