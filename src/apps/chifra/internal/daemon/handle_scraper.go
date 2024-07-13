package daemonPkg

import (
	"context"

	scrapePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/scrape"
)

// HandleScraper starts and manages the scraper process
func (opts *DaemonOptions) HandleScraper() error {
	_ = context.TODO()
	if len(opts.Scrape) == 0 {
		return nil
	}

	scrapeOpts := scrapePkg.GetScrapeOptions([]string{}, &opts.Globals)
	err := scrapeOpts.ScrapeInternal()
	return err
}
