package scrapePkg

import (
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

func (opts *ScrapeOptions) Pause(progressThen *rpc.MetaData) {
	// we always pause at least a quarter of a second to allow the node to 'rest'
	time.Sleep(250 * time.Millisecond)
	isDefaultSleep := opts.Sleep >= 13 && opts.Sleep <= 14
	distanceFromHead := progressThen.Latest - progressThen.Staging
	shouldSleep := !isDefaultSleep || distanceFromHead <= (2*opts.Settings.Unripe_dist)
	if shouldSleep {
		sleep := opts.Sleep
		if sleep > 1 {
			logger.Info("Sleeping for", sleep, "seconds -", distanceFromHead, "away from head.")
		}
		halfSecs := (sleep * 2) - 1 // we already slept one quarter of a second
		for i := 0; i < int(halfSecs); i++ {
			time.Sleep(time.Duration(500) * time.Millisecond)
		}
	}

}
