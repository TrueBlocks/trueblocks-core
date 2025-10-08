package types

import (
	"encoding/json"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// TestLazyLoadingValidation tests that our lazy loading system works correctly with base types
func TestLazyLoadingValidation(t *testing.T) {
	// Test Transaction with lazy loading
	wei1eth := base.NewWei(1000000000000000000) // 1 ETH
	tx := &Transaction{
		Hash:        base.HexToHash("0x1234567890abcdef1234567890abcdef1234567890abcdef1234567890abcdef"),
		BlockNumber: 18000000,
		Gas:         21000,
		GasPrice:    20000000000, // 20 gwei
		Value:       *wei1eth,
		Timestamp:   base.Timestamp(1699000000),
		Receipt: &Receipt{
			GasUsed: 21000, // Standard ETH transfer
		},
	}

	// Create ModelProps for lazy loading
	props := &ModelProps{
		Format: "json",
	}

	// Test EnsureCalcs
	err := tx.EnsureCalcs(props, []string{})
	if err != nil {
		t.Fatalf("EnsureCalcs failed: %v", err)
	}

	// Verify Calcs was populated
	if tx.Calcs == nil {
		t.Fatal("Calcs was not populated")
	}

	// Check that calculated fields have correct types
	if tx.Calcs.Date == "" {
		t.Error("Date field not populated")
	}

	if tx.Calcs.GasCost.IsZero() {
		t.Error("GasCost field not properly calculated")
	}

	if tx.Calcs.Ether == "" {
		t.Error("Ether field not populated")
	}

	// Test Statement with base.Wei fields
	wei1ethIn := base.NewWei(1000000000000000000)   // 1 ETH
	wei05ethOut := base.NewWei(500000000000000000)  // 0.5 ETH
	wei2ethBeg := base.NewWei(2000000000000000000)  // 2 ETH
	wei25ethEnd := base.NewWei(2500000000000000000) // 2.5 ETH
	stmt := &Statement{
		BlockNumber: 18000000,
		Asset:       base.HexToAddress("0x0000000000000000000000000000000000000000"), // ETH
		AmountIn:    *wei1ethIn,
		AmountOut:   *wei05ethOut,
		BegBal:      *wei2ethBeg,
		EndBal:      *wei25ethEnd,
		Timestamp:   base.Timestamp(1699000000),
	}

	// Test EnsureCalcs for Statement
	err = stmt.EnsureCalcs(props, []string{})
	if err != nil {
		t.Fatalf("Statement EnsureCalcs failed: %v", err)
	}

	// Verify Statement Calcs
	if stmt.Calcs == nil {
		t.Fatal("Statement Calcs was not populated")
	}

	if stmt.Calcs.Date == "" {
		t.Error("Statement Date field not populated")
	}

	if stmt.Calcs.TotalIn.IsZero() {
		t.Error("Statement TotalIn not properly calculated")
	}

	if stmt.Calcs.AmountNet.IsZero() {
		t.Error("Statement AmountNet not calculated")
	}

	// Test that multiple calls don't re-populate (lazy loading check)
	originalCalcs := tx.Calcs
	err = tx.EnsureCalcs(props, []string{})
	if err != nil {
		t.Fatalf("Second EnsureCalcs failed: %v", err)
	}

	if tx.Calcs != originalCalcs {
		t.Error("EnsureCalcs re-populated when it should have been cached")
	}

	// Test JSON marshaling of the entire object with Calcs
	jsonBytes, err := json.Marshal(tx)
	if err != nil {
		t.Fatalf("JSON marshaling failed: %v", err)
	}

	// Unmarshal back to verify round-trip
	var txCopy Transaction
	err = json.Unmarshal(jsonBytes, &txCopy)
	if err != nil {
		t.Fatalf("JSON unmarshaling failed: %v", err)
	}

	// Verify base types survived the round-trip
	if txCopy.Hash.Hex() != tx.Hash.Hex() {
		t.Errorf("Hash didn't survive JSON round-trip: got %s, want %s", txCopy.Hash.Hex(), tx.Hash.Hex())
	}

	if txCopy.Value.String() != tx.Value.String() {
		t.Errorf("Wei value didn't survive JSON round-trip: got %s, want %s", txCopy.Value.String(), tx.Value.String())
	}

	if txCopy.Timestamp != tx.Timestamp {
		t.Errorf("Timestamp didn't survive JSON round-trip: got %d, want %d", txCopy.Timestamp, tx.Timestamp)
	}

	t.Logf("✅ Lazy loading validation successful!")
	t.Logf("   - Transaction hash: %s", tx.Hash.Hex())
	t.Logf("   - Calculated date: %s", tx.Calcs.Date)
	t.Logf("   - Gas cost (Wei): %s", tx.Calcs.GasCost.String())
	t.Logf("   - Statement total in: %s", stmt.Calcs.TotalIn.String())
	t.Logf("   - JSON round-trip successful")
}
