package config

var DefaultIpfsGateway = "http://gateway.ipfs.io/ipfs/"

// Functions in this package read and cache configuration files,
// filling in default values

var cachedGlobal Global

func ReadGlobal() *Global {
	if cachedGlobal.Version.Current != "" {
		return &cachedGlobal
	}

	config := &Global{
		Settings: globalSettings{
			IndexPath: GetConfigPath("unchained"),
		},
	}

	ReadTo(config, "trueBlocks")
	cachedGlobal = *config
	return config
}

var cachedBlockScrape BlockScrape

func ReadBlockScrape() *BlockScrape {
	if cachedBlockScrape.Dev.IpfsGateway != "" {
		return &cachedBlockScrape
	}

	config := &BlockScrape{
		Dev: blockScrapeDev{
			IpfsGateway: DefaultIpfsGateway,
		},
	}

	ReadTo(config, "blockScrape")
	cachedBlockScrape = *config
	return config
}
