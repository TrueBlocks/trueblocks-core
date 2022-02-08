// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"
)

// GetChainId returns the expected chain id for a given chain
func GetChainId(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return ch.ChainId
}

func cleanUrl(url string) string {
	if !strings.HasPrefix(url, "http") {
		url = "https://" + url
	}
	if !strings.HasSuffix(url, "/") {
		url += "/"
	}
	return url
}

// GetLocalExplorer returns the expected chain id for a given chain
func GetLocalExplorer(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return cleanUrl(ch.LocalExplorer)
}

// GetRemoteExplorer returns the expected chain id for a given chain
func GetRemoteExplorer(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return cleanUrl(ch.RemoteExplorer)
}

// GetPinGateway returns the pin gateway for a chain
func GetPinGateway(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return cleanUrl(ch.PinGateway)
}

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return cleanUrl(ch.RpcProvider)
}

// GetApiProvider returns the RPC provider for a chain
func GetApiProvider(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return cleanUrl(ch.ApiProvider)
}

// GetSymbol returns the expected chain id for a given chain
func GetSymbol(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return ch.Symbol
}
