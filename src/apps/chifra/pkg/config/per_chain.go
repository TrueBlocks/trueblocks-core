// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"
)

// HasChains returns the expected chain id for a given chain
func HasChains() bool {
	return len(GetRootConfig().Chains) > 0
}

// GetChain
func GetChain(chain string) chainGroup {
	ch := GetRootConfig().Chains[chain]
	if ch.Chain == "" {
		ch.RpcProvider = cleanPrefix(ch.RpcProvider)
		ch.LocalExplorer = cleanUrl(ch.LocalExplorer)
		ch.RemoteExplorer = cleanUrl(ch.RemoteExplorer)
		if ch.IpfsGateway == "" {
			ch.IpfsGateway = GetRootConfig().Settings.DefaultGateway
		}
		ch.IpfsGateway = strings.Replace(ch.IpfsGateway, "[{CHAIN}]", chain, -1)
		ch.IpfsGateway = cleanUrl(ch.IpfsGateway)
		GetRootConfig().Chains[chain] = ch
	}
	return ch
}

// GetSymbol returns the expected chain id for a given chain
func GetSymbol(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return ch.Symbol
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
