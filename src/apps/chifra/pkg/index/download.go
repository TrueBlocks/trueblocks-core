// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package index

// Fetching, unzipping, validating and saving both index and bloom chunks

import (
	"context"
	"errors"
	"fmt"
	"io"
	"os"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
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

type ProgressChan chan<- *progress.ProgressMsg

// WorkerArguments are types meant to hold worker function arguments. We cannot
// pass the arguments directly, because a worker function is expected to take one
// parameter of type interface{}.
type downloadWorkerArguments struct {
	ctx             context.Context
	progressChannel ProgressChan
	gatewayUrl      string
	downloadWg      *sync.WaitGroup
	writeChannel    chan *jobResult
	nRetries        int
}

type writeWorkerArguments struct {
	ctx             context.Context
	progressChannel ProgressChan
	cancel          context.CancelFunc
	writeWg         *sync.WaitGroup
}

// worker function type as accepted by Ants
type workerFunction func(interface{})

// getDownloadWorker returns a worker function that downloads a chunk
func getDownloadWorker(chain string, workerArgs downloadWorkerArguments, chunkType walk.CacheType) workerFunction {
	progressChannel := workerArgs.progressChannel

	return func(param interface{}) {
		chunk := param.(manifest.ChunkRecord)

		defer workerArgs.downloadWg.Done()

		select {
		case <-workerArgs.ctx.Done():
			// Cancel
			return

		default:
			hash := chunk.BloomHash
			if chunkType == walk.Index_Final {
				hash = chunk.IndexHash
			}
			if hash != "" {

				// TODO: Do we really need the colored display?
				msg := fmt.Sprintf("%v", chunk)
				msg = strings.Replace(msg, hash.String(), colors.BrightCyan+hash.String()+colors.Off, -1)
				progressChannel <- &progress.ProgressMsg{
					Payload: &chunk,
					Event:   progress.Start,
					Message: msg,
				}

				download, err := pinning.FetchFromGateway(workerArgs.ctx, workerArgs.gatewayUrl, hash.String())
				if errors.Is(workerArgs.ctx.Err(), context.Canceled) {
					// The request to fetch the chunk was cancelled, because user has
					// pressed Ctrl-C
					return
				}

				if workerArgs.ctx.Err() != nil {
					// User hit control + c - clean up both peices for the current chunk
					chunkPath := config.GetPathToIndex(chain) + "finalized/" + chunk.Range + ".bin"
					RemoveLocalFile(ToIndexPath(chunkPath), "user canceled", progressChannel)
					RemoveLocalFile(ToBloomPath(chunkPath), "user canceled", progressChannel)
					progressChannel <- &progress.ProgressMsg{
						Payload: &chunk,
						Event:   progress.Error,
						Message: workerArgs.ctx.Err().Error(),
					}
					return
				}
				if err == nil {
					workerArgs.writeChannel <- &jobResult{
						rng:      chunk.Range,
						fileSize: download.ContentLen,
						contents: download.Body,
						theChunk: &chunk,
					}
				} else {
					progressChannel <- &progress.ProgressMsg{
						Payload: &chunk,
						Event:   progress.Error,
						Message: err.Error(),
					}
				}
			}
		}
	}
}

// getWriteWorker returns a worker function that writes chunk to disk
func getWriteWorker(chain string, workerArgs writeWorkerArguments, chunkType walk.CacheType) workerFunction {
	progressChannel := workerArgs.progressChannel

	return func(resParam interface{}) {
		// Take download data from the channel and save it
		res := resParam.(*jobResult)

		defer workerArgs.writeWg.Done()

		select {
		case <-workerArgs.ctx.Done():
			return
		default:
			cleanOnQuit := func() {
				logger.Warn(sigintTrap.TrapMessage)
			}
			trapChannel := sigintTrap.Enable(workerArgs.ctx, workerArgs.cancel, cleanOnQuit)
			err := writeBytesToDisc(chain, chunkType, res)
			sigintTrap.Disable(trapChannel)
			if errors.Is(workerArgs.ctx.Err(), context.Canceled) {
				// Ctrl-C was pressed, cancel
				return
			}

			if err != nil {
				progressChannel <- &progress.ProgressMsg{
					Payload: res.theChunk,
					Event:   progress.Error,
					Message: err.Error(),
				}
				return
			}

			progressChannel <- &progress.ProgressMsg{
				Payload: res.theChunk,
				Event:   progress.Finished,
				Message: chunkType.String(),
			}
		}
	}
}

// DownloadChunks downloads, unzips and saves the chunk of type indicated by chunkType
// for each chunk in chunks. ProgressMsg is reported to progressChannel.
func DownloadChunks(chain string, chunksToDownload []manifest.ChunkRecord, chunkType walk.CacheType, poolSize int, progressChannel ProgressChan) {
	// Context lets us handle Ctrl-C easily
	ctx, cancel := context.WithCancel(context.Background())
	defer func() {
		cancel()
	}()

	var downloadWg sync.WaitGroup
	writeChannel := make(chan *jobResult, poolSize)
	downloadWorkerArgs := downloadWorkerArguments{
		ctx:             ctx,
		progressChannel: progressChannel,
		downloadWg:      &downloadWg,
		gatewayUrl:      config.GetIpfsGateway(chain),
		writeChannel:    writeChannel,
		nRetries:        8,
	}
	downloadPool, err := ants.NewPoolWithFunc(poolSize, getDownloadWorker(chain, downloadWorkerArgs, chunkType))
	defer downloadPool.Release()
	if err != nil {
		panic(err)
	}

	var writeWg sync.WaitGroup
	writeWorkerArgs := writeWorkerArguments{
		ctx:             ctx,
		progressChannel: progressChannel,
		cancel:          cancel,
		writeWg:         &writeWg,
	}
	writePool, err := ants.NewPoolWithFunc(poolSize, getWriteWorker(chain, writeWorkerArgs, chunkType))
	defer writePool.Release()
	if err != nil {
		panic(err)
	}

	// Closed in the go routine after we're finished writing or the user cancels
	writeWg.Add(1)
	go func() {
		for result := range writeChannel {
			if ctx.Err() != nil {
				// The user hit Ctrl-C. It may have been disabled by sigintTrap, so we
				// must drain the channel. Otherwise, it will deadlock
				continue
			}

			// Closed inside the invocation
			writeWg.Add(1)
			_ = writePool.Invoke(result)
		}

		// Close the opening wg when all writes are finished or the user canceled
		writeWg.Done()
	}()

	for _, chunk := range chunksToDownload {
		downloadWg.Add(1)
		_ = downloadPool.Invoke(chunk)
	}
	downloadWg.Wait()

	close(writeChannel)
	writeWg.Wait()

	if errors.Is(ctx.Err(), context.Canceled) {
		progressChannel <- &progress.ProgressMsg{
			Event: progress.Cancelled,
		}
		return
	}

	progressChannel <- &progress.ProgressMsg{
		Event: progress.AllDone,
	}
}

// writeBytesToDisc save the downloaded bytes to disc
func writeBytesToDisc(chain string, chunkType walk.CacheType, res *jobResult) error {
	fullPath := config.GetPathToIndex(chain) + "finalized/" + res.rng + ".bin"
	if chunkType == walk.Index_Bloom {
		fullPath = ToBloomPath(fullPath)
	}
	outputFile, err := os.OpenFile(fullPath, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0666)
	if err != nil {
		return fmt.Errorf("error creating output file file %s in writeBytesToDisc: [%s]", res.rng, err)
	}

	// Unzip and save content to a file
	_, err = io.Copy(outputFile, res.contents)
	if err != nil {
		if file.FileExists(outputFile.Name()) {
			outputFile.Close()
			os.Remove(outputFile.Name())
			col := colors.Magenta
			if fullPath == ToIndexPath(fullPath) {
				col = colors.Yellow
			}
			logger.Warn("Failed download", col, res.rng, colors.Off, "(will retry)", strings.Repeat(" ", 30))
		}
		// Information about this error
		// https://community.k6.io/t/warn-0040-request-failed-error-stream-error-stream-id-3-internal-error/777/2
		return fmt.Errorf("error copying %s file in writeBytesToDisc: [%s]", res.rng, err)
	}

	outputFile.Close()
	return nil
}

func RemoveLocalFile(fullPath, reason string, progressChannel ProgressChan) bool {
	if file.FileExists(fullPath) {
		err := os.Remove(fullPath)
		if err != nil {
			progressChannel <- &progress.ProgressMsg{
				Event:   progress.Error,
				Message: fmt.Sprintf("Failed to remove file %s [%s]", fullPath, err.Error()),
			}
		} else {
			progressChannel <- &progress.ProgressMsg{
				Event:   progress.Update,
				Message: fmt.Sprintf("File %s removed [%s]", fullPath, reason),
			}
		}
	}
	return file.FileExists(fullPath)
}
