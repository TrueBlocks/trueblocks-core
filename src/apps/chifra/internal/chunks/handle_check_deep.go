// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"io"
	"sync"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	shell "github.com/ipfs/go-ipfs-api"
)

type reporter struct {
	chunk  *manifest.ChunkRecord
	report *simpleReportCheck
	mutex  *sync.Mutex
}

// CheckDeep digs deep into the data. In `index` mode, it opens each index and checks
// that all addresses in the index return true when checked against its corresponding
// Bloom filter. In `manifest` mode, it checks that each IPFS hash in the manifest is
// actually pinned. The later requires a locally running IPFS node.
func (opts *ChunksOptions) CheckDeep(cacheMan *manifest.Manifest, report *simpleReportCheck) error {
	chain := opts.Globals.Chain

	mutex := sync.Mutex{}
	theMap := make(map[string]*reporter)
	for _, chunk := range cacheMan.ChunkMap {
		theMap[chunk.Range] = &reporter{chunk, report, &mutex}
	}

	total := len(theMap)
	var done atomic.Int32
	progressChan := make(chan int)
	defer close(progressChan)
	var tick int32 = 10

	var sh *shell.Shell
	var procFunc func(rangeStr string, item *reporter) (err error)
	if opts.Mode == "index" {
		total = 0
		tick = 1000
		procFunc = func(rangeStr string, item *reporter) (err error) {
			rng := base.RangeFromRangeString(item.chunk.Range)
			_, path := rng.RangeToFilename(chain)
			bloomFilename := index.ToBloomPath(path)
			bl, err := bloom.NewChunkBloom(bloomFilename)
			if err != nil {
				return
			}
			defer bl.Close()

			misses := 0
			path = index.ToIndexPath(path) // it may not exist if user did not do chifra init --all for example
			if file.FileExists(path) {
				indexChunk, err := index.NewChunkData(path)
				if err != nil {
					return err
				}
				defer indexChunk.Close()

				_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
				if err != nil {
					return err
				}

				total += int(indexChunk.Header.AddressCount)
				for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
					obj := index.AddressRecord{}
					err := obj.ReadAddress(indexChunk.File)
					if err != nil {
						return err
					}
					if !bl.IsMember(obj.Address) {
						fmt.Println("X", colors.Yellow, "bloom miss", obj.Address, "in", item.chunk.Range, colors.Off)
						misses++
					}
					progressChan <- 1
				}

				item.mutex.Lock()
				defer item.mutex.Unlock()
				report.VisitedCnt++
				report.CheckedCnt++
				if misses == 0 {
					report.PassedCnt++
				}

				return nil
			}

			progressChan <- 1

			return nil
		}
	} else if opts.Mode == "manifest" {
		total = len(theMap) * 2
		sh = shell.NewShell("localhost:5001")
		procFunc = func(rangeStr string, item *reporter) (err error) {
			progressChan <- 1
			err = checkHashes(item.chunk, "blooom", sh, item)
			if err != nil {
				return err
			}
			progressChan <- 1
			return checkHashes(item.chunk, "index", sh, item)
		}
	} else {
		return fmt.Errorf("unknown mode: %s", opts.Mode)
	}

	// Listen on a channel and whenever it updates, call `reportProgress`
	go func() {
		for progress := range progressChan {
			doneNow := done.Add(int32(progress))
			logger.PctProgress(doneNow, total, tick)
		}
	}()

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errorChan := make(chan error)
	go utils.IterateOverMap(ctx, errorChan, theMap, procFunc)

	// Block until we get an error from any of the iterations or the iteration finishes
	if stepErr := <-errorChan; stepErr != nil {
		cancel()
		return stepErr
	}

	return nil
}

func checkHashes(chunk *manifest.ChunkRecord, which string, sh *shell.Shell, report *reporter) error {
	h := chunk.BloomHash.String()
	// sz := int(chunk.BloomSize)
	if which == "index" {
		h = chunk.IndexHash.String()
		// sz = int(chunk.IndexSize)
	}

	hash, _, err := sh.BlockStat(h)

	report.mutex.Lock()
	defer report.mutex.Unlock()

	report.report.VisitedCnt++
	report.report.CheckedCnt++
	if err != nil {
		err = fmt.Errorf("%s %s is not pinned: %w", which, h, err)
	} else if hash != h {
		err = fmt.Errorf("%s hash (%s) mismatch (%s)", which, h, hash)
		// } else if size != sz {
		// 	err = fmt.Errorf("%s size (%d) mismatch (%d)", which, sz, size)
	} else {
		report.report.PassedCnt++
	}

	if err != nil {
		msg := fmt.Sprintf("%s %s: %s", which, chunk.Range, err)
		report.report.MsgStrings = append(report.report.MsgStrings, msg)
	}

	return nil
}
