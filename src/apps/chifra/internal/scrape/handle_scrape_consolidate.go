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
)

const asciiAppearanceSize = 59

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData, blazeOpts *BlazeOptions) (bool, error) {
	// Get a sorted list of files in the ripe folder
	ripeFolder := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "ripe")
	ripeFileList, err := os.ReadDir(ripeFolder)
	if err != nil {
		return true, err
	}
	if len(ripeFileList) == 0 {
		logger.Log(logger.Info, "No new blocks...")
		return true, nil
	}

	// If we didn't get as many ripe files as we were looking for...
	if uint64(len(ripeFileList)) != opts.BlockCnt {
		// Then, if they are not at least sequential, clean up and try again...
		if err := isListSequential(opts.Globals.Chain, ripeFileList); err != nil {
			index.CleanTemporaryFolders(config.GetPathToCache(opts.Globals.Chain), false)
			return true, err
		}
	}

	stageFolder := filepath.Join(config.GetPathToIndex(opts.Globals.Chain), "staging")
	stageFn, _ := file.LatestFileInFolder(stageFolder) // it may not exist...

	ripeRange := rangeFromFileList(ripeFileList)
	curRange := cache.FileRange{First: opts.StartBlock, Last: opts.StartBlock + opts.BlockCnt - 1}
	stageRange, _ := cache.RangeFromFilename(stageFn)

	nRecsThen := uint64(0)
	if file.FileExists(stageFn) {
		curRange = stageRange
		nRecsThen = uint64(file.FileSize(stageFn) / asciiAppearanceSize)
	}

	logger.Log(logger.Info, "ripeFolder: ", ripeFolder)
	logger.Log(logger.Info, "stageFolder:", stageFolder)
	logger.Log(logger.Info, "nRipes:     ", len(ripeFileList))
	logger.Log(logger.Info, "stageFn:    ", stageFn)
	logger.Log(logger.Info, "stageRange: ", stageRange)
	logger.Log(logger.Info, "ripeRange:  ", ripeRange)
	logger.Log(logger.Info, "curRange:   ", curRange)

	// Note, this file may be empty or non-existant
	backupFn, err := file.MakeBackup(stageFn, filepath.Join(config.GetPathToCache(opts.Globals.Chain)+"tmp"))
	if err != nil {
		return true, errors.New("Could not create backup file: " + err.Error())
	}

	logger.Log(logger.Info, "Created backup file for stage")
	defer func() {
		if backupFn != "" && file.FileExists(backupFn) {
			// If the backup file exists, something failed, so we replace the original file.
			logger.Log(logger.Info, "Replacing backed up staging file")
			os.Rename(backupFn, stageFn)
			os.Remove(backupFn) // seems redundant, but may not be on some operating systems
		}
	}()

	nAdded := uint64(0)
	appearances := file.AsciiFileToLines(stageFn)
	os.Remove(stageFn)
	for _, ripeFile := range ripeFileList {
		ripePath := filepath.Join(ripeFolder, ripeFile.Name())
		appearances = append(appearances, file.AsciiFileToLines(ripePath)...)
		os.Remove(ripePath)
		curCount := uint64(len(appearances))

		ripeRange, _ := cache.RangeFromFilename(ripePath)
		curRange.Last = ripeRange.Last

		isSnap := (curRange.Last >= opts.Settings.First_snap && (curRange.Last%opts.Settings.Snap_to_grid) == 0)
		isOvertop := (curCount >= uint64(opts.Settings.Apps_per_chunk))

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
				snapper = int(opts.Settings.Snap_to_grid)
			}

			logger.Log(logger.Info, colors.BrightYellow, "Writing to", path, colors.Off)
			_, err = index.WriteChunk(opts.Globals.Chain, path, appMap, len(appearances), snapper)
			if err != nil {
				return true, err
			}
			nAdded += uint64(len(appearances))

			curRange.First = curRange.Last + 1
			appearances = []string{}
		}
	}

	logger.Log(logger.Info, colors.BrightYellow, "curRange", curRange, len(appearances), colors.Off)
	if len(appearances) > 0 {
		nAdded += uint64(len(appearances))
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

	// TODO: BOGUS - THIS PROBABLY DOESN'T WORK - NOT SURE WHAT IT'S SUPPOSED TO DO
	nRecsNow := nRecsThen + nAdded
	Report(opts.StartBlock, opts.Settings.Apps_per_chunk, opts.BlockCnt, nRecsThen, nRecsNow, false)

	logger.Log(logger.Info, "Removing backup file as it's not needed.")
	os.Remove(backupFn) // commits the change

	return true, err
}

func Report(startBlock, nAppsPerChunk, blockCount, nRecsThen, nRecsNow uint64, hide bool) {
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

func isListSequential(chain string, ripeFileList []os.DirEntry) error {
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

func rangeFromFileList(list []os.DirEntry) cache.FileRange {
	if len(list) == 0 {
		return cache.FileRange{}
	}
	f, _ := cache.BnsFromFilename(list[0].Name())
	_, l := cache.BnsFromFilename(list[len(list)-1].Name())
	return cache.FileRange{First: f, Last: l}
}
