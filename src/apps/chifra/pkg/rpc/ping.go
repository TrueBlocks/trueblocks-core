package rpc

import (
	"bytes"
	"context"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"strings"
	"time"
)

// PingRpc sends a ping request to the RPC provider using eth_chainId,
// returns detailed probe result and simple error for backward compatibility.
func PingRpc(providerUrl string) (*PingResult, error) {
	// Set a timeout of 6 seconds (consistent with Khedra)
	ctx, cancel := context.WithTimeout(context.Background(), 6*time.Second)
	defer cancel()

	startTime := time.Now()
	result := &PingResult{
		URL:       providerUrl,
		Mode:      "json",
		CheckedAt: startTime.Unix(),
	}

	if providerUrl == "" {
		result.Error = "empty url"
		return result, fmt.Errorf("%s", result.Error)
	}

	payload := RpcRequest{
		JSONRPC: "2.0",
		ID:      1,
		Method:  "eth_chainId",
		Params:  []interface{}{},
	}

	jsonData, err := json.Marshal(payload)
	if err != nil {
		result.Error = fmt.Sprintf("marshal request: %v", err)
		return result, fmt.Errorf("%s", result.Error)
	}

	req, err := http.NewRequestWithContext(ctx, "POST", providerUrl, bytes.NewBuffer(jsonData))
	if err != nil {
		result.Error = fmt.Sprintf("create request: %v", err)
		return result, fmt.Errorf("%s", result.Error)
	}

	req.Header.Set("Content-Type", "application/json")

	client := &http.Client{}
	resp, err := client.Do(req)
	if err != nil {
		// If the context timeout triggers, this error will reflect it
		result.Error = fmt.Sprintf("request failed: %v", err)
		return result, fmt.Errorf("%s", result.Error)
	}
	defer resp.Body.Close()

	result.StatusCode = resp.StatusCode
	result.LatencyMS = time.Since(startTime).Milliseconds()

	if resp.StatusCode != http.StatusOK {
		result.Error = fmt.Sprintf("unexpected status code: %d", resp.StatusCode)
		return result, fmt.Errorf("%s", result.Error)
	}

	body, err := io.ReadAll(resp.Body)
	if err != nil {
		result.Error = fmt.Sprintf("read response: %v", err)
		return result, fmt.Errorf("%s", result.Error)
	}

	var jsonResp RpcResponse
	if err := json.Unmarshal(body, &jsonResp); err != nil {
		result.Error = fmt.Sprintf("parse response: %v", err)
		return result, fmt.Errorf("%s", result.Error)
	}

	if jsonResp.Error != nil {
		result.Error = fmt.Sprintf("rpc error %d: %s", jsonResp.Error.Code, jsonResp.Error.Message)
		return result, fmt.Errorf("%s", result.Error)
	}

	if jsonResp.Result == nil {
		result.Error = "empty result"
		return result, fmt.Errorf("%s", result.Error)
	}

	chainIdStr := strings.Trim(string(jsonResp.Result), "\"")
	result.ChainID = chainIdStr
	result.OK = true

	return result, nil
}
