// Copyright 2025 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"testing"
)

func TestGetBlockTime(t *testing.T) {
	tests := []struct {
		name     string
		chain    string
		expected float64
	}{
		// Known chains with default block times
		{
			name:     "Ethereum mainnet",
			chain:    "mainnet",
			expected: 13.0,
		},
		{
			name:     "Optimism",
			chain:    "optimism",
			expected: 2.0,
		},
		{
			name:     "Polygon",
			chain:    "polygon",
			expected: 2.0,
		},
		{
			name:     "BSC",
			chain:    "bsc",
			expected: 3.0,
		},
		{
			name:     "Gnosis",
			chain:    "gnosis",
			expected: 5.0,
		},
		{
			name:     "Arbitrum",
			chain:    "arbitrum",
			expected: 0.25,
		},
		{
			name:     "Unknown chain falls back to default",
			chain:    "unknown_chain",
			expected: DefaultBlockTime,
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			result := GetBlockTime(tt.chain)
			if result != tt.expected {
				t.Errorf("GetBlockTime(%s) = %f, expected %f", tt.chain, result, tt.expected)
			}
		})
	}
}

func TestBlockTimeCalculations(t *testing.T) {
	// Test the calculations that were previously hardcoded
	tests := []struct {
		name          string
		chain         string
		secondsDiff   float64
		expectedBlks  uint32
	}{
		{
			name:          "Ethereum 1 hour",
			chain:         "mainnet",
			secondsDiff:   3600,
			expectedBlks:  277, // 3600 / 13.0 = 276.92
		},
		{
			name:          "Optimism 1 hour",
			chain:         "optimism", 
			secondsDiff:   3600,
			expectedBlks:  1800, // 3600 / 2.0 = 1800
		},
		{
			name:          "Arbitrum 1 minute",
			chain:         "arbitrum",
			secondsDiff:   60,
			expectedBlks:  240, // 60 / 0.25 = 240
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			blockTime := GetBlockTime(tt.chain)
			result := uint32(tt.secondsDiff / blockTime)
			if result != tt.expectedBlks {
				t.Errorf("Block calculation for %s: got %d blocks, expected %d", tt.chain, result, tt.expectedBlks)
			}
		})
	}
}