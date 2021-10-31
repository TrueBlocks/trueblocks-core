package config

// Global config

type globalVersion struct {
	Current string
}

type globalSettings struct {
	RpcProvider  string
	CachePath    string
	IndexPath    string
	EtherscanKey string `toml:"etherscan_key"`
}

type Global struct {
	Version  globalVersion
	Settings globalSettings
}

// BlockScrape config

type blockScrapeRequires struct {
	Tracing bool
	Parity  bool
	Balance bool
	Archive bool
}

type blockScrapeDev struct {
	IpfsGateway string `toml:"ipfs_gateway"`
}

type BlockScrape struct {
	Requires blockScrapeRequires
	Dev      blockScrapeDev
}
