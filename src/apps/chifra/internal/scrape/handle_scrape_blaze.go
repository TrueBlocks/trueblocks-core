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

	for bn := int(opts.StartBlock); bn < int(opts.StartBlock+opts.BlockCnt); bn++ {
		if !blazeOpts.ProcessedMap[bn] {
			// A block was not processed, clean up, report the error and return
			index.CleanTemporaryFolders(config.GetPathToIndex(opts.Globals.Chain), false)
			msg := fmt.Sprintf("Block %d was not processed%s", bn, strings.Repeat(" ", 50))
			return errors.New(msg)
		}
	}

	WriteTimestamps(blazeOpts.Chain, blazeOpts.TsArray, blazeOpts.StartBlock+blazeOpts.BlockCount)

	return nil
}

// TODO: Protect against overwriting files on disc
func WriteTimestamps(chain string, tsArray []tslib.Timestamp, endPoint uint64) error {
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
		ts := tslib.Timestamp{}
		if cnt >= len(tsArray) {
			ts = tslib.Timestamp{
				Bn: uint32(bn),
				Ts: uint32(rpcClient.GetBlockTimestamp(config.GetRpcProvider(chain), bn)),
			}
		} else {
			ts = tsArray[cnt]
			if tsArray[cnt].Bn != uint32(bn) {
				ts = tslib.Timestamp{
					Bn: uint32(bn),
					Ts: uint32(rpcClient.GetBlockTimestamp(config.GetRpcProvider(chain), bn)),
				}
				cnt-- // set it back
			}
		}

		if (bn % 13) == 0 {
			msg := fmt.Sprintf("Checking or updating timestamps %-04d of %-04d (%d remaining)%s", bn, endPoint, endPoint-bn, spaces)
			logger.Log(logger.Progress, msg)
		}

		if err = binary.Write(fp, binary.LittleEndian, &ts); err != nil {
			return err
		}

		cnt++
	}
	return nil
}
