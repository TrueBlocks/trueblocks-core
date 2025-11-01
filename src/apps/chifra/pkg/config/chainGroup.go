// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/configtypes"

// GetChain returns the chain for a given chain
func GetChain(chain string) configtypes.ChainGroup {
	return GetRootConfig().Chains[chain]
}

// GetChains returns a list of all chains configured in the config file. Note, there is no "official"
// list. Users may add their own chains.
func GetChains() []configtypes.ChainGroup {
	chainArray := make([]configtypes.ChainGroup, 0, len(GetRootConfig().Chains))
	for _, v := range GetRootConfig().Chains {
		chainArray = append(chainArray, v)
	}
	return chainArray
}

// IsChainConfigured returns true if the chain is configured in the config file.
func IsChainConfigured(needle string) bool {
	chains := GetRootConfig().Chains
	if chains == nil {
		return false
	}
	_, found := chains[needle]
	return found
}
