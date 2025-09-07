//go:build integration
// +build integration

package rpc

import (
	"encoding/json"
	"testing"
)

func TestRpcPingResult_JSON(t *testing.T) {
	// Test that PingResult can be marshaled and unmarshaled correctly
	original := &PingResult{
		OK:            true,
		ChainID:       "0x1",
		ChainName:     "mainnet",
		ClientVersion: "Geth/v1.10.0",
		Error:         "",
		LatencyMS:     150,
	}

	// Marshal to JSON
	data, err := json.Marshal(original)
	if err != nil {
		t.Fatalf("Failed to marshal PingResult: %v", err)
	}

	// Unmarshal back
	var result PingResult
	err = json.Unmarshal(data, &result)
	if err != nil {
		t.Fatalf("Failed to unmarshal PingResult: %v", err)
	}

	// Compare fields
	if result.OK != original.OK {
		t.Errorf("OK field mismatch: got %v, want %v", result.OK, original.OK)
	}

	if result.ChainID != original.ChainID {
		t.Errorf("ChainID field mismatch: got %s, want %s", result.ChainID, original.ChainID)
	}

	if result.ChainName != original.ChainName {
		t.Errorf("ChainName field mismatch: got %s, want %s", result.ChainName, original.ChainName)
	}

	if result.ClientVersion != original.ClientVersion {
		t.Errorf("ClientVersion field mismatch: got %s, want %s", result.ClientVersion, original.ClientVersion)
	}

	if result.Error != original.Error {
		t.Errorf("Error field mismatch: got %s, want %s", result.Error, original.Error)
	}

	if result.LatencyMS != original.LatencyMS {
		t.Errorf("LatencyMS field mismatch: got %d, want %d", result.LatencyMS, original.LatencyMS)
	}
}

func TestRpcPingResult_ErrorCase(t *testing.T) {
	// Test error case serialization
	result := &PingResult{
		OK:        false,
		ChainID:   "",
		ChainName: "",
		Error:     "connection timeout",
		LatencyMS: 0,
	}

	data, err := json.Marshal(result)
	if err != nil {
		t.Fatalf("Failed to marshal error case: %v", err)
	}

	var unmarshaled PingResult
	err = json.Unmarshal(data, &unmarshaled)
	if err != nil {
		t.Fatalf("Failed to unmarshal error case: %v", err)
	}

	if unmarshaled.OK != false {
		t.Errorf("Expected OK=false for error case, got %v", unmarshaled.OK)
	}

	if unmarshaled.Error != "connection timeout" {
		t.Errorf("Expected error message, got %s", unmarshaled.Error)
	}
}
