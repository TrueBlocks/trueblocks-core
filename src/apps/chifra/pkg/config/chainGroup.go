// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import "strings"

type chainGroup struct {
	Chain          string         `toml:"chain,omitempty"`
	ChainId        string         `toml:"chainId"`
	IpfsGateway    string         `toml:"ipfsGateway,omitempty"`
	LocalExplorer  string         `toml:"localExplorer,omitempty"`
	RemoteExplorer string         `toml:"remoteExplorer,omitempty"`
	RpcProvider    string         `toml:"rpcProvider"`
	Symbol         string         `toml:"symbol"`
	Scrape         ScrapeSettings `toml:"scrape"`
}

func GetChains() []chainGroup {
	chainArray := make([]chainGroup, 0, len(GetRootConfig().Chains))
	for k, v := range GetRootConfig().Chains {
		v.Chain = k
		if len(v.IpfsGateway) == 0 {
			v.IpfsGateway = GetRootConfig().Settings.DefaultGateway
		}
		chainArray = append(chainArray, v)
	}
	return chainArray
}

// GetRpcProvider returns the RPC provider for a chain
func GetChain(chain string) chainGroup {
	return GetRootConfig().Chains[chain]
}

func IsChainConfigured(needle string) bool {
	return GetRootConfig().Chains[needle] != chainGroup{}
}

// GetChainId returns the expected chain id for a given chain
func GetChainId(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return ch.ChainId
}

// GetIpfsGateway returns the ipfs gateway for a chain
func GetIpfsGateway(chain string) string {
	def := GetRootConfig().Settings.DefaultGateway
	gateway := GetRootConfig().Chains[chain].IpfsGateway
	if len(gateway) == 0 {
		gateway = def
	}
	gateway = strings.Replace(gateway, "[{CHAIN}]", chain, -1)
	return cleanUrl(gateway)
}

func cleanUrl(url string) string {
	url = cleanPrefix(url)
	if !strings.HasSuffix(url, "/") {
		url += "/"
	}
	return url
}

func cleanPrefix(url string) string {
	if !strings.HasPrefix(url, "http") {
		url = "https://" + url
	}
	return url
}
