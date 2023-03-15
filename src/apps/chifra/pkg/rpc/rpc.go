package rpc

import (
	"bytes"
	"encoding/json"
	"io"
	"net/http"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

// Params are used during calls to the RPC.
type Params []interface{}

// Payload is used during to make calls to the RPC.
type Payload struct {
	Method string `json:"method"`
	Params `json:"params"`
}

var rpcCounter uint32

// FromRpc Returns all traces for a given block.
func FromRpc(rpcProvider string, payload *Payload, ret interface{}) error {
	type rpcPayload struct {
		Jsonrpc string `json:"jsonrpc"`
		Method  string `json:"method"`
		Params  `json:"params"`
		ID      int `json:"id"`
	}

	payloadToSend := rpcPayload{
		Jsonrpc: "2.0",
		Method:  payload.Method,
		Params:  payload.Params,
		ID:      int(atomic.AddUint32(&rpcCounter, 1)),
	}

	plBytes, err := json.Marshal(payloadToSend)
	if err != nil {
		return err
	}
	// fmt.Println(string(plBytes))

	body := bytes.NewReader(plBytes)
	req, err := http.NewRequest("POST", rpcProvider, body)
	if err != nil {
		return err
	}

	req.Header.Set("Content-Type", "application/json")
	resp, err := http.DefaultClient.Do(req)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	theBytes, err := io.ReadAll(resp.Body)
	if err != nil {
		return err
	}

	return json.Unmarshal(theBytes, ret)
}

func Query[T any](chain string, method string, params Params) (*T, error) {
	var response struct {
		Result T `json:"result"`
	}

	payload := Payload{
		Method: method,
		Params: params,
	}

	err := FromRpc(config.GetRpcProvider(chain), &payload, &response)
	if err != nil {
		return nil, err
	}

	return &response.Result, err
}

func QuerySlice[T any](chain string, method string, params Params) ([]T, error) {
	var response struct {
		Result []T `json:"result"`
	}

	payload := Payload{
		Method: method,
		Params: params,
	}

	err := FromRpc(config.GetRpcProvider(chain), &payload, &response)
	if err != nil {
		return nil, err
	}

	return response.Result, err
}
