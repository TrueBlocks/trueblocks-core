package scrapePkg

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
)

// Report prints out a report of the progress of the scraper.
func (bm *BlazeManager) report(nBlocks, perChunk, nChunks, nAppsNow, nAppsFound, nAddrsFound int) {
	_ = nBlocks // linter
	nNeeded := perChunk - min(perChunk, nAppsNow)
	appsPerAddr := float64(nAppsFound) / float64(nAddrsFound)
	pctFull := float64(nAppsNow) / float64(perChunk)
	pctStr := fmt.Sprintf("%0.1f%%", pctFull*100)
	if len(pctStr) < 5 {
		pctStr = " " + pctStr
	}
	chunksStr := ""
	if nChunks > 0 {
		chunksStr = fmt.Sprintf(" created {%d} chunk(s)", nChunks)
	} else {
		chunksStr = fmt.Sprintf(" %s", bm.chain)
	}

	report := `%7.7s @#% 11d}: {% 8d}/{% 8d} ({%0.1f apps/addr}) stage {% 8d} need {% 8d} (@%5.5s})%s`
	msg := fmt.Sprintf(report,
		bm.chain,
		bm.EndBlock()-1,
		nAppsFound,
		nAddrsFound,
		appsPerAddr,
		nAppsNow,
		nNeeded,
		pctStr,
		chunksStr,
	)
	logger.Info(colors.Colored(msg))
}

// Pause goes to sleep for a period of time based on the settings.
func (opts *ScrapeOptions) pause(ctx context.Context, dist base.Blknum) {
	// sleepOrCancel is a helper that waits ms milliseconds unless the context has been cancelled.
	sleepOrCancel := func(ms time.Duration) (ok bool) {
		// We need to create a new timer explicitly, so we can cleanup the memory.
		// See the documentation of time.After
		timer := time.NewTimer(ms)
		select {
		case <-ctx.Done():
			// If the context has been cancelled, we need to drain the timer's channel
			// to make sure nothing "leaks"
			if !timer.Stop() {
				<-timer.C
			}
			return false
		case <-timer.C:
			// Since we have just read the channel's value, we can just call Stop
			timer.Stop()
			return true
		}
	}
	// we always pause at least a quarter of a second to allow the node to 'rest'
	if !sleepOrCancel(250 * time.Millisecond) {
		return
	}
	isDefaultSleep := opts.Sleep >= 13 && opts.Sleep <= 14
	shouldSleep := !isDefaultSleep || dist <= base.Blknum(2*config.GetScrape(opts.Globals.Chain).UnripeDist)
	if shouldSleep {
		sleep := opts.Sleep // this value may change elsewhere allow us to break out of sleeping????
		logger.Progress(sleep > 1, "Sleeping for", sleep, "seconds -", dist, "away from head.")
		halfSecs := (sleep * 2) - 1 // we already slept one quarter of a second
		for i := 0; i < int(halfSecs); i++ {
			if !sleepOrCancel(500 * time.Millisecond) {
				return
			}
		}
	}
}
