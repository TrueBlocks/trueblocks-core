package config

func ReadGlobal() *Global {
	config := &Global{
		IndexPath: getConfigPath("unchained"),
	}

	ReadTo(config, "trueBlocks")
	return config
}

var DefaultIpfsGateway = "http://gateway.ipfs.io/ipfs/"

func ReadBlockScrape() *BlockScrape {
	config := &BlockScrape{
		IpfsGateway: DefaultIpfsGateway,
	}

	ReadTo(config, "blockScrape")
	return config
}
