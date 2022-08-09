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

const asciiAppearanceSize = 59

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData, blazeOpts *BlazeOptions) (ok bool, err error) {
	settings, _ := scrape.GetSettings(opts.Globals.Chain, &opts.Settings)

	ripeFolder := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "ripe")

	ripeFileList, err := os.ReadDir(ripeFolder)
	if err != nil {
		return true, err
	}
	if len(ripeFileList) == 0 {
		logger.Log(logger.Info, "No new blocks...")
		return true, nil
	}

	// If we got as many as we asked for, we can assume there are no gaps...
	if uint64(len(ripeFileList)) != opts.BlockCnt {
		// Otherwise, if we have either more or less files that we scraped this round
		// something went wrong -- either this round or the preivous. In either
		// case, we can continue if the files are sequential.
		if err := IsListSequential(opts.Globals.Chain, ripeFileList); !ok || err != nil {
			// The ripe files are no sequential, clean up and try again
			index.CleanTemporaryFolders(config.GetPathToCache(opts.Globals.Chain), false)
			return true, err // 'true' means continue processing
		}
	}
	// f, _ := cache.BnsFromFilename(ripeFileList[0].Name())
	// _, l := cache.BnsFromFilename(ripeFileList[len(ripeFileList)-1].Name())
	// ripeRange := cache.FileRange{First: f, Last: l}
	// fmt.Println()
	// fmt.Println("ripeRange: ", ripeRange, index.GetStatus(opts.Globals.Chain))
	// time.Sleep(4 * time.Second)

	stageFolder := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging")
	curRange := cache.FileRange{First: opts.StartBlock, Last: opts.StartBlock + opts.BlockCnt - 1}

	// first := uint64(opts.StartBlock)
	appearances := []string{}

	stageFn, _ := file.LatestFileInFolder(stageFolder) // it's okay if it doesn't exist
	recordCount := uint64(0)

	logger.Log(logger.Info, "nFiles in ripe:", len(ripeFileList))
	logger.Log(logger.Info, "StageFn:", stageFn)
	logger.Log(logger.Info, "CurRange:", curRange)

	if file.FileExists(stageFn) {
		recordCount = uint64(file.FileSize(stageFn) / asciiAppearanceSize)
		records := file.AsciiFileToLines(stageFn)
		parts := strings.Split(records[0], "\t")
		if len(parts) > 1 {
			first, _ := strconv.ParseUint(parts[1], 10, 64)
			rng, _ := cache.RangeFromFilename(stageFn)
			curRange = cache.FileRange{First: first, Last: rng.Last}
		}
	}

	logger.Log(logger.Info, "CurRange:", curRange)
	// fmt.Println()

	// Note, this file may be empty or non-existant
	backupFn, err := file.MakeBackup(stageFn, filepath.Join(config.GetPathToCache(opts.Globals.Chain)+"tmp"))
	if err != nil {
		return true, errors.New("Could not create backup file: " + err.Error())
	}
	defer func() {
		if backupFn != "" && file.FileExists(backupFn) {
			// If the backup file exists, something failed, so we replace the original file.
			os.Rename(backupFn, stageFn)
			os.Remove(backupFn) // seems redundant, but may not be on some operating systems
		}
	}()

	appearances = file.AsciiFileToLines(stageFn)
	os.Remove(stageFn)
	for _, ripeFile := range ripeFileList {
		ripePath := filepath.Join(ripeFolder, ripeFile.Name())
		appearances = append(appearances, file.AsciiFileToLines(ripePath)...)
		os.Remove(ripePath)
		curCount := uint64(len(appearances))

		ripeRange, _ := cache.RangeFromFilename(ripePath)
		curRange.Last = ripeRange.Last

		isSnap := (curRange.Last >= settings.First_snap && (curRange.Last%settings.Snap_to_grid) == 0)
		isOvertop := (curCount >= uint64(settings.Apps_per_chunk))

		if isSnap || isOvertop {
			appMap := make(index.AddressAppearanceMap, len(appearances))
			for _, line := range appearances {
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

			logger.Log(logger.Info, colors.BrightYellow, "Writing to", path, colors.Off)
			_, err = index.WriteChunk(opts.Globals.Chain, path, appMap, len(appearances), snapper)
			if err != nil {
				return true, err
			}

			curRange.First = curRange.Last + 1
			appearances = []string{}
		}
	}

	logger.Log(logger.Info, colors.BrightYellow, "curRange", curRange, len(appearances), colors.Off)
	if len(appearances) > 0 {
		var rng cache.FileRange
		line0 := appearances[0]
		parts := strings.Split(line0, "\t")
		if len(parts) > 1 {
			rng.First, _ = strconv.ParseUint(parts[1], 10, 32)
		} else {
			return true, errors.New("Cannot find first block number at line 0 in consolidate: " + line0)
		}
		lineLast := appearances[len(appearances)-1]
		parts = strings.Split(lineLast, "\t")
		if len(parts) > 1 {
			rng.Last, _ = strconv.ParseUint(parts[1], 10, 32)
		} else {
			return true, errors.New("Cannot find last block number at lineLast in consolidate: " + lineLast)
		}
		f := fmt.Sprintf("%s.txt", rng)
		fileName := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging", f)
		err = file.LinesToAsciiFile(fileName, appearances)
		if err != nil {
			os.Remove(fileName) // cleans up by replacing the previous stage
			return true, err
		}
		logger.Log(logger.Info, colors.Red, "fileName:", fileName, colors.Off)
		logger.Log(logger.Info, colors.Red, "curRange:", curRange, colors.Off)
	}

	// time.Sleep(2 * time.Second)
	meta, _ := rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
	cntBeforeCall := utils.Max(progressThen.Ripe, utils.Max(progressThen.Staging, progressThen.Finalized))
	cntAfterCall := utils.Max(meta.Ripe, utils.Max(meta.Staging, meta.Finalized))
	Report("After All --> ", opts.StartBlock, settings.Apps_per_chunk, opts.BlockCnt, uint64(recordCount), cntAfterCall-cntBeforeCall+uint64(recordCount), false)

	os.Remove(backupFn) // commits the change

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

func IsListSequential(chain string, ripeFileList []os.DirEntry) error {
	prev := cache.NotARange
	for _, file := range ripeFileList {
		fileRange, _ := cache.RangeFromFilename(file.Name())
		if prev != cache.NotARange && prev != fileRange {
			if !fileRange.Follows(prev, !scrape.AllowMissing(chain)) {
				msg := fmt.Sprintf("Ripe files are not sequential (%s ==> %s)", prev, fileRange)
				return errors.New(msg)
			}
		}
		prev = fileRange
	}
	return nil
}
