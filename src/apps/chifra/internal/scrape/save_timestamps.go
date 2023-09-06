package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"fmt"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// TODO: Protect against overwriting files on disc

func (bm *BlazeManager) WriteTimestamps(blocks []base.Blknum) error {
	chain := bm.chain

	sort.Slice(bm.timestamps, func(i, j int) bool {
		return bm.timestamps[i].Bn < bm.timestamps[j].Bn
	})

	// Assume that the existing timestamps file always contains valid timestamps in
	// a valid order so we can only append as we go (which is very fast)
	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		return err
	}

	defer func() {
		tslib.ClearCache(chain)
		fp.Close()
	}()

	cnt := 0
	for _, block := range blocks {
		// Append to the timestamps file all the new timestamps but as we do that make sure we're
		// not skipping anything at the front, in the middle, or at the end of the list
		ts := tslib.TimestampRecord{}
		if cnt >= len(bm.timestamps) {
			ts = tslib.TimestampRecord{
				Bn: uint32(block),
				Ts: uint32(bm.opts.Conn.GetBlockTimestamp(block)),
			}
		} else {
			ts = bm.timestamps[cnt]
			if bm.timestamps[cnt].Bn != uint32(block) {
				ts = tslib.TimestampRecord{
					Bn: uint32(block),
					Ts: uint32(bm.opts.Conn.GetBlockTimestamp(block)),
				}
				cnt-- // set it back
			}
		}

		msg := fmt.Sprintf("Updating timestamps %-04d of %-04d (%-04d remaining)"+spaces,
			block,
			blocks[len(blocks)-1],
			blocks[len(blocks)-1]-block,
		)
		logger.Progress((block%13) == 0, msg)

		if err = binary.Write(fp, binary.LittleEndian, &ts); err != nil {
			return err
		}

		cnt++
	}

	return nil
}
