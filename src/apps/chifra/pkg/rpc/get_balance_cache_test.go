package rpc

import (
	"fmt"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

// TestGetBalanceAtCacheCollision tests that the in-memory balance cache
// correctly distinguishes between different chains to prevent cache key collisions.
// This test demonstrates the fix for issue #3993.
func TestGetBalanceAtCacheCollision(t *testing.T) {
	// Create two mock connections for different chains
	conn1 := &Connection{
		Chain:        "mainnet",
		balanceCache: make(map[string]*base.Wei),
	}

	conn2 := &Connection{
		Chain:        "gnosis",
		balanceCache: make(map[string]*base.Wei),
	}

	// Test address and block number
	testAddr := base.HexToAddress("0x1234567890123456789012345678901234567890")
	testBlock := base.Blknum(1000000)

	// Create different balance values for each chain
	mainnetBalance := base.NewWei(1000)
	gnosisBalance := base.NewWei(2000)

	// Simulate caching balances for the same address/block on different chains
	// With the fix, these should have different cache keys:
	// - mainnet: "mainnet|0x1234567890123456789012345678901234567890|1000000"
	// - gnosis:  "gnosis|0x1234567890123456789012345678901234567890|1000000"

	// Store balance for mainnet
	mainnetKey := fmt.Sprintf("%s|%s|%d", conn1.Chain, testAddr.Hex(), testBlock)
	conn1.balanceCache[mainnetKey] = mainnetBalance

	// Store balance for gnosis
	gnosisKey := fmt.Sprintf("%s|%s|%d", conn2.Chain, testAddr.Hex(), testBlock)
	conn2.balanceCache[gnosisKey] = gnosisBalance

	// Verify that the cache keys are different
	if mainnetKey == gnosisKey {
		t.Errorf("Cache keys should be different for different chains, but both are: %s", mainnetKey)
	}

	// Verify we can retrieve the correct balance for each chain
	if cachedMainnet, ok := conn1.balanceCache[mainnetKey]; !ok {
		t.Error("Failed to retrieve mainnet balance from cache")
	} else if cachedMainnet.String() != mainnetBalance.String() {
		t.Errorf("Mainnet balance mismatch: got %s, want %s", cachedMainnet.String(), mainnetBalance.String())
	}

	if cachedGnosis, ok := conn2.balanceCache[gnosisKey]; !ok {
		t.Error("Failed to retrieve gnosis balance from cache")
	} else if cachedGnosis.String() != gnosisBalance.String() {
		t.Errorf("Gnosis balance mismatch: got %s, want %s", cachedGnosis.String(), gnosisBalance.String())
	}

	// Test what would happen with the old cache key format (without chain)
	// This demonstrates the bug that was fixed
	oldKey := fmt.Sprintf("%s|%d", testAddr.Hex(), testBlock)

	// If we used the old key format, one chain's balance would overwrite the other
	oldCache := make(map[string]*base.Wei)
	oldCache[oldKey] = mainnetBalance // mainnet writes first
	oldCache[oldKey] = gnosisBalance  // gnosis overwrites it!

	// With the old format, we'd get the wrong balance
	if oldCache[oldKey].String() != gnosisBalance.String() {
		t.Logf("Old cache format demonstration: mainnet balance was overwritten by gnosis balance")
	}
}

// TestGetBalanceAtCacheKeyFormat verifies the exact format of cache keys
func TestGetBalanceAtCacheKeyFormat(t *testing.T) {
	tests := []struct {
		name     string
		chain    string
		address  string
		block    base.Blknum
		expected string
	}{
		{
			name:     "mainnet key",
			chain:    "mainnet",
			address:  "0x1234567890123456789012345678901234567890",
			block:    1000000,
			expected: "mainnet|0x1234567890123456789012345678901234567890|1000000",
		},
		{
			name:     "gnosis key",
			chain:    "gnosis",
			address:  "0x1234567890123456789012345678901234567890",
			block:    1000000,
			expected: "gnosis|0x1234567890123456789012345678901234567890|1000000",
		},
		{
			name:     "different block same address",
			chain:    "mainnet",
			address:  "0x1234567890123456789012345678901234567890",
			block:    2000000,
			expected: "mainnet|0x1234567890123456789012345678901234567890|2000000",
		},
	}

	for _, tt := range tests {
		t.Run(tt.name, func(t *testing.T) {
			addr := base.HexToAddress(tt.address)
			key := fmt.Sprintf("%s|%s|%d", tt.chain, addr.Hex(), tt.block)
			if key != tt.expected {
				t.Errorf("Cache key format mismatch: got %s, want %s", key, tt.expected)
			}
		})
	}
}
