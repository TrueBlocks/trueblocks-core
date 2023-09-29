// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"
	"sync"
)

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

var chainMutex sync.Mutex

// GetChain returns the chain for a given chain
func GetChain(chain string) chainGroup {
	chainMutex.Lock()
	defer chainMutex.Unlock()

	ch := GetRootConfig().Chains[chain]
	if ch.Chain == "" {
		ch.Chain = chain
		ch.LocalExplorer = cleanUrl(ch.LocalExplorer)
		ch.RemoteExplorer = cleanUrl(ch.RemoteExplorer)
		ch.RpcProvider = cleanPrefix(ch.RpcProvider)
		if len(ch.IpfsGateway) == 0 {
			ch.IpfsGateway = GetSettings().DefaultGateway
		}
		ch.IpfsGateway = strings.Replace(ch.IpfsGateway, "[{CHAIN}]", chain, -1)
		// cache it...
		GetRootConfig().Chains[chain] = ch
	}
	return ch
}

// GetChains returns a list of all chains configured in the config file. Note, there is no "official"
// list. Users may add their own chains.
func GetChains() []chainGroup {
	chainArray := make([]chainGroup, 0, len(GetRootConfig().Chains))
	for k, v := range GetRootConfig().Chains {
		v.Chain = k
		if len(v.IpfsGateway) == 0 {
			v.IpfsGateway = GetSettings().DefaultGateway
		}
		chainArray = append(chainArray, v)
	}
	return chainArray
}

// IsChainConfigured returns true if the chain is configured in the config file.
func IsChainConfigured(needle string) bool {
	return GetRootConfig().Chains[needle] != chainGroup{}
}

// cleanUrl cleans the user's input trying to make a valid Url.
func cleanUrl(url string) string {
	url = cleanPrefix(url)
	if !strings.HasSuffix(url, "/") {
		url += "/"
	}
	return url
}

// cleanPrefix cleans the user's input trying to make a valid Url.
func cleanPrefix(url string) string {
	if !strings.HasPrefix(url, "http") {
		url = "https://" + url
	}
	return url
}
