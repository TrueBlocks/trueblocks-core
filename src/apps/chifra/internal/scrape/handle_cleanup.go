package scrapePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleCleanup processes after the forever loop has exited. Currently does nothing but may be needed in the future.
func (opts *ScrapeOptions) HandleCleanup(progressThen *rpcClient.MetaData) (ok bool, err error) {
	if utils.DebuggingOn {
		logger.Log(logger.Info, "PostLoop")
	}
	return true, nil
}
