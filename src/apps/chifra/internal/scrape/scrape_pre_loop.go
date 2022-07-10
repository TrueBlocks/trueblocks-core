// TODO: BOGUS - THINGS TO DO:
// TODO: BOGUS - CLEANUP BETWEEN RUNS IF THINGS DON'T WORK OUT AND SOMETHING HAD TO QUIT EARLY
// TODO: BOGUS - MAKE SURE WE'RE NOT RUNNING IF ACCTSCRAPE (OR ANYTHING ELSE?) IS RUNNINGs
// TODO: BOGUS - RESPOND WELL TO RUNNING AGAINST NON-ARCHIVE NODES see isArchiveNode() in C++ code
package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func (opts *ScrapeOptions) preLoop(progressThen *rpcClient.MetaData) (bool, error) {
	if os.Getenv("NO_COLOR") == "true" {
		// TODO: BOGUS - TESTING SCRAPING
		colors.ColorsOff()
	}
	logger.Log(logger.Info, "PreLoop")
	bloomZeroPath := config.GetPathToIndex(opts.Globals.Chain) + fmt.Sprintf("blooms/%09d-%09d", 0, 0) + ".bloom"
	if !file.FileExists(bloomZeroPath) {
		// logger.Log(logger.Info, "")
		// allocs, err := names.LoadPrefunds(opts.Globals.Chain)
		// if err != nil {
		// 	return true, err
		// }
		// for _, alloc := range allocs {
			// fmt.Println(alloc.Address.Hex(), alloc.Balance.Text(10))
			// os << "Wrote " << consolidatedLines.size() << " records to " << cTeal << relativize(chunkPath) << cOff
			// LOG_INFO(os.str())
		fmt.Println("Need to build zero block chunk", bloomZeroPath, file.FileExists(bloomZeroPath))
		// }
		/*
		   if (!loadPrefundBalances())
		       return usage("Could not load prefunds database.");
		   CStringArray consolidatedLines;
		   forEveryPrefund(visitPrefund, &consolidatedLines);
		   string_q chunkPath = indexFolder_finalized + chunkId + ".bin";
		   if (!writeIndexAsBinary(chunkPath, consolidatedLines)) {
		       LOG_ERR(cRed, "Failed to write index chunk for block zero.", cOff);
		       return false;
		   }
		*/
		/*
			bool visitPrefund(const Allocation& prefund, void* data) {
			    ostringstream os;
			    CStringArray* appearances = (CStringArray*)data;
			    os << prefund.address << "\t" << padNum9(0) << "\t" << padNum5((uint32_t)appearances->size()) << endl;
			    appearances->push_back(os.str());
			    return true;
			}
		*/
		/*
		   for (auto prefund : prefundBalMap) {
		       Allocation alloc;
		       alloc.address = prefund.first;
		       alloc.amount = prefund.second;
		       if (!(*func)(alloc, data))
		           return false;
		   }
		*/
		/*
		   LOG_TEST_STR("Loading prefund balances");
		   if (prefundBalMap.size() > 0) {
		       LOG_TEST_STR("Already loaded");
		       return true;
		   }
		   if (fileExists(cacheFolderBin_allocs)) {
		       CArchive archive(READING_ARCHIVE);
		       if (archive.Lock(cacheFolderBin_allocs, modeReadOnly, LOCK_NOWAIT)) {
		           uint64_t count;
		           archive >> count;
		           for (size_t i = 0; i < count; i++) {
		               wei_t amount;
		               string_q address;
		               archive >> address >> amount;
		               prefundBalMap[address] = amount;
		           }
		           archive.Release();
		           return true;
		       }
		   }
		   CStringArray lines;
		   asciiFileToLines(chainConfigsTxt_allocs, lines);
		   if (lines.size() > 0) {
		       for (auto line : lines) {
		           if (startsWith(line, "0x")) {
		               CStringArray parts;
		               explode(parts, substitute(line, "\"", ""), ',');
		               string_q address = toLower(parts[0]);
		               wei_t amount = str_2_Wei(parts[1]);
		               prefundBalMap[address] = amount;
		           }
		       }
		   } else {
		       LOG_ERR("Got zero records from ", chainConfigsTxt_allocs);
		   }
		   establishFolder(cacheFolderBin_allocs);
		   CArchive archive(WRITING_ARCHIVE);
		   if (archive.Lock(cacheFolderBin_allocs, modeWriteCreate, LOCK_NOWAIT)) {
		       archive << uint64_t(prefundBalMap.size());
		       for (const auto& item : prefundBalMap)
		           archive << item.first << item.second;
		       archive.Release();
		       return true;
		   } else {
		       LOG_ERR("Could not open prefunds cache for writing", cacheFolderBin_allocs);
		   }

		   LOG_WARN("Could not lock prefund cache at: ", cacheFolderBin_allocs);
		   return false;
		*/
	}
	return true, nil
}
