// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"encoding/binary"
	"fmt"
	"io"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	shell "github.com/ipfs/go-ipfs-api"
)

type reporter struct {
	chunk  *types.ChunkRecord
	report *types.ReportCheck
	mutex  *sync.Mutex
}

// CheckDeep digs deep into the data. In `index` mode, it opens each index and checks
// that all addresses in the index return true when checked against its corresponding
// Bloom filter. In `manifest` mode, it checks that each IPFS hash in the manifest is
// actually pinned. The later requires a locally running IPFS node.
func (opts *ChunksOptions) CheckDeep(cacheMan *manifest.Manifest, report *types.ReportCheck) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	nErrors := 0

	mutex := sync.Mutex{}
	appMap := make(map[string]*reporter)
	for _, chunk := range cacheMan.ChunkMap {
		appMap[chunk.Range] = &reporter{chunk, report, &mutex}
	}

	showProgress := opts.Globals.ShowProgress()
	bar := logger.NewBar(logger.BarOptions{
		Enabled: showProgress,
		Total:   int64(len(appMap)),
	})

	addrCnt := 0

	var sh *shell.Shell
	var iterFunc func(rangeStr string, item *reporter) (err error)
	if opts.Mode == "index" {
		logger.Info("Checking each address in each index against its Bloom filter...")
		iterFunc = func(rangeStr string, item *reporter) (err error) {
			_ = rangeStr
			rng := base.RangeFromRangeString(item.chunk.Range)
			path := rng.RangeToFilename(chain)
			bl, err := index.OpenBloom(index.ToBloomPath(path), true /* check */)
			if err != nil {
				return
			}
			defer bl.Close()

			misses := 0
			path = index.ToIndexPath(path) // it may not exist if user did not do chifra init --all for example
			if file.FileExists(path) {
				indexChunk, err := index.OpenIndex(path, true /* check */)
				if err != nil {
					return err
				}
				defer indexChunk.Close()

				_, err = indexChunk.File.Seek(int64(index.HeaderWidth), io.SeekStart)
				if err != nil {
					return err
				}

				for i := 0; i < int(indexChunk.Header.AddressCount); i++ {
					obj := types.AddrRecord{}
					if err := binary.Read(indexChunk.File, binary.LittleEndian, &obj); err != nil {
						return err
					}
					if !bl.IsMember(obj.Address) {
						fmt.Println("X", colors.Yellow, "bloom miss", obj.Address, "in", item.chunk.Range, colors.Off)
						misses++
					}
					addrCnt++
					if i%8000 == 0 {
						bar.Prefix = fmt.Sprintf("Checked %d addresses against %d Blooms", addrCnt, len(appMap))
						bar.Tick()
					}
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
			return nil
		}

	} else if opts.Mode == "manifest" {
		sh = shell.NewShell(config.GetPinning().LocalPinUrl)
		iterFunc = func(rangeStr string, item *reporter) (err error) {
			_ = rangeStr
			bar.Tick()
			err = checkHashes(item.chunk, "bloom", sh, item)
			if err != nil {
				return err
			}
			return checkHashes(item.chunk, "index", sh, item)
		}
	} else {
		return fmt.Errorf("unknown mode: %s", opts.Mode)
	}

	iterErrorChan := make(chan error)
	iterCtx, iterCancel := context.WithCancel(context.Background())
	defer iterCancel()
	go utils.IterateOverMap(iterCtx, iterErrorChan, appMap, iterFunc)
	for err := range iterErrorChan {
		if !testMode || nErrors == 0 {
			logger.Fatal(err)
			nErrors++
		}
	}
	bar.Finish(true /* newLine */)

	return nil
}

func checkHashes(chunk *types.ChunkRecord, which string, sh *shell.Shell, report *reporter) error {
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
