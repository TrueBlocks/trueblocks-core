package scrapePkg

import (
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func (opts *ScrapeOptions) Pause(progressThen *rpcClient.MetaData) {
	//we always pause at least a quarter of a second to allow the node to 'rest'
	time.Sleep(250 * time.Millisecond)

	distanceFromHead := progressThen.Latest - progressThen.Staging
	isDefaultSleep := opts.Sleep == 14 || opts.Sleep == 13
	shouldSleep := !isDefaultSleep || distanceFromHead <= (2*opts.UnripeDist)

	if shouldSleep {
		sleep := opts.Sleep
		// TODO: BOGUS - THIS WORK IS INCOMPLETE
		// TODO: BOGUS - CHAIN SPECIFIC
		// if shouldSleep {
		// 	sleep = utils.Max(sleep, 13)
		// }
		if sleep > 1 {
			logger.Log(logger.Info, "Sleeping for", sleep, "seconds -", distanceFromHead, "away from head.")
		}
		halfSecs := (sleep * 2) - 1 // we already slept one half second
		for i := 0; i < int(halfSecs); i++ {
			time.Sleep(time.Duration(500) * time.Millisecond)
		}
	}

}
