package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"sort"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeBlaze is called each time around the forever loop prior to calling into
// Blaze to actually scrape the blocks.
func (opts *ScrapeOptions) HandleScrapeBlaze(progressThen *rpcClient.MetaData) (ok bool, err error) {

	tes := os.Getenv("TEST_END_SCRAPE")
	if tes != "" {
		val, err := strconv.ParseUint(tes, 10, 32)
		if (val != 0 && progressThen.Staging > val) || err != nil {
			return false, err
		}
	}

	// We always start one after where we last left off
	opts.StartBlock = utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized)) + 1

	// TODO: BOGUS - TESTING SCRAPING
	if utils.DebuggingOn {
		fmt.Println()
		fmt.Println("----------------------------------------------------------------------------------------------")
		logger.Log(logger.Info, "Handle ScrapeBlaze", os.Getenv("TEST_END_SCRAPE"))

		envs := opts.getEnvStrings(progressThen)
		fmt.Println("Calling with", opts.toCmdLine())
		for _, e := range envs {
			fmt.Println(e)
		}
		fmt.Println(progressThen)

		logger.Log(logger.Info, "chain:", opts.Globals.Chain)
		logger.Log(logger.Info, "start_block:", opts.StartBlock)
		logger.Log(logger.Info, "block_cnt:", opts.BlockCnt)
		logger.Log(logger.Info, "block_chan_cnt:", opts.BlockChanCnt)
		logger.Log(logger.Info, "addr_chan_cnt:", opts.AddrChanCnt)
	}

	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, false /* testMode */)
	blazeOpts := BlazeOptions{
		Chain:       opts.Globals.Chain,
		NChannels:   utils.Max(opts.BlockChanCnt, opts.AddrChanCnt),
		NProcessed:  0,
		StartBlock:  opts.StartBlock,
		BlockCount:  opts.BlockCnt,
		RipeBlock:   meta.Latest - utils.Min(meta.Latest, opts.UnripeDist),
		UnripeDist:  opts.UnripeDist,
		RpcProvider: config.GetRpcProvider(opts.Globals.Chain),
		AppMap:      make(index.AddressAppearanceMap, 500000),
	}

	_, err = blazeOpts.HandleBlaze(meta)
	// Even though we may have errored, we want to process whatever data we know we
	// have and is in a "good" state, such as timestamps
	blazeOpts.CleanupPostScrape()
	logger.Log(logger.Info, "Size of AppMap:", len(blazeOpts.AppMap))
	if err != nil {
		os.RemoveAll(config.GetPathToIndex(opts.Globals.Chain) + "ripe")
		return true, err
	}

	return true, nil
}

// bool freshenTimestampsAppend(blknum_t firstBlock, blknum_t nBlocks) {
// This routine is called after Blaze finishes. Blaze will have written a text file containing
// blockNumber,timestamp pairs that it encountered during it's pass. The job of this
// routine is to append timestamps to the timestamps database up to but not
// including firstBlock + nBlocks (so 0 and 1, if firstBlock == 0 and nBlocks == 2)
// Note that the text file may not contain every block. In fact, on many chains
// it does not. This routine has to recitfy that. Plus -- if this routine does not
// complete, it may have to start prior to firstBlock the next time, so we have to
// check the invariant: sizeof(file) == bn[last].Bn * 2 and bn[i] == i.
//     if (isTestMode())
//         return true;
//     if (!fileExists(indexFolderBin_ts)) {
//         ostringstream cmd;
//         cmd << "cd \"" << indexFolder << "\" ; ";
//         cmd << "cp \"" << chainConfigsZip_ts << "\" . ; ";
//         cmd << "gunzip ts.bin.gz";
//         establishFolder(indexFolder);
//         doCommand(cmd.str());
//         if (!fileExists(indexFolderBin_ts)) {
//             return false;
//         }
//     }
//     if (fileExists(indexFolderBin_ts + ".lck")) {  // it's being updated elsewhere
//         LOG_ERR("Timestamp file ", indexFolderBin_ts, " is locked. Cannot update. Re-running...");
//         ::remove((indexFolderBin_ts + ".lck").c_str());
//         return false;
//     }
//     // If we're already there, we're finished
//     blknum_t lastBlock = firstBlock + nBlocks;
//     size_t nRecords = ((fileSize(indexFolderBin_ts) / sizeof(uint32_t)) / 2);
//     if (nRecords >= lastBlock) {
//         return true;
//     }
//     // We always start at one less than the number of blocks already in the file
//     // (i.e., if there's two we need to add block two next.)
//     firstBlock = nRecords;
//     // We need to fill blocks firstBlock to lastBlock (non-inclusive). Make note
//     // of which ones were processed by Blaze
//     string_q blazeTsFilename = cacheFolder_tmp + "tempTsFile.txt";
//     CStringArray lines;
//     asciiFileToLines(blazeTsFilename, lines);
//     // The strings are left-padded with zeros, so we can sort them as strings
//     sort(lines.begin(), lines.end());
//     typedef map<blknum_t, timestamp_t> bn_ts_map_t;
//     bn_ts_map_t theMap;
//     for (auto line : lines) {
//         if (shouldQuit())
//             break;
//         CStringArray parts;
//         explode(parts, line, '-');
//         if (parts.size() == 2) {
//             theMap[str_2_Uint(parts[0])] = str_2_Int(parts[1]);
//         } else {
//             LOG_ERR("Line without dash (-) in temporarty timestamp file. Cannot continue.");
//             break;
//         }
//     }
//     for (blknum_t bn = firstBlock; bn < (firstBlock + nBlocks) && !shouldQuit(); bn++) {
//         if (theMap[bn] == 0) {
//             CBlock block;
//             getBlockHeader(block, bn);
//             theMap[bn] = block.timestamp;
//         }
//     }
//     lockSection();
//     CArchive file(WRITING_ARCHIVE);
//     if (!file.Lock(indexFolderBin_ts, modeWriteAppend, LOCK_NOWAIT)) {
//         LOG_ERR("Failed to open ", indexFolderBin_ts);
//         unlockSection();
//         return false;
//     }
//     for (auto item : theMap) {
//         if (shouldQuit())
//             break;
//         file << ((uint32_t)item.first) << ((uint32_t)item.second);
//         file.flush();
//         if (!DebuggingOn) {
//             ostringstream post;
//             post << " (" << (lastBlock - item.first);
//             post << " " << item.second << " - " << ts_2_Date(item.second).Format(FMT_EXPORT) << ")";
//             post << "             \r";
//             LOG_INFO(UPDATE, item.first, lastBlock, post.str());
//         }
//     }
//     file.Release();
//     unlockSection();
//     return true;
// }

func (opts *BlazeOptions) CleanupPostScrape() {
	tsFilename := config.GetPathToCache(opts.Chain) + "tmp/tempTsFile.txt"
	lines := file.AsciiFileToLines(tsFilename)
	sort.Slice(lines, func(i, j int) bool {
		return lines[i] < lines[j]
	})

	// tsArray := make([]tslib.Timestamp, len(lines))
	for _, line := range lines {
		parts := strings.Split(line, "-")
		ts := tslib.Timestamp{
			Bn: uint32(globals.ToUint64(strings.TrimLeft(parts[0], "0"))),
			Ts: uint32(globals.ToUint64(strings.TrimLeft(parts[1], "0"))),
		}
		fmt.Println(ts)
		// tsArray = append(tsArray, ts)
	}
	fmt.Println(tsFilename, file.FileSize(tsFilename))
	// tslib.Append(opts.Chain, tsArray)
}
