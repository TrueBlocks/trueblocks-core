package scrapePkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData) (ok bool, err error) {
	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	FileCounts(indexPath)

	if !opts.verifyRipeFiles() {
		return true, nil
	}

	stageFn, _ := file.LatestFileInFolder(indexPath + "staging/")
	r, _ := cache.RangeFromFilename(stageFn)
	cnt := file.FileSize(stageFn) / 59

	if utils.DebuggingOn {
		logger.Log(logger.Info, "In constructor: ", stageFn)
		logger.Log(logger.Info, r)
		logger.Log(logger.Info, cnt)
		Report("Before Call --> ", opts.StartBlock, opts.AppsPerChunk, opts.BlockCnt, uint64(cnt), 0, true)
	}

	err = opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStrings(progressThen))
	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	if utils.DebuggingOn {
		fmt.Println(meta)
	}

	FileCounts(indexPath)
	cntBeforeCall := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized))
	cntAfterCall := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized))
	if utils.DebuggingOn {
		fmt.Println("cntBeforeCall:", cntBeforeCall)
		fmt.Println("cntAfterCall:", cntAfterCall)
		fmt.Println("diff", (cntAfterCall - cntBeforeCall))
	}
	Report("After All --> ", opts.StartBlock, opts.AppsPerChunk, opts.BlockCnt, uint64(cnt), cntAfterCall-cntBeforeCall+uint64(cnt), false)

	return true, err
}

func FileCounts(indexPath string) {
	if utils.DebuggingOn {
		folders := []string{
			"finalized/",
			"blooms/",
			"staging/",
			"unripe/",
			"ripe/",
		}
		for _, folder := range folders {
			fmt.Println("Found", file.NFilesInFolder(indexPath+folder), "files in", indexPath+folder)
		}
	}
}

func Report(msg string, startBlock, nAppsPerChunk, blockCount, nRecsThen, nRecsNow uint64, hide bool) {
	if false { // ss.NRecsNow == ss.NRecsThen {
		// logger.Log(logger.Info, "No new blocks...")
	} else {
		if utils.DebuggingOn {
			logger.Log(logger.Info, "-- golang --------------------------------------------------\n", msg)
			logger.Log(logger.Info, "startBlock:   ", startBlock)
			logger.Log(logger.Info, "nAppsPerChunk:", nAppsPerChunk)
			logger.Log(logger.Info, "blockCount:   ", blockCount)
			logger.Log(logger.Info, "nRecsThen:    ", nRecsThen)
			logger.Log(logger.Info, "nRecsNow:     ", nRecsNow)
		}

		if hide {
			return
		}

		need := uint64(0)
		if nAppsPerChunk >= nRecsNow {
			need = nAppsPerChunk - nRecsNow
		}
		seen := nRecsNow - nRecsThen
		pct := float64(nRecsNow) / float64(nAppsPerChunk)
		pBlk := float64(seen) / float64(blockCount)
		height := startBlock + blockCount - 1
		msg := "Block {%d}: have {%d} addrs of {%d} ({%0.1f}%%). Need {%d} more. Found {%d} records ({%0.2f} apps/blk)."
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		logger.Log(logger.Info, fmt.Sprintf(msg, height, nRecsNow, nAppsPerChunk, pct*100, need, seen, pBlk))
	}
}

type ScraperState struct {
	StartBlock    uint64
	NRecsNow      uint64
	NRecsThen     uint64
	NAppsPerChunk uint64
	BlockCount    uint64
}

func NewScraperState(sB, nN, nT, nA, bC uint64) ScraperState {
	ss := ScraperState{}
	ss.StartBlock = sB
	ss.NRecsNow = nN
	ss.NRecsThen = nT
	ss.NAppsPerChunk = nA
	ss.BlockCount = bC
	return ss
}
