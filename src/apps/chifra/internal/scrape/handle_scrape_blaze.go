package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"errors"
	"fmt"
	"os"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (bm *BlazeManager) HandleScrapeBlaze() error {
	chain := bm.chain

	// Do the actual scrape, wait until it finishes, clean up and return on failure
	if _, err := bm.HandleBlaze(); err != nil {
		_ = index.CleanTempIndexFolders(chain, []string{"ripe", "unripe"})
		return err
	}

	start := bm.StartBlock()
	end := bm.StartBlock() + bm.BlockCount()

	for bn := start; bn < end; bn++ {
		if !bm.processedMap[bn] {
			// At least one block was not processed. This would only happen in the event of an
			// error, so clean up, report the error and return. The loop will repeat.
			_ = index.CleanTempIndexFolders(chain, []string{"ripe", "unripe"})
			msg := fmt.Sprintf("A block %d was not processed%s", bn, strings.Repeat(" ", 50))
			return errors.New(msg)
		}
	}

	_ = bm.WriteTimestamps(end)

	return nil
}

// TODO: Protect against overwriting files on disc

func (bm *BlazeManager) WriteTimestamps(endPoint uint64) error {
	chain := bm.chain
	conn := rpc.TempConnection(chain)

	sort.Slice(bm.timestamps, func(i, j int) bool {
		return bm.timestamps[i].Bn < bm.timestamps[j].Bn
	})

	// Assume that the existing timestamps file always contains valid timestamps in a valid order so we can only append
	tsPath := config.PathToIndex(chain) + "ts.bin"
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		return err
	}

	defer func() {
		tslib.ClearCache(chain)
		fp.Close()
		// sigintTrap.Disable(trapCh)
		// writeMutex.Unlock()
	}()

	nTs, _ := tslib.NTimestamps(chain)

	cnt := 0
	for bn := nTs; bn < endPoint; bn++ {
		// Append to the timestamps file all the new timestamps but as we do that make sure we're
		// not skipping anything at the front, in the middle, or at the end of the list
		ts := tslib.TimestampRecord{}
		if cnt >= len(bm.timestamps) {
			ts = tslib.TimestampRecord{
				Bn: uint32(bn),
				Ts: uint32(conn.GetBlockTimestamp(bn)),
			}
		} else {
			ts = bm.timestamps[cnt]
			if bm.timestamps[cnt].Bn != uint32(bn) {
				ts = tslib.TimestampRecord{
					Bn: uint32(bn),
					Ts: uint32(conn.GetBlockTimestamp(bn)),
				}
				cnt-- // set it back
			}
		}

		logger.Progress((bn%13) == 0, fmt.Sprintf("Checking or updating timestamps %-04d of %-04d (%d remaining)%s", bn, endPoint, endPoint-bn, spaces))
		if err = binary.Write(fp, binary.LittleEndian, &ts); err != nil {
			return err
		}

		cnt++
	}
	return nil
}
