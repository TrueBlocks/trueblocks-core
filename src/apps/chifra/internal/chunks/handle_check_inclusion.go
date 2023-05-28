// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"context"
	"fmt"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index/bloom"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type reporter struct {
	chunk  *manifest.ChunkRecord
	report *simpleReportCheck
}

// CheckInclusion digs deep into the index chunk and makes sure that all addresses in the chunk return
// true when tested against the bloom filter. It requires an IPFS node to be running locally.
func (opts *ChunksOptions) CheckInclusion(man *manifest.Manifest, report *simpleReportCheck) error {
	theMap := make(map[string]*reporter)
	for _, chunk := range man.ChunkMap {
		theMap[chunk.Range] = &reporter{chunk, report}
	}

	total := len(theMap)
	var done atomic.Int32
	progressChan := make(chan int)
	defer close(progressChan)
	// Listen on a channel and whenever it updates, call `reportProgress`
	go func() {
		for progress := range progressChan {
			doneNow := done.Add(int32(progress))
			logger.PctProgress(doneNow, total)
		}
	}()

	ctx, cancel := context.WithCancel(context.Background())
	defer cancel()
	errorChannel := make(chan error)
	utils.IterateOverMap(ctx, errorChannel, theMap, func(rangeStr string, item *reporter) (err error) {
		rng := base.RangeFromRangeString(item.chunk.Range)
		_, path := rng.RangeToFilename(opts.Globals.Chain)
		// fmt.Println("key:", chunk.Range, "path:", cache.ToBloomPath(path))
		// fmt.Println("key:", chunk.Range, "path:", cache.ToIndexPath(path))
		report.VisitedCnt++
		report.CheckedCnt++
		report.PassedCnt++
		bloomFilename := cache.ToBloomPath(path)
		bl, err := bloom.NewChunkBloom(bloomFilename)
		if err != nil {
			return
		}
		defer bl.Close()

		// for _, mon := range updater.MonitorMap {
		test := base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b")
		if bl.IsMember(test) {
			fmt.Println("X", colors.Yellow, "bloom hit", test, "in", item.chunk.Range, colors.Off)
			// time.Sleep(1 * time.Microsecond)
		}
		// }

		return nil
	})

	// Block until we get an error from any of the iterations or the iteration finishes
	if stepErr := <-errorChannel; stepErr != nil {
		cancel()
		return stepErr
	}

	return nil
}
