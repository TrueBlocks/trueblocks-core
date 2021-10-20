package config

type Global struct {
	IndexPath string
}

type BlockScrape struct {
	Tracing     bool
	IpfsGateway string `toml:"ipfs_gateway"`
}
