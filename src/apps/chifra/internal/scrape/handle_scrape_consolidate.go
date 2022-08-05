package scrapePkg

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrape"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// TODO: BOGUS
const DebuggingOn = true
const asciiAppearanceSize = 59

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData, blazeOpts *BlazeOptions) (ok bool, err error) {
	ripePath := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "ripe")
	ripeFiles, err := os.ReadDir(ripePath)
	if err != nil {
		return true, err
	}

	allow_missing := scrape.AllowMissing(opts.Globals.Chain)

	prev := cache.NotARange
	for _, file := range ripeFiles {
		fileRange, _ := cache.RangeFromFilename(file.Name())
		if prev != cache.NotARange && prev != fileRange {
			if !fileRange.Follows(prev, !allow_missing) {
				msg := fmt.Sprintf("Current file (%s) does not sequentially follow previous file (%09d.txt).\n", file.Name(), prev.First)
				logger.Log(logger.Error, msg)
				err = index.CleanTemporaryFolders(config.GetPathToCache(opts.Globals.Chain), false)
				if err != nil {
					return true, err
				}
				return true, errors.New(msg)
			}
		}
		prev = fileRange
	}

	stageFn, _ := file.LatestFileInFolder(filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging"))
	cnt := file.FileSize(stageFn) / asciiAppearanceSize
	stagingFolder := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging")
	firstFile, err := file.EarliestFileInFolder(stagingFolder)
	if err != nil {
		logger.Log(logger.Info, "earliest file not found, it's okay", err)
	}

	recordCount := uint64(file.FileSize(firstFile) / asciiAppearanceSize)

	first := uint64(opts.StartBlock)
	if recordCount > 0 {
		records := file.AsciiFileToLines(firstFile)
		parts := strings.Split(records[0], "\t")
		if len(parts) > 1 {
			first, _ = strconv.ParseUint(parts[1], 10, 64)
		}
	}

	settings := scrape.GetSettings(opts.Globals.Chain)

	allApps := file.AsciiFileToLines(firstFile)
	os.Remove(firstFile)
	curRange := cache.FileRange{First: first, Last: 0}
	bn := 0
	for _, ff := range ripeFiles {
		path := filepath.Join(ripePath, ff.Name())
		allApps = append(allApps, file.AsciiFileToLines(path)...)
		os.Remove(path)

		fR, _ := cache.RangeFromFilename(ff.Name())
		bn = int(fR.First)
		isSnap := (bn >= settings.First_snap && (bn%settings.Snap_to_grid) == 0)

		recordCount1 := uint64(len(allApps))
		isOvertop := (recordCount1 >= uint64(settings.Apps_per_chunk))

		curRange.Last = utils.Max(curRange.Last, uint64(bn))
		if isSnap || isOvertop {
			appMap := make(index.AddressAppearanceMap, len(allApps))
			for _, line := range allApps {
				parts := strings.Split(line, "\t")
				if len(parts) == 3 {
					addr := strings.ToLower(parts[0])
					bn, _ := strconv.ParseUint(parts[1], 10, 32)
					txid, _ := strconv.ParseUint(parts[2], 10, 32)
					appMap[addr] = append(appMap[addr], index.AppearanceRecord{
						BlockNumber:   uint32(bn),
						TransactionId: uint32(txid),
					})
				}
			}
			filename := cache.NewCachePath(opts.Globals.Chain, cache.Index_Final)
			path := filename.GetFullPath(curRange.String())
			snapper := -1
			if isSnap {
				snapper = int(settings.Snap_to_grid)
			}
			_, err = index.WriteChunk(opts.Globals.Chain, path, appMap, len(allApps), snapper)
			if err != nil {
				return true, err
			}
			curRange.First = curRange.Last + 1
			allApps = []string{}
		}
	}

	if len(allApps) > 0 {
		line := allApps[len(allApps)-1]
		parts := strings.Split(line, "\t")
		if len(parts) > 1 {
			bn, _ := strconv.ParseUint(parts[1], 10, 32)
			f := fmt.Sprintf("%09d.txt", bn)
			fileName := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging", f)
			err = file.LinesToAsciiFile(fileName, allApps)
			if err != nil {
				return true, err
			}
		}
	}

	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	cntBeforeCall := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized))
	cntAfterCall := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized))
	Report("After All --> ", opts.StartBlock, opts.AppsPerChunk, opts.BlockCnt, uint64(cnt), cntAfterCall-cntBeforeCall+uint64(cnt), false)

	return true, err
}

func Report(msg string, startBlock, nAppsPerChunk, blockCount, nRecsThen, nRecsNow uint64, hide bool) {
	if nRecsNow == nRecsThen {
		logger.Log(logger.Info, "No new blocks...")

	} else {
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
		msg := "Block {%d}: have {%d} addrs of {%d} ({%0.1f%%}). Need {%d} more. Found {%d} records ({%0.2f} apps/blk)."
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

func (opts *ScrapeOptions) ListIndexFolder(indexPath, msg string) {
	// logger.Log(logger.Info, "======================= Enter", msg, "=======================")
	// filepath.Walk(indexPath, func(fullPath string, info os.FileInfo, err error) error {
	// 	if err != nil {
	// 		return err
	// 	}
	// 	rel := strings.Replace(fullPath, indexPath, "./", -1)
	// 	if strings.Contains(fullPath, "finalized") && strings.HasSuffix(fullPath, ".bin") {
	// 		header, _ := index.ReadChunkHeader(opts.Globals.Chain, fullPath)
	// 		logger.Log(logger.Info, rel, strings.Replace(header.String(), "0x81ae14ba68e372bc9bd4a295b844abd8e72b1de10fcd706e624647701d911da1", "0x81ae...1da1", -1))
	// 	} else if strings.HasSuffix(fullPath, ".bin") || strings.HasSuffix(fullPath, ".bloom") {
	// 		logger.Log(logger.Info, rel, info.Size())
	// 	} else if strings.HasSuffix(fullPath, ".txt") {
	// 		logger.Log(logger.Info, rel, info.Size()/asciiAppearanceSize)
	// 	}
	// 	return nil
	// })
	// logger.Log(logger.Info, "======================= Exit", msg, "=======================")
}
