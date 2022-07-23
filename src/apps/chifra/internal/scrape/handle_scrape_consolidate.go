package scrapePkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleScrapeConsolidate calls into the block scraper to (a) call Blaze and (b) consolidate if applicable
func (opts *ScrapeOptions) HandleScrapeConsolidate(progressThen *rpcClient.MetaData) (ok bool, err error) {
	if utils.DebuggingOn {
		prevStage, _ := file.LatestFileInFolder(config.GetPathToIndex(opts.Globals.Chain) + "staging/")
		logger.Log(logger.Info, "prevStage:", prevStage)
		logger.Log(logger.Info, "startBlock:", opts.StartBlock)
	}

	if !opts.verifyRipeFiles() {
		return true, nil
	}

	// ss := NewScraperState()
	err = opts.Globals.PassItOn("blockScrape", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStrings(progressThen))
	return true, err
}
