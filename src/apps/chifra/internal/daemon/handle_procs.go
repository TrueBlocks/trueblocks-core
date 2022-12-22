package daemonPkg

// HandleMonitor starts and manages the monitor process
func (opts *DaemonOptions) HandleMonitor() error {
	// monitorOpts := monitorsPkg.GetMonitorsOptions([]string{}, &opts.Globals)
	// monitorOpts.Watch:   true,
	// monitorOpts.Sleep:   opts.Sleep,
	// err, _ := monitorOpts.MonitorsInternal()
	// return err
	return nil
}

// HandleScraper starts and manages the scraper process
func (opts *DaemonOptions) HandleScraper() error {
	// scrapeOpts := scrapePkg.GetScrapeOptions([]string{}, &opts.Globals)
	// scrapeOpts.Sleep = opts.Sleep
	// err, _ := scrapeOpts.ScrapeInternal()
	// return err
	return nil
}
