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

// GetChainId returns the expected chain id for a given chain
func GetChainId(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return ch.ChainId
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

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return cleanPrefix(ch.RpcProvider)
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

// ToBloomPath returns a path pointing to the bloom filter given either a path to itself or its associated index data
func ToBloomPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".bloom") {
		return pathIn
	}
	ret := strings.Replace(pathIn, ".bin", ".bloom", -1)
	ret = strings.Replace(ret, ".txt", ".bloom", -1)
	ret = strings.Replace(ret, "/finalized/", "/blooms/", -1)
	ret = strings.Replace(ret, "/staging/", "/blooms/", -1)
	return ret
}

// ToIndexPath returns a path pointing to the bloom filter
func ToIndexPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".bin") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bloom", ".bin", -1)
	ret = strings.Replace(ret, ".txt", ".bin", -1)
	ret = strings.Replace(ret, "/blooms/", "/finalized/", -1)
	ret = strings.Replace(ret, "/staging/", "/finalized/", -1)
	return ret
}

// ToStagingPath returns a path pointing to the staging folder given either a neighboring path
func ToStagingPath(pathIn string) string {
	if strings.HasSuffix(pathIn, ".txt") {
		return pathIn
	}

	ret := strings.Replace(pathIn, ".bin", ".txt", -1)
	ret = strings.Replace(ret, ".bloom", ".txt", -1)
	ret = strings.Replace(ret, "/finalized/", "/staging/", -1)
	ret = strings.Replace(ret, "/blooms/", "/staging/", -1)
	return ret
}
