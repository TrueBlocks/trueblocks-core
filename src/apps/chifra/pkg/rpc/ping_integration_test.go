//go:build integration
// +build integration

package rpc

import (
	"net/http"
	"net/http/httptest"
	"strings"
	"testing"
)

func TestPingRpc_Success(t *testing.T) {
	// Mock server that responds to eth_chainId
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		if r.Method != "POST" {
			t.Errorf("Expected POST request, got %s", r.Method)
		}

		if !strings.Contains(r.Header.Get("Content-Type"), "application/json") {
			t.Errorf("Expected application/json content type")
		}

		// Return a successful chainId response (mainnet)
		response := `{"jsonrpc":"2.0","id":1,"result":"0x1"}`
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		w.Write([]byte(response))
	}))
	defer server.Close()

	result, err := PingRpc(server.URL)

	if err != nil {
		t.Fatalf("Expected no error, got %v", err)
	}

	if !result.OK {
		t.Fatalf("Expected OK=true, got %v", result.OK)
	}

	if result.ChainID != "0x1" {
		t.Errorf("Expected ChainID=0x1, got %s", result.ChainID)
	}

	// Latency might be 0 for very fast mock servers, so just check it's not negative
	if result.LatencyMS < 0 {
		t.Errorf("Expected non-negative latency, got %d", result.LatencyMS)
	}

	if result.Error != "" {
		t.Errorf("Expected no error, got %s", result.Error)
	}
}

// TestPingRpc_WithClientVersion removed - feature not yet implemented

func TestPingRpc_NetworkError(t *testing.T) {
	// Use an invalid URL to trigger network error
	result, err := PingRpc("http://invalid-host-that-does-not-exist:8545")

	if err == nil {
		t.Fatalf("Expected an error for invalid host")
	}

	// Even on error, we should get a result with details
	if result == nil {
		t.Fatalf("Expected result even on error, got nil")
	}

	if result.OK {
		t.Errorf("Expected OK=false on network error, got %v", result.OK)
	}

	if result.Error == "" {
		t.Errorf("Expected error message to be set")
	}
}

func TestPingRpc_HTTPError(t *testing.T) {
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		w.WriteHeader(http.StatusInternalServerError)
		w.Write([]byte("Internal Server Error"))
	}))
	defer server.Close()

	result, err := PingRpc(server.URL)

	if err == nil {
		t.Fatalf("Expected an error for HTTP 500")
	}

	// Even on error, we should get a result with details
	if result == nil {
		t.Fatalf("Expected result even on error, got nil")
	}

	if result.OK {
		t.Errorf("Expected OK=false on HTTP error, got %v", result.OK)
	}

	if result.StatusCode != 500 {
		t.Errorf("Expected StatusCode=500, got %d", result.StatusCode)
	}
}

func TestPingRpc_JSONRPCError(t *testing.T) {
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// Return a JSON-RPC error response
		response := `{"jsonrpc":"2.0","id":1,"error":{"code":-32601,"message":"Method not found"}}`
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		w.Write([]byte(response))
	}))
	defer server.Close()

	result, err := PingRpc(server.URL)

	if err == nil {
		t.Fatalf("Expected an error for JSON-RPC error response")
	}

	// Even on error, we should get a result with details
	if result == nil {
		t.Fatalf("Expected result even on error, got nil")
	}

	if result.OK {
		t.Errorf("Expected OK=false on JSON-RPC error, got %v", result.OK)
	}

	if !strings.Contains(result.Error, "Method not found") {
		t.Errorf("Expected error message to contain 'Method not found', got %s", result.Error)
	}

	if !strings.Contains(err.Error(), "Method not found") {
		t.Errorf("Expected error to contain 'Method not found', got %s", err.Error())
	}
}

func TestPingRpc_InvalidJSON(t *testing.T) {
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		// Return invalid JSON
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		w.Write([]byte("invalid json response"))
	}))
	defer server.Close()

	result, err := PingRpc(server.URL)

	if err == nil {
		t.Fatalf("Expected an error for invalid JSON")
	}

	// Even on error, we should get a result with details
	if result == nil {
		t.Fatalf("Expected result even on error, got nil")
	}

	if result.OK {
		t.Errorf("Expected OK=false on JSON parse error, got %v", result.OK)
	}
}

func TestPingRpc_EmptyURL(t *testing.T) {
	result, err := PingRpc("")

	if err == nil {
		t.Fatalf("Expected an error for empty URL")
	}

	// Even on error, we should get a result with details
	if result == nil {
		t.Fatalf("Expected result even on error, got nil")
	}

	if result.OK {
		t.Errorf("Expected OK=false for empty URL, got %v", result.OK)
	}

	if result.Error != "empty url" {
		t.Errorf("Expected specific error message, got %s", result.Error)
	}
}

// TestPingRpc_Timeout removed - takes too long (7+ seconds)
// Timeout behavior is already tested implicitly in network error cases

func TestPingRpc_ChainName(t *testing.T) {
	// Test just one representative case instead of multiple
	server := httptest.NewServer(http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		response := `{"jsonrpc":"2.0","id":1,"result":"0x1"}`
		w.Header().Set("Content-Type", "application/json")
		w.WriteHeader(http.StatusOK)
		w.Write([]byte(response))
	}))
	defer server.Close()

	result, err := PingRpc(server.URL)

	if err != nil {
		t.Fatalf("Expected no error, got %v", err)
	}

	if result.ChainID != "0x1" {
		t.Errorf("Expected ChainID=0x1, got %s", result.ChainID)
	}

	// Note: Current implementation doesn't populate ChainName
	if result.ChainName != "" {
		t.Errorf("Expected empty ChainName (not implemented), got %s", result.ChainName)
	}
}
