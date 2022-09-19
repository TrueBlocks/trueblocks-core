package scrapePkg

import (
	"errors"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config/scrapeCfg"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/paths"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

const asciiAppearanceSize = 59

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData, blazeOpts *BlazeOptions) (bool, error) {
	// Get a sorted list of files in the ripe folder
	ripeFolder := filepath.Join(config.GetPathToIndex(blazeOpts.Chain), "ripe")
	ripeFileList, err := os.ReadDir(ripeFolder)
	if err != nil {
		return true, err
	}
	if len(ripeFileList) == 0 {
		msg := fmt.Sprintf("No new blocks at block %d (%d away from head)%s", progressThen.Latest, (progressThen.Latest - progressThen.Ripe), spaces)
		logger.Log(logger.Info, msg)
		return true, nil
	}

	// If we didn't get as many ripe files as we were looking for...
	ripeCnt := len(ripeFileList)
	if uint64(ripeCnt) < (blazeOpts.BlockCount - blazeOpts.UnripeDist) {
		// Then, if they are not at least sequential, clean up and try again...
		if err := isListSequential(blazeOpts.Chain, ripeFileList); err != nil {
			index.CleanTemporaryFolders(config.GetPathToCache(blazeOpts.Chain), false)
			return true, err
		}
	}

	stageFolder := filepath.Join(config.GetPathToIndex(blazeOpts.Chain), "staging")
	stageFn, _ := file.LatestFileInFolder(stageFolder) // it may not exist...
	nAppsThen := int(file.FileSize(stageFn) / asciiAppearanceSize)

	// ripeRange := rangeFromFileList(ripeFileList)
	stageRange := paths.RangeFromFilename(stageFn)

	curRange := paths.FileRange{First: blazeOpts.StartBlock, Last: blazeOpts.StartBlock + opts.BlockCnt - 1}
	if file.FileExists(stageFn) {
		curRange = stageRange
	}

	// Note, this file may be empty or non-existant
	tmpPath := filepath.Join(config.GetPathToCache(blazeOpts.Chain) + "tmp")
	backupFn, err := file.MakeBackup(tmpPath, stageFn)
	if err != nil {
		return true, errors.New("Could not create backup file: " + err.Error())
	}

	// logger.Log(logger.Info, "Created backup file for stage")
	defer func() {
		if backupFn != "" && file.FileExists(backupFn) {
			// If the backup file exists, something failed, so we replace the original file.
			// logger.Log(logger.Info, "Replacing backed up staging file")
			os.Rename(backupFn, stageFn)
			os.Remove(backupFn) // seems redundant, but may not be on some operating systems
		}
	}()

	appearances := file.AsciiFileToLines(stageFn)
	os.Remove(stageFn) // we have a backup copy, so it's not so bad to delete it here
	for _, ripeFile := range ripeFileList {
		ripePath := filepath.Join(ripeFolder, ripeFile.Name())
		appearances = append(appearances, file.AsciiFileToLines(ripePath)...)
		os.Remove(ripePath) // if we fail halfway through, this will get noticed next time around and cleaned up
		curCount := uint64(len(appearances))

		ripeRange := paths.RangeFromFilename(ripePath)
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

			indexPath := config.GetPathToIndex(blazeOpts.Chain) + "finalized/" + curRange.String() + ".bin"
			if report, err := index.WriteChunk(blazeOpts.Chain, indexPath, appMap, len(appearances), opts.Pin, opts.Remote); err != nil {
				return false, err
			} else if report == nil {
				log.Fatal("Should not happen, write chunk returned empty report")
			} else {
				report.Snapped = isSnap
				report.Report()
			}

			curRange.First = curRange.Last + 1
			appearances = []string{}
		}
	}

	if len(appearances) > 0 {
		lineLast := appearances[len(appearances)-1]
		parts := strings.Split(lineLast, "\t")
		Last := uint64(0)
		if len(parts) > 1 {
			Last, _ = strconv.ParseUint(parts[1], 10, 32)
		} else {
			return true, errors.New("Cannot find last block number at lineLast in consolidate: " + lineLast)
		}
		m, _ := rpcClient.GetMetaData(blazeOpts.Chain, opts.Globals.TestMode)
		rng := paths.FileRange{First: m.Finalized + 1, Last: Last}
		f := fmt.Sprintf("%s.txt", rng)
		fileName := filepath.Join(config.GetPathToIndex(blazeOpts.Chain), "staging", f)
		err = file.LinesToAsciiFile(fileName, appearances)
		if err != nil {
			os.Remove(fileName) // cleans up by replacing the previous stage
			return true, err
		}
		// logger.Log(logger.Info, colors.Red, "fileName:", fileName, colors.Off)
		// logger.Log(logger.Info, colors.Red, "curRange:", curRange, colors.Off)
	}

	stageFn, _ = file.LatestFileInFolder(stageFolder) // it may not exist...
	nAppsNow := int(file.FileSize(stageFn) / asciiAppearanceSize)
	opts.Report(nAppsThen, nAppsNow)

	// logger.Log(logger.Info, "Removing backup file as it's not needed.")
	os.Remove(backupFn) // commits the change

	return true, err
}

func (opts *ScrapeOptions) Report(nAppsThen, nAppsNow int) {
	msg := "Block={%d} have {%d} appearances of {%d} ({%0.1f%%}). Need {%d} more. Added {%d} records ({%0.2f} apps/blk)."
	need := opts.Settings.Apps_per_chunk - utils.Min(opts.Settings.Apps_per_chunk, uint64(nAppsNow))
	seen := nAppsNow
	if nAppsThen < nAppsNow {
		seen = nAppsNow - nAppsThen
	}
	pct := float64(nAppsNow) / float64(opts.Settings.Apps_per_chunk)
	pBlk := float64(seen) / float64(opts.BlockCnt)
	height := opts.StartBlock + opts.BlockCnt - 1
	msg = strings.Replace(msg, "{", colors.Green, -1)
	msg = strings.Replace(msg, "}", colors.Off, -1)
	logger.Log(logger.Info, fmt.Sprintf(msg, height, nAppsNow, opts.Settings.Apps_per_chunk, pct*100, need, seen, pBlk))
}

func isListSequential(chain string, ripeFileList []os.DirEntry) error {
	prev := paths.NotARange
	allowMissing := scrapeCfg.AllowMissing(chain)
	for _, file := range ripeFileList {
		fileRange := paths.RangeFromFilename(file.Name())
		if prev != paths.NotARange && prev != fileRange {
			if !prev.Preceeds(fileRange, !allowMissing) {
				msg := fmt.Sprintf("Ripe files are not sequential (%s ==> %s)", prev, fileRange)
				return errors.New(msg)
			}
		}
		prev = fileRange
	}
	return nil
}

var spaces = strings.Repeat(" ", 40)
