package scrapePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// Y_5_postLoop processes after the loop has exited. Currently does nothing but may be needed in the future
func (opts *ScrapeOptions) Y_5_postLoop(progressThen *rpcClient.MetaData) (bool, error) {
	// TODO: BOGUS - TESTING SCRAPING
	if utils.OnOff {
		logger.Log(logger.Info, "PostLoop")
	}
	return true, nil
}
