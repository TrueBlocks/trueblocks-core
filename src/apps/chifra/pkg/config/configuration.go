package config

var DefaultIpfsGateway = "http://gateway.ipfs.io/ipfs/"

// Functions in this package read configuration files,
// filling in default values

func ReadGlobal() *Global {
	config := &Global{
		Settings: globalSettings{
			IndexPath: getConfigPath("unchained"),
		},
	}

	ReadTo(config, "trueBlocks")
	return config
}

func ReadBlockScrape() *BlockScrape {
	config := &BlockScrape{
		Dev: blockScrapeDev{
			IpfsGateway: DefaultIpfsGateway,
		},
	}

	ReadTo(config, "blockScrape")
	return config
}
