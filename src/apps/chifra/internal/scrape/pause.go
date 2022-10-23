package scrapePkg

import (
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

const (
	minimumSleep = time.Second / 4
)

func (opts *ScrapeOptions) Pause(progressThen *rpcClient.MetaData) <-chan time.Time {
	sleep := opts.computePause(progressThen)
	return time.After(sleep)
}

func (opts *ScrapeOptions) computePause(progressThen *rpcClient.MetaData) time.Duration {
	var sleep time.Duration

	isDefaultSleep := opts.Sleep >= 13 && opts.Sleep <= 14
	distanceFromHead := progressThen.Latest - progressThen.Staging
	shouldSleep := !isDefaultSleep || distanceFromHead <= (2*opts.Settings.Unripe_dist)
	if shouldSleep {
		if opts.Sleep > 1 {
			logger.Log(logger.Info, "Sleeping for", opts.Sleep, "seconds -", distanceFromHead, "away from head.")
		}
		sleep = time.Duration(opts.Sleep) * time.Second
	}

	// we always pause at least a quarter of a second to allow the node to 'rest'
	if sleep < minimumSleep {
		sleep = minimumSleep
	}

	return sleep
}
