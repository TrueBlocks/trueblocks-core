package configtypes

type ChainGroup struct {
	Chain          string         `json:"chain" toml:"chain,omitempty"`
	ChainId        string         `json:"chainId" toml:"chainId"`
	IpfsGateway    string         `json:"ipfsGateway" toml:"ipfsGateway,omitempty"`
	KeyEndpoint    string         `json:"keyEndpoint" toml:"keyEndpoint,omitempty"`
	LocalExplorer  string         `json:"localExplorer" toml:"localExplorer,omitempty"`
	RemoteExplorer string         `json:"removeExplorer" toml:"remoteExplorer,omitempty"`
	RpcProvider    string         `json:"rpcProvider" toml:"rpcProvider"`
	Symbol         string         `json:"symbol" toml:"symbol"`
	Scrape         ScrapeSettings `json:"scrape" toml:"scrape"`
}
