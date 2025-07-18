package configtypes

import "encoding/json"

type ChainGroup struct {
	Chain          string         `json:"chain" toml:"chain,omitempty"`
	ChainId        string         `json:"chainId" toml:"chainId"`
	IpfsGateway    string         `json:"ipfsGateway" toml:"ipfsGateway,omitempty"`
	KeyEndpoint    string         `json:"keyEndpoint" toml:"keyEndpoint,omitempty"`
	LocalExplorer  string         `json:"localExplorer" toml:"localExplorer,omitempty"`
	RemoteExplorer string         `json:"remoteExplorer" toml:"remoteExplorer,omitempty"`
	RpcProviderOld string         `json:"rpcProvider,omitempty" toml:"rpcProvider,omitempty"` // deprecated
	RpcProviders   []string       `json:"rpcProviders" toml:"rpcProviders,omitempty"`
	Symbol         string         `json:"symbol" toml:"symbol"`
	BlockTime      float64        `json:"blockTime" toml:"blockTime,omitempty"`
	Scrape         ScrapeSettings `json:"scrape" toml:"scrape"`
}

func (s *ChainGroup) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (cg ChainGroup) GetRpcProvider() string {
	if len(cg.RpcProviders) > 0 {
		return cg.RpcProviders[0]
	}
	return cg.RpcProviderOld
}
