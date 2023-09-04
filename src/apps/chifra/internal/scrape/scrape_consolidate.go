package scrapePkg

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

const asciiAppearanceSize = 59

// Consolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (bm *BlazeManager) Consolidate(blocks []base.Blknum) (error, bool) {
	chain := bm.chain
	settings := bm.opts.Settings
	testMode := bm.opts.Globals.TestMode

	ripeFolder := filepath.Join(config.GetPathToIndex(chain), "ripe")
	stageFolder := filepath.Join(config.GetPathToIndex(chain), "staging")

	ripeFiles, err := os.ReadDir(ripeFolder)
	if err != nil {
		return err, true
	}

	if len(ripeFiles) == 0 {
		// There are no new ripeFiles, but we've processed all blocks in the block
		// list. We want to rename the stage file (if it exists) to reflect the
		// new stage head. If the file doesn't exist, just touch it. The scraper
		// uses the stage file's name to know where to start the next loop.
		curStage, _ := file.LatestFileInFolder(stageFolder) // it may not exist...
		curRange := base.RangeFromFilename(curStage)

		// All blocks in the list have been processed, so we know where the stage ends.
		newRange := base.FileRange{First: curRange.First, Last: blocks[len(blocks)-1]}
		newStage := filepath.Join(stageFolder, newRange.String()+".txt")

		if file.FileExists(curStage) {
			_ = os.Rename(curStage, newStage)
			_ = os.Remove(curStage) // seems redundant, but may not be on some operating systems
		} else {
			file.Touch(newStage)
		}

		// Let the user know something happened...
		return fmt.Errorf("no new ripe blocks found at %d (%d away from head)",
			bm.meta.ChainHeight(),
			bm.meta.ChainHeight() - bm.meta.Ripe,
		), true
	}

	// If the number of ripe files is less than we expected...
	if uint64(len(ripeFiles)) < (bm.BlockCount() - settings.Unripe_dist) {
		// ...at least make sure they're sequential (if the chain so desires).
		if err := bm.isListSequential(chain, ripeFiles); err != nil {
			_ = index.CleanTemporaryFolders(config.GetPathToCache(chain), false)
			return err, true
		}
	}

	stageFn, _ := file.LatestFileInFolder(stageFolder) // it may not exist...
	stageRange := base.RangeFromFilename(stageFn)
	nAppsThen := int(file.FileSize(stageFn) / asciiAppearanceSize)

	var curRange base.FileRange
	if file.FileExists(stageFn) {
		curRange = stageRange
	} else {
		curRange = base.FileRange{First: blocks[0], Last: blocks[len(blocks)-1]}
	}

	// Note, the stage may be empty or non-existant
	backupFn, err := file.MakeBackup(filepath.Join(config.GetPathToCache(chain)+"tmp"), stageFn)
	if err != nil {
		return errors.New("Could not create backup file: " + err.Error()), true
	}
	defer func() {
		// If the backup file exists, the function did not complete. In that case, we replace the original file.
		if backupFn != "" && file.FileExists(backupFn) {
			_ = os.Rename(backupFn, stageFn)
			_ = os.Remove(backupFn) // seems redundant, but may not be on some operating systems
		}
	}()

	// Note:
	// if we fail at any point past this, the backup file will replace the original file

	// Start by copying in the existing appearances on the stage...
	appearances := file.AsciiFileToLines(stageFn)
	os.Remove(stageFn) // We have a backup copy, so it's okay to remove this here...

	// Spin through each ripe file and add it to the appearances array until we have either (a) a snap
	// point or (b) enough appearances to overtop. In either case, we consolidate the appearances into
	// a chunk, create its bloom, and write it to disk. Note that each time we copy one of the ripe
	// files, we remove it.
	for _, ripeFile := range ripeFiles {
		thisFile := filepath.Join(ripeFolder, ripeFile.Name())
		ripeRange := base.RangeFromFilename(thisFile)
		curRange.Last = ripeRange.Last

		// append the appearances from this file
		appearances = append(appearances, file.AsciiFileToLines(thisFile)...)
		os.Remove(thisFile) // if we fail halfway through, this will get noticed next time around and cleaned up
		curCount := uint64(len(appearances))

		isSnap := (curRange.Last >= settings.First_snap && (curRange.Last%settings.Snap_to_grid) == 0)
		isOvertop := curCount >= settings.Apps_per_chunk

		if isSnap || isOvertop {
			// We've found a chunk. We can consolidate it. A chunk is a relational table relating
			// a table of addresses to where they appear in a table of appearances. We store, for
			// each address, its appearances.
			appMap := make(index.AddressAppearanceMap, len(appearances))
			for _, line := range appearances {
				parts := strings.Split(line, "\t")
				if len(parts) != 3 {
					// protect ourselves -- TODO: should we report this?
					continue
				}
				addr := strings.ToLower(parts[0])
				appMap[addr] = append(appMap[addr], index.AppearanceRecord{
					BlockNumber:   uint32(utils.MustParseUint(parts[1])),
					TransactionId: uint32(utils.MustParseUint(parts[2])),
				})
			}

			indexPath := config.GetPathToIndex(chain) + "finalized/" + curRange.String() + ".bin"
			if report, err := index.WriteChunk(chain, indexPath, appMap, len(appearances), bm.opts.Pin, bm.opts.Remote); err != nil {
				return err, false
			} else if report == nil {
				logger.Fatal("Should not happen, write chunk returned empty report")
			} else {
				report.Snapped = isSnap
				report.Report()
			}

			// There may be more than one chunk in the current stage, so continue until we're finished.
			appearances = []string{} // clear
		}
	}

	// There are almost certainly appearances left over that were not consolidated. Write these back to the stage.
	if len(appearances) > 0 {
		lastLine := appearances[len(appearances)-1]
		parts := strings.Split(lastLine, "\t")
		if len(parts) < 2 {
			// note that we don't remove the backup file
			return fmt.Errorf("cannot find block number at last line (%s)", lastLine), true

		} else {
			last := blocks[len(blocks)-1]
			meta, _ := bm.opts.Conn.GetMetaData(testMode) // the meta may have changed when we wrote the chunks
			newStageRng := base.FileRange{First: meta.Finalized + 1, Last: last}
			newStageFn := filepath.Join(config.GetPathToIndex(chain), "staging", fmt.Sprintf("%s.txt", newStageRng))
			err = file.LinesToAsciiFile(newStageFn, appearances)
			if err != nil {
				os.Remove(newStageFn) // remove it if it was created, note that we don't remove the backup file
				return err, true
			}
		}
	}

	// Let the user know what happened...
	stageFn, _ = file.LatestFileInFolder(stageFolder) // it may not exist...
	nAppsNow := int(file.FileSize(stageFn) / asciiAppearanceSize)
	bm.report(int(settings.Apps_per_chunk), nAppsThen, nAppsNow)

	// Commit the change by deleting the backup file (if the backup file exists
	// when this function exits, it will replace the original file - thereby
	// starting over).
	os.Remove(backupFn)

	return err, true
}

func (bm *BlazeManager) isListSequential(chain string, ripeFiles []os.DirEntry) error {
	prev := base.NotARange
	allowMissing := bm.opts.Settings.Allow_missing
	for _, file := range ripeFiles {
		fileRange := base.RangeFromFilename(file.Name())
		if prev != base.NotARange && prev != fileRange {
			if !prev.Preceeds(fileRange, !allowMissing) {
				return fmt.Errorf("ripe files are not sequential (%s ==> %s)", prev, fileRange)
			}
		}
		prev = fileRange
	}
	return nil
}
