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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) HandleScrapeBlaze(progress *rpcClient.MetaData, blazeOpts *BlazeOptions) (ok bool, err error) {
	_, err = blazeOpts.HandleBlaze(progress)

	if err != nil {
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "unripe")
		logger.ExitError("HandleScrapeBlaze returned error", err, ". Cleaned up.")
		return true, err
	}
	cnt := 0
	for i := int(opts.StartBlock); i < int(opts.StartBlock+opts.BlockCnt); i++ {
		if !blazeOpts.ProcessedMap[i] {
			logger.Log(logger.Info, "block", i, "not processed")
			cnt++
		}
	}
	if cnt > 0 {
		msg := fmt.Sprintf("%d items were not processed", cnt)
		logger.Log(logger.Info, msg)
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "unripe")
		logger.ExitError("HandleScrapeBlaze returned error", err, ". Cleaned up.")
		return true, errors.New(msg)
	}

	blazeOpts.WriteTimestamps()

	return true, nil
}

func (opts *BlazeOptions) WriteTimestamps() error {
	sort.Slice(opts.TsArray, func(i, j int) bool {
		return opts.TsArray[i].Bn < opts.TsArray[j].Bn
	})

	// Assume that the timestamps file always contains valid timestamps
	tsPath := config.GetPathToIndex(opts.Chain) + "ts.bin"
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_APPEND|os.O_CREATE, 0644)
	if err != nil {
		return err
	}

	defer func() {
		tslib.DeCache(opts.Chain)
		fp.Close()
		// TODO: BOGUS - DOn'T ALLOW CONTROL+C and ONE WRITER
		// sigintTrap.Disable(trapCh)
		// writeMutex.Unlock()
	}()

	// Add, to the end of the timestamps file, all the timestamps in the array with the
	// following caveat: fill in any missing timestamps including those that may be
	// missing from the front of the array
	cnt := 0
	nTs, _ := tslib.NTimestamps(opts.Chain)
	start := uint32(nTs)
	stop := uint32(opts.StartBlock + opts.BlockCount)

	for bn := start; bn < stop; bn++ {
		if (bn % 13) == 0 {
			f := "-------- ( ------)- <PROG>  : Checking timestamps %-04d of %-04d\r"
			fmt.Fprintf(os.Stderr, f, bn, stop)
		}
		ts := tslib.Timestamp{}
		if cnt >= len(opts.TsArray) {
			ts = tslib.Timestamp{
				Bn: bn,
				Ts: uint32(rpcClient.GetBlockTimestamp(config.GetRpcProvider(opts.Chain), uint64(bn))),
			}
			// logger.Log(logger.Info, "missing ts after", ts)
		} else {
			ts = opts.TsArray[cnt]
			if opts.TsArray[cnt].Bn != bn {
				ts = tslib.Timestamp{
					Bn: bn,
					Ts: uint32(rpcClient.GetBlockTimestamp(config.GetRpcProvider(opts.Chain), uint64(bn))),
				}
				// logger.Log(logger.Info, "missing ts in or before", ts)
				cnt-- // set it back
				// } else {
				// 	logger.Log(logger.Info, "okay ts", ts)
			}
		}
		err = binary.Write(fp, binary.LittleEndian, &ts)
		if err != nil {
			return err
		}
		cnt++
	}
	return nil
}
