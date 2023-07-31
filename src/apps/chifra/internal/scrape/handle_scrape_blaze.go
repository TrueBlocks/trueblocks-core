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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) HandleScrapeBlaze(progress *rpcClient.MetaData, blazeOpts *BlazeOptions) error {

	// Do the actual scrape, wait until it finishes, clean up and return on failure
	if _, err := blazeOpts.HandleBlaze(progress); err != nil {
		index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), false)
		return err
	}

	for bn := opts.StartBlock; bn < opts.StartBlock+opts.BlockCnt; bn++ {
		if !blazeOpts.ProcessedMap[bn] {
			// At least one block was not processed. This would only happen in the event of an
			// error, so clean up, report the error and return. The loop will repeat.
			index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), false)
			msg := fmt.Sprintf("A block %d was not processed%s", bn, strings.Repeat(" ", 50))
			return errors.New(msg)
		}
	}

	WriteTimestamps(blazeOpts.Chain, blazeOpts.TsArray, blazeOpts.StartBlock+blazeOpts.BlockCount)

	return nil
}

// TODO: Protect against overwriting files on disc

func WriteTimestamps(chain string, tsArray []tslib.TimestampRecord, endPoint uint64) error {
	rpcOptions := rpcClient.DefaultRpcOptions(&rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
	})

	sort.Slice(tsArray, func(i, j int) bool {
		return tsArray[i].Bn < tsArray[j].Bn
	})

	// Assume that the existing timestamps file always contains valid timestamps in a valid order so we can only append
	tsPath := config.GetPathToIndex(chain) + "ts.bin"
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		return err
	}

	defer func() {
		tslib.DeCache(chain)
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
		if cnt >= len(tsArray) {
			ts = tslib.TimestampRecord{
				Bn: uint32(bn),
				Ts: uint32(rpcOptions.GetBlockTimestamp(chain, &bn)),
			}
		} else {
			ts = tsArray[cnt]
			if tsArray[cnt].Bn != uint32(bn) {
				ts = tslib.TimestampRecord{
					Bn: uint32(bn),
					Ts: uint32(rpcOptions.GetBlockTimestamp(chain, &bn)),
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
