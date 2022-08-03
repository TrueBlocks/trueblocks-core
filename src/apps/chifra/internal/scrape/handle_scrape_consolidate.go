package scrapePkg

import (
	"errors"
	"fmt"
	"os"
	"path"
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
	indexPath := config.GetPathToIndex(opts.Globals.Chain)
	cachePath := config.GetPathToCache(opts.Globals.Chain)
	FileCounts(indexPath, cachePath)

	stageFn, _ := file.LatestFileInFolder(indexPath + "staging/")
	// r, _ := cache.RangeFromFilename(stageFn)
	cnt := file.FileSize(stageFn) / asciiAppearanceSize

	opts.Consolidate(blazeOpts)
	// opts.ListIndexFolder(indexPath, "After Consolidate")

	// if DebuggingOn {
	// 	newPinsFn := config.GetPathToCache(opts.Globals.Chain) + "tmp/chunks_created.txt"
	// 	fmt.Println(newPinsFn)
	// 	fmt.Println(file.AsciiFileToString(newPinsFn))
	// }

	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	// if DebuggingOn {
	// 	fmt.Println(meta)
	// }

	// FileCounts(indexPath, cachePath)
	cntBeforeCall := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized))
	cntAfterCall := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized))
	// if DebuggingOn {
	// 	fmt.Println("cntBeforeCall:", cntBeforeCall)
	// 	fmt.Println("cntAfterCall:", cntAfterCall)
	// 	fmt.Println("diff", (cntAfterCall - cntBeforeCall))
	// }
	Report("After All --> ", opts.StartBlock, opts.AppsPerChunk, opts.BlockCnt, uint64(cnt), cntAfterCall-cntBeforeCall+uint64(cnt), false)

	return true, err
}

func FileCounts(indexPath, cachePath string) {
	// if DebuggingOn {
	// 	folders := []string{
	// 		"finalized/",
	// 		"blooms/",
	// 		"staging/",
	// 		"unripe/",
	// 		"ripe/",
	// 	}
	// 	for _, folder := range folders {
	// 		fmt.Println("Found", file.NFilesInFolder(indexPath+folder), "files in", indexPath+folder)
	// 	}
	// 	fmt.Println("Found", file.NFilesInFolder(cachePath+"tmp/"), "files in", cachePath+"tmp/")
	// }
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

func (opts *ScrapeOptions) Consolidate(blazeOpts *BlazeOptions) error {
	if !opts.verifyRipeFiles(opts.StartBlock) {
		logger.Log(logger.Error, "verifyRipeFiles failed")
		config.CleanIndexFolder(config.GetPathToCache(opts.Globals.Chain))
		return errors.New("non-sequential files -- rescanning")
	}

	ripeFolder := config.GetPathToIndex(opts.Globals.Chain) + "ripe/"
	files, err := os.ReadDir(ripeFolder)
	if err != nil {
		return err
	}

	settings := scrape.GetSettings(opts.Globals.Chain)
	stagingFolder := config.GetPathToIndex(opts.Globals.Chain) + "staging/"
	firstFile, err := file.EarliestFileInFolder(stagingFolder)
	if err != nil {
		logger.Log(logger.Info, "earliest file not found, it's okay", err)
	}

	recordCount := uint64(file.FileSize(firstFile) / asciiAppearanceSize)

	first := uint64(1)
	if recordCount > 0 {
		records := file.AsciiFileToLines(firstFile)
		parts := strings.Split(records[0], "\t")
		if len(parts) > 1 {
			first, _ = strconv.ParseUint(parts[1], 10, 64)
		}
	}

	allApps := file.AsciiFileToLines(firstFile)
	os.Remove(firstFile)
	curRange := cache.FileRange{First: first, Last: 0}
	bn := 0
	for _, ff := range files {
		path := filepath.Join(ripeFolder, ff.Name())
		allApps = append(allApps, file.AsciiFileToLines(path)...)
		os.Remove(path)

		fR, _ := cache.RangeFromFilename(ff.Name())
		bn = int(fR.First)
		isSnap := (bn > settings.First_snap && (bn%settings.Snap_to_grid) == 0)

		recordCount1 := uint64(len(allApps))
		isOvertop := (recordCount1 >= uint64(settings.Apps_per_chunk))

		// final := (ff.Name() == files[len(files)-1].Name())
		curRange.Last = utils.Max(curRange.Last, uint64(bn))

		// if isSnap {
		// 	curRange.Last = utils.Max(curRange.Last, uint64(bn))
		// logger.Log(logger.Info, strings.Repeat("~", 100))
		// logger.Log(logger.Info, "Snap at", curRange.First, curRange.Last)
		// } else if isOvertop {
		// 	curRange.Last = utils.Max(curRange.Last, uint64(bn))
		// logger.Log(logger.Info, strings.Repeat("~", 100))
		// logger.Log(logger.Info, "Overtops at", curRange.First, curRange.Last)
		// } else {
		// 	if final {
		// 		logger.Log(logger.Info, strings.Repeat("~", 100))
		// 		logger.Log(logger.Info, "Final", curRange.First, curRange.Last)
		// 	}
		// }

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
			// logger.Log(logger.Info, "Would have written:", len(allApps), "records and", len(appMap), "addresses to", curRange)
			filename := cache.NewCachePath(opts.Globals.Chain, cache.Index_Final)
			path := filename.GetFullPath(curRange.String())
			snapper := -1
			if isSnap {
				snapper = int(settings.Snap_to_grid)
			}
			_, err = index.WriteChunk(opts.Globals.Chain, path, appMap, len(allApps), snapper)
			if err != nil {
				return err
			}
			curRange.First = curRange.Last + 1
			allApps = []string{}
		}
	}
	f := fmt.Sprintf("%09d.txt", (curRange.First + uint64(len(allApps)) - 1))
	fileName := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging", f)
	// logger.Log(logger.Info, "Left over:", len(allApps), "written to", fileName)
	return file.LinesToAsciiFile(fileName, allApps)
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

func (opts *ScrapeOptions) verifyRipeFiles(start uint64) bool {
	ripePath := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "ripe")
	files, err := os.ReadDir(ripePath)
	if err != nil {
		fmt.Println(err.Error())
		return false
	}
	// logger.Log(logger.Info, "verifyRipeFiles", ripePath, "nFiles:", len(files), scrape.AllowMissing(opts.Globals.Chain))

	prev := cache.FileRange{First: start, Last: start}
	for _, file := range files {
		fR, _ := cache.RangeFromFilename(file.Name())
		if prev != fR {
			if !fR.Follows(prev, !scrape.AllowMissing(opts.Globals.Chain)) {
				logger.Log(logger.Error, "Current file (", file.Name(), ") does not sequentially follow previous file ", prev.First, ".")
				// TODO: BOGUS THIS IS NEARLY IDENTICAL TO CleanIndexFolder BUT DOESN'T REMOVE STAGING - MAKE CLEANING ON FAILED PROCESSING CONSISTENET
				for _, f := range []string{"ripe", "unripe", "maps"} {
					folder := path.Join(config.GetPathToIndex(opts.Globals.Chain), f)
					err := os.RemoveAll(folder)
					if err != nil {
						logger.Log(logger.Error, "Cannot delete folder (", folder, ").")
						return false
					}
				}
				return false
			}
		}
		prev = fR
	}

	return true
}
