package scrape

type ScrapeOptionsType struct {
	Action         string
	Sleep          float64
	Pin            bool
	Publish        bool
	Block_Cnt      uint64
	Block_Chan_Cnt uint64
	Addr_Chan_Cnt  uint64
}
