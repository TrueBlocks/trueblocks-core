// Copyright 2025 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// Default block times for known chains (in seconds)
var defaultBlockTimes = map[string]float64{
	"mainnet":   13.0,  // Ethereum mainnet
	"optimism":  2.0,   // Optimism L2
	"polygon":   2.0,   // Polygon PoS
	"bsc":       3.0,   // BNB Smart Chain
	"gnosis":    5.0,   // Gnosis Chain (formerly xDai)
	"arbitrum":  0.25,  // Arbitrum One (very fast, ~250ms)
	"base":      2.0,   // Base L2
	"sepolia":   13.0,  // Sepolia testnet
	"goerli":    13.0,  // Goerli testnet
	"holesky":   12.0,  // Holesky testnet
	"avalanche": 2.0,   // Avalanche C-Chain
	"fantom":    1.0,   // Fantom Opera
	"celo":      5.0,   // Celo
	"aurora":    1.0,   // Aurora (NEAR EVM)
	"moonbeam":  12.0,  // Moonbeam
	"moonriver": 12.0,  // Moonriver
	"cronos":    6.0,   // Cronos
	"evmos":     6.0,   // Evmos
	"kava":      6.0,   // Kava EVM
	"metis":     4.0,   // Metis
	"boba":      0.5,   // Boba Network
	"zksync":    1.0,   // zkSync Era
	"linea":     12.0,  // Linea
	"scroll":    3.0,   // Scroll
	"manta":     2.0,   // Manta Pacific
}

// DefaultBlockTime is the fallback block time when chain-specific config is not available
const DefaultBlockTime = 13.0 // Ethereum mainnet as default

// GetBlockTime returns the block time for a given chain in seconds
func GetBlockTime(chain string) float64 {
	// First, check if configured in config file
	if chainConfig := GetChain(chain); chainConfig.BlockTime > 0 {
		return chainConfig.BlockTime
	}

	// Fall back to known defaults
	if blockTime, ok := defaultBlockTimes[chain]; ok {
		return blockTime
	}

	// Ultimate fallback with warning
	logger.Warn("No block time configured for chain, using default", "chain", chain, "default", DefaultBlockTime)
	return DefaultBlockTime
}