package rpc

import (
	"bytes"
	"context"
	"fmt"
	"net/http"
	"time"
)

// PingRpc sends a ping request to the RPC provider, returns an error or nil on success.
func PingRpc(providerUrl string) error {
	// Set a timeout of, for example, 2 seconds for the request
	ctx, cancel := context.WithTimeout(context.Background(), 2000*time.Millisecond)
	defer cancel()

	jsonData := []byte(`{ "jsonrpc": "2.0", "method": "web3_clientVersion", "id": 6 }`)
	req, err := http.NewRequestWithContext(ctx, "POST", providerUrl, bytes.NewBuffer(jsonData))
	if err != nil {
		return err
	}

	req.Header.Set("Content-Type", "application/json")

	clientHTTP := &http.Client{}
	resp, err := clientHTTP.Do(req)
	if err != nil {
		// If the context timeout triggers, this error will reflect it
		return err
	}
	defer resp.Body.Close()

	if resp.StatusCode != http.StatusOK {
		return fmt.Errorf("unexpected status code: %d", resp.StatusCode)
	}

	return nil
}
