package daemonPkg

import scrapePkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/scrape"

// HandleScraper starts and manages the scraper process
func (opts *DaemonOptions) HandleScraper() error {
	if len(opts.Scrape) == 0 {
		return nil
	}

	scrapeOpts := scrapePkg.GetScrapeOptions([]string{}, &opts.Globals)
	// scrapeOpts.Sleep = opts.Sleep
	err, _ := scrapeOpts.ScrapeInternal()
	return err
}
