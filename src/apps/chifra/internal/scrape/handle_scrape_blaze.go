package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"encoding/binary"
	"os"
	"sort"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) HandleScrapeBlaze(progressThen *rpcClient.MetaData, blazeOpts *BlazeOptions) (ok bool, err error) {

	// Quit early if we're testing... TODO: BOGUS - REMOVE THIS
	tes := os.Getenv("TEST_END_SCRAPE")
	if tes != "" {
		val, err := strconv.ParseUint(tes, 10, 32)
		if (val != 0 && progressThen.Staging > val) || err != nil {
			return false, err
		}
	}

	// We always start one after where we last left off
	opts.StartBlock = utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized)) + 1
	if (opts.StartBlock + opts.BlockCnt) > progressThen.Latest {
		opts.BlockCnt = (progressThen.Latest - opts.StartBlock)
	}

	// TODO: BOGUS - TESTING SCRAPING
	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, false /* testMode */)

	// '28' behind head unless head is less or equal to than '28', then head
	ripe := meta.Latest
	if ripe > opts.UnripeDist {
		ripe = meta.Latest - opts.UnripeDist
	}

	*blazeOpts = BlazeOptions{
		Chain:         opts.Globals.Chain,
		NChannels:     utils.Max(opts.BlockChanCnt, opts.AddrChanCnt),
		NProcessed:    0,
		StartBlock:    opts.StartBlock,
		BlockCount:    opts.BlockCnt,
		RipeBlock:     ripe,
		UnripeDist:    opts.UnripeDist,
		RpcProvider:   config.GetRpcProvider(opts.Globals.Chain),
		AppearanceMap: make(index.AddressAppearanceMap, 500000),
		TsArray:       make([]tslib.Timestamp, 0, opts.BlockCnt),
	}

	_, err = blazeOpts.HandleBlaze(meta)
	if err != nil {
		logger.Log(logger.Info, "Error in call to Blaze", err)
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		return true, err
	}
	logger.Log(logger.Info, "Writing timestamps", len(blazeOpts.TsArray))
	blazeOpts.CleanupPostScrape()
	if DebuggingOn {
		logger.Log(logger.Info, "Size of AppMap:", len(blazeOpts.AppearanceMap))
	}

	return true, nil
}

func (opts *BlazeOptions) CleanupPostScrape() error {
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
	stop := opts.TsArray[len(opts.TsArray)-1].Bn + 1
	// fmt.Println("start:", start, "stop:", stop)
	for bn := start; bn < stop; bn++ {
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
			} else {
				// logger.Log(logger.Info, "okay ts", ts)
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
