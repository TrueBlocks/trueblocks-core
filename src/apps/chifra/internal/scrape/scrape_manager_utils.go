package scrapePkg

import (
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Report prints out a report of the progress of the scraper.
func (bm *BlazeManager) report(perChunk, nAppsThen, nAppsNow int64) {
	need := perChunk - utils.Min(perChunk, nAppsNow)
	seen := nAppsNow
	if nAppsThen < nAppsNow {
		seen = nAppsNow - nAppsThen
	}
	pct := float64(nAppsNow) / float64(perChunk)
	pBlk := float64(seen) / float64(bm.BlockCount())
	height := bm.StartBlock() + bm.BlockCount() - 1

	const templ = `Block={%d} have {%d} appearances of {%d} ({%0.1f%%}). Need {%d} more. Added {%d} records ({%0.2f} apps/blk).`
	msg := strings.Replace(templ, "{", colors.Green, -1)
	msg = strings.Replace(msg, "}", colors.Off, -1)
	logger.Info(fmt.Sprintf(msg, height, nAppsNow, perChunk, pct*100, need, seen, pBlk))
}

// Pause goes to sleep for a period of time based on the settings.
func (opts *ScrapeOptions) pause(dist uint64) {
	// we always pause at least a quarter of a second to allow the node to 'rest'
	time.Sleep(250 * time.Millisecond)
	isDefaultSleep := opts.Sleep >= 13 && opts.Sleep <= 14
	shouldSleep := !isDefaultSleep || dist <= (2*opts.Settings.Unripe_dist)
	if shouldSleep {
		sleep := opts.Sleep // this value may change elsewhere allow us to break out of sleeping????
		logger.Progress(sleep > 1, "Sleeping for", sleep, "seconds -", dist, "away from head.")
		halfSecs := (sleep * 2) - 1 // we already slept one quarter of a second
		for i := 0; i < int(halfSecs); i++ {
			time.Sleep(time.Duration(500) * time.Millisecond)
		}
	}
}
