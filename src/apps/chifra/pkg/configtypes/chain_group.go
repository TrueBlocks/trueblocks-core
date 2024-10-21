package configtypes

type ChainGroup struct {
	Chain          string         `toml:"chain,omitempty"`
	ChainId        string         `toml:"chainId"`
	IpfsGateway    string         `toml:"ipfsGateway,omitempty"`
	KeyEndpoint    string         `toml:"keyEndpoint,omitempty"`
	LocalExplorer  string         `toml:"localExplorer,omitempty"`
	RemoteExplorer string         `toml:"remoteExplorer,omitempty"`
	RpcProvider    string         `toml:"rpcProvider"`
	Symbol         string         `toml:"symbol"`
	Scrape         ScrapeSettings `toml:"scrape"`
}
