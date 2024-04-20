package scrapePkg

import (
	"context"
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/notify"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

const asciiAppearanceSize = 59

// Consolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (bm *BlazeManager) Consolidate(blocks []base.Blknum) (error, bool) {
	var err error
	chain := bm.chain

	indexPath := config.PathToIndex(chain)

	backupFn := ""
	stageFn, _ := file.LatestFileInFolder(bm.StageFolder()) // it may not exist...
	if file.FileExists(stageFn) {
		backupFn, err = file.MakeBackup(filepath.Join(config.PathToCache(chain)+"tmp"), stageFn)
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
	}

	// After this point if we fail the backup file will replace the original file, so
	// we can safely remove these the stage file (and ripe files) and it will get replaced

	// Make sure the user can't interrupt us...
	ctx, cancel := context.WithCancel(context.Background())
	cleanOnQuit := func() {
		logger.Warn(sigintTrap.TrapMessage)
	}
	trapChannel := sigintTrap.Enable(ctx, cancel, cleanOnQuit)
	defer sigintTrap.Disable(trapChannel)

	// Some counters...
	nAppsFound := 0
	nAddrsFound := 0

	// Load the stage into the map...
	exists := file.FileExists(stageFn) // order matters
	appMap, chunkRange, nAppearances := bm.AsciiFileToAppearanceMap(stageFn)
	if !exists {
		// Brand new stage.
		chunkRange = base.FileRange{First: bm.meta.Finalized + 1, Last: blocks[0]}
	}

	// For each block...
	nChunks := 0
	for _, block := range blocks {
		fn := fmt.Sprintf("%09d.txt", block)
		if file.FileExists(filepath.Join(bm.UnripeFolder(), fn)) {
			continue // skip unripe files
		}

		ripeFn := filepath.Join(bm.RipeFolder(), fn)
		if !file.FileExists(ripeFn) {
			if bm.hasNoAddresses(block) {
				// this is okay -- see comment at the function
			} else {
				msg := fmt.Sprintf("ripe file not found for block %d", block) + spaces
				if !bm.AllowMissing() {
					_ = cleanEphemeralIndexFolders(chain)
					return errors.New(msg), true
				} else {
					logger.Warn(msg)
				}
			}
		}

		// Read in the ripe file, add it to the appMap and...
		thisMap, _, thisCount := bm.AsciiFileToAppearanceMap(ripeFn)
		nAppearances += thisCount
		nAppsFound += thisCount
		nAddrsFound += len(thisMap)
		for addr, apps := range thisMap {
			appMap[addr] = append(appMap[addr], apps...)
		}
		chunkRange.Last = block

		// ...decide if we need to consolidate...
		isSnap := bm.IsSnap(chunkRange.Last)            // Have we hit a snap point?
		isOvertop := nAppearances >= int(bm.PerChunk()) // Does this block overtop a chunk?
		if isSnap || isOvertop {
			// Make a chunk - i.e., consolidate
			chunkPath := indexPath + "finalized/" + chunkRange.String() + ".bin"
			publisher := base.ZeroAddr
			var chunk index.Chunk
			if report, err := chunk.Write(chain, publisher, chunkPath, appMap, nAppearances); err != nil {
				return err, false
			} else if report == nil {
				logger.Fatal("should not happen ==> write chunk returned empty report")
			} else {
				report.Snapped = isSnap
				report.FileSize = file.FileSize(chunkPath)
				report.Report()
			}
			if err = NotifyChunkWritten(chunk, chunkPath); err != nil {
				return err, true
			}

			// reset for next chunk
			bm.meta, _ = bm.opts.Conn.GetMetaData(bm.IsTestMode())
			appMap = make(map[string][]types.AppRecord, 0)
			chunkRange.First = chunkRange.Last + 1
			chunkRange.Last = chunkRange.Last + 1
			nAppearances = 0
			nChunks++
		}
	}

	var newRange base.FileRange
	if len(appMap) > 0 { // are there any appearances in this block range?
		newRange = base.FileRange{First: bm.meta.Finalized + 1, Last: 0}

		// We need an array because we're going to write it back to disc
		appearances := make([]string, 0, nAppearances)
		for addr, apps := range appMap {
			for _, app := range apps {
				record := fmt.Sprintf("%s\t%09d\t%05d", addr, app.BlockNumber, app.TransactionIndex)
				appearances = append(appearances, record)
				newRange.Last = utils.Max(newRange.Last, uint64(app.BlockNumber))
			}
		}

		// The stage needs to be sorted because the end user queries it and we want the search to be fast
		sort.Strings(appearances)

		stageFn = filepath.Join(bm.StageFolder(), fmt.Sprintf("%s.txt", newRange))
		if err := file.LinesToAsciiFile(stageFn, appearances); err != nil {
			os.Remove(stageFn)
			return err, true
		}
	}

	// Let the user know what happened...
	nAppsNow := int(file.FileSize(stageFn) / asciiAppearanceSize)
	bm.report(len(blocks), int(bm.PerChunk()), nChunks, nAppsNow, nAppsFound, nAddrsFound)

	if err := Notify(notify.Notification[string]{
		Msg:     notify.MessageStageUpdated,
		Meta:    bm.meta,
		Payload: newRange.String(),
	}); err != nil {
		return err, true
	}

	// Commit the change by deleting the backup file.
	os.Remove(backupFn)

	return nil, true
}

// AsciiFileToAppearanceMap reads the appearances from the stage file and returns them as a map
func (bm *BlazeManager) AsciiFileToAppearanceMap(fn string) (map[string][]types.AppRecord, base.FileRange, int) {
	appearances := file.AsciiFileToLines(fn)
	os.Remove(fn) // It's okay to remove this. If it fails, we'll just start over.

	appMap := make(map[string][]types.AppRecord, len(appearances))
	fileRange := base.FileRange{First: utils.NOPOS, Last: 0}

	if len(appearances) == 0 {
		return appMap, base.FileRange{First: 0, Last: 0}, 0
	}

	nAdded := 0
	for _, line := range appearances {
		parts := strings.Split(line, "\t")
		if len(parts) == 3 { // shouldn't be needed, but just in case...
			addr := strings.ToLower(parts[0])
			bn := utils.MustParseUint(strings.TrimLeft(parts[1], "0"))
			txid := utils.MustParseUint(strings.TrimLeft(parts[2], "0"))
			// See #3252
			if addr == base.SentinalAddr.Hex() && txid == types.MisconfigReward {
				continue
			}
			fileRange.First = utils.Min(fileRange.First, bn)
			fileRange.Last = utils.Max(fileRange.Last, bn)
			appMap[addr] = append(appMap[addr], types.AppRecord{
				BlockNumber:      uint32(bn),
				TransactionIndex: uint32(txid),
			})
			nAdded++
		}
	}

	return appMap, fileRange, nAdded
}

// hasNoAddresses returns true if (a) the miner is zero, (b) there are no transactions, uncles, or withdrawals.
// (It is truly a block with no addresses -- for example block 15537860 on mainnet.)
func (bm *BlazeManager) hasNoAddresses(bn base.Blknum) bool {
	if block, err := bm.opts.Conn.GetBlockHeaderByNumber(bn); err != nil {
		return false
	} else {
		return base.IsPrecompile(block.Miner.Hex()) &&
			len(block.Transactions) == 0 &&
			len(block.Uncles) == 0 &&
			len(block.Withdrawals) == 0
	}
}
