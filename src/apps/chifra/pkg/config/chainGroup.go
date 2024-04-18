// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

type chainGroup struct {
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

// GetChain returns the chain for a given chain
func GetChain(chain string) chainGroup {
	return GetRootConfig().Chains[chain]
}

// GetChains returns a list of all chains configured in the config file. Note, there is no "official"
// list. Users may add their own chains.
func GetChains() []chainGroup {
	chainArray := make([]chainGroup, 0, len(GetRootConfig().Chains))
	for _, v := range GetRootConfig().Chains {
		chainArray = append(chainArray, v)
	}
	return chainArray
}

// IsChainConfigured returns true if the chain is configured in the config file.
func IsChainConfigured(needle string) bool {
	return GetRootConfig().Chains[needle] != chainGroup{}
}
