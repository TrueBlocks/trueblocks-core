package scrapePkg

import (
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// BlazeManager manages the scraper by keeping track of the progress of the scrape and
// maintaining the timestamp array and processed map. The processed map helps us know
// if every block was visited or not.
type BlazeManager struct {
	chain        string
	ripeBlock    base.Blknum
	timestamps   []tslib.TimestampRecord
	processedMap map[base.Blknum]bool
	nProcessed   uint64
	opts         *ScrapeOptions
	meta         *rpc.MetaData
}

// StartBlock returns the start block for the current pass of the scraper.
func (bm *BlazeManager) StartBlock() base.Blknum {
	return bm.opts.StartBlock
}

// BlockCount returns the number of blocks to process for this pass of the scraper.
func (bm *BlazeManager) BlockCount() base.Blknum {
	return bm.opts.BlockCnt
}

// Report prints out a report of the progress of the scraper.
func (bm *BlazeManager) report(nAppsThen, nAppsNow int) {
	settings := bm.opts.Settings

	msg := "Block={%d} have {%d} appearances of {%d} ({%0.1f%%}). Need {%d} more. Added {%d} records ({%0.2f} apps/blk)."
	need := settings.Apps_per_chunk - utils.Min(settings.Apps_per_chunk, uint64(nAppsNow))
	seen := nAppsNow
	if nAppsThen < nAppsNow {
		seen = nAppsNow - nAppsThen
	}
	pct := float64(nAppsNow) / float64(settings.Apps_per_chunk)
	pBlk := float64(seen) / float64(bm.BlockCount())
	height := bm.StartBlock() + bm.BlockCount() - 1
	msg = strings.Replace(msg, "{", colors.Green, -1)
	msg = strings.Replace(msg, "}", colors.Off, -1)
	logger.Info(fmt.Sprintf(msg, height, nAppsNow, settings.Apps_per_chunk, pct*100, need, seen, pBlk))
}

// Pause goes to sleep for a period of time based on the settings.
func (bm *BlazeManager) pause() {
	// we always pause at least a quarter of a second to allow the node to 'rest'
	time.Sleep(250 * time.Millisecond)
	isDefaultSleep := bm.opts.Sleep >= 13 && bm.opts.Sleep <= 14
	distanceFromHead := bm.meta.Latest - bm.meta.Staging
	shouldSleep := !isDefaultSleep || distanceFromHead <= (2*bm.opts.Settings.Unripe_dist)
	if shouldSleep {
		sleep := bm.opts.Sleep // this value may change elsewhere allow us to break out of sleeping????
		if sleep > 1 {
			logger.Info("Sleeping for", sleep, "seconds -", distanceFromHead, "away from head.")
		}
		halfSecs := (sleep * 2) - 1 // we already slept one quarter of a second
		for i := 0; i < int(halfSecs); i++ {
			time.Sleep(time.Duration(500) * time.Millisecond)
		}
	}
}

// scrapedData combines the extracted block data, trace data, and log data into a
// structure that is passed through to the AddressChannel for further processing.
type scrapedData struct {
	bn       base.Blknum
	traces   []types.SimpleTrace
	receipts []types.SimpleReceipt
}
