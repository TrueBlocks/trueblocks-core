package scrapePkg

import (
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func colored(s string) string {
	s = strings.Replace(s, "{", colors.Green, -1)
	s = strings.Replace(s, "}", colors.Off, -1)
	return s
}

// Report prints out a report of the progress of the scraper.
func (bm *BlazeManager) report(nBlocks, perChunk, nAppsNow, nAppsFound, nAddrsFound int) {
	nNeeded := perChunk - utils.Min(perChunk, nAppsNow)
	pctFull := float64(nAppsNow) / float64(perChunk)
	appsPerBlock := float64(nAppsFound) / float64(bm.BlockCount())

	msg := fmt.Sprintf(`At block {%d}, found {%d} apps for {%d} addrs in {%d} blocks ({%0.2f} apps/blk). Have {%d} of {%d} {%0.1f%%}. Need {%d} more.`,
		bm.EndBlock(),
		nAppsFound,
		nAddrsFound,
		nBlocks,
		appsPerBlock,
		nAppsNow,
		perChunk,
		pctFull*100,
		nNeeded,
	)
	logger.Info(colored(msg))
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
