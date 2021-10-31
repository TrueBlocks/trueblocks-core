package config

// Functions in this package read and cache configuration files,
// filling in default values

var cachedGlobal Global
var DefaultIndexPath = GetConfigPath("unchained")

func ReadGlobal() *Global {
	if cachedGlobal.Version.Current != "" {
		return &cachedGlobal
	}

	config := &Global{
		Settings: globalSettings{
			IndexPath: DefaultIndexPath,
		},
	}

	ReadTo(config, "trueBlocks")
	cachedGlobal = *config
	return config
}

var cachedBlockScrape BlockScrape
var DefaultIpfsGateway = "http://gateway.ipfs.io/ipfs/"

func ReadBlockScrape() *BlockScrape {
	if cachedBlockScrape.Dev.IpfsGateway != "" {
		return &cachedBlockScrape
	}

	config := &BlockScrape{
		Dev: blockScrapeDev{
			IpfsGateway: DefaultIpfsGateway,
		},
		Requires: blockScrapeRequires{
			Tracing:  true,
			Parity:   true,
			Balances: false,
		},
	}

	ReadTo(config, "blockScrape")
	cachedBlockScrape = *config
	return config
}
