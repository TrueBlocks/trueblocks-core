package query

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"runtime"
	"strings"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/debug"
)

// Params are used during calls to the RPC.
type Params []interface{}

// Payload is used to make calls to the RPC.
type Payload struct {
	Method string `json:"method"`
	Params `json:"params"`
}

type rpcResponse[T any] struct {
	Result T             `json:"result"`
	Error  *eip1474Error `json:"error"`
}

type eip1474Error struct {
	Code    int    `json:"code"`
	Message string `json:"message"`
}

// Query returns a single result for given method and params.
func Query[T any](chain string, method string, params Params) (*T, error) {
	var response rpcResponse[T]

	payload := Payload{
		Method: method,
		Params: params,
	}

	provider := config.GetChain(chain).RpcProvider
	err := fromRpc(provider, &payload, &response)
	if err != nil {
		return nil, err
	}
	if response.Error != nil {
		return nil, fmt.Errorf("%d: %s", response.Error.Code, response.Error.Message)
	}

	return &response.Result, err
}

var rpcCounter uint32

type rpcPayload struct {
	Jsonrpc string `json:"jsonrpc"`
	Method  string `json:"method"`
	Params  `json:"params"`
	ID      int `json:"id"`
}

// fromRpc Returns all traces for a given block.
func fromRpc(rpcProvider string, payload *Payload, ret any) error {
	payloadToSend := rpcPayload{
		Jsonrpc: "2.0",
		Method:  payload.Method,
		Params:  payload.Params,
		ID:      int(atomic.AddUint32(&rpcCounter, 1)),
	}

	debug.DebugCurl(rpcDebug{
		payload: payloadToSend,
		url:     rpcProvider},
	)

	plBytes, err := json.Marshal(payloadToSend)
	if err != nil {
		return err
	}

	return sendRpcRequest(rpcProvider, plBytes, ret)
}

// BatchPayload is a wrapper around Payload type that allows us
// to associate a name (Key) to given request.
type BatchPayload struct {
	Key string
	*Payload
}

// QueryBatch batches requests to the node. Returned values are stored in map, with the same keys as defined
// in `batchPayload` (this way we don't have to operate on array indices)
func QueryBatch[T any](chain string, batchPayload []BatchPayload) (map[string]*T, error) {
	var response []rpcResponse[T]

	keys := make([]string, 0, len(batchPayload))
	payloads := make([]Payload, 0, len(batchPayload))
	for _, config := range batchPayload {
		keys = append(keys, config.Key)
		payloads = append(payloads, *config.Payload)
	}

	provider := config.GetChain(chain).RpcProvider
	err := fromRpcBatch(provider, payloads, &response)
	if err != nil {
		return nil, err
	}
	results := make(map[string]*T, len(batchPayload))
	for index, key := range keys {
		results[key] = &response[index].Result
	}
	return results, err
}

func fromRpcBatch(rpcProvider string, payloads []Payload, ret interface{}) error {
	payloadToSend := make([]rpcPayload, 0, len(payloads))

	for _, payload := range payloads {
		theLoad := rpcPayload{
			Jsonrpc: "2.0",
			Method:  payload.Method,
			Params:  payload.Params,
			ID:      int(atomic.AddUint32(&rpcCounter, 1)),
		}
		debug.DebugCurl(rpcDebug{
			payload: theLoad,
			url:     rpcProvider},
		)
		payloadToSend = append(payloadToSend, theLoad)
	}

	plBytes, err := json.Marshal(payloadToSend)
	if err != nil {
		return err
	}

	return sendRpcRequest(rpcProvider, plBytes, ret)
}

func sendRpcRequest(rpcProvider string, marshalled []byte, result any) error {
	body := bytes.NewReader(marshalled)
	resp, err := http.Post(rpcProvider, "application/json", body)
	if err != nil {
		return err
	}
	defer resp.Body.Close()

	theBytes, err := io.ReadAll(resp.Body)
	if err != nil {
		return err
	}

	return json.Unmarshal(theBytes, result)
}

func init() {
	// We need to increase MaxIdleConnsPerHost, otherwise chifra will keep trying to open too
	// many ports. It can lead to bind errors.
	// The default value is too low, so Go closes ports too fast. In the meantime, chifra tries
	// to get new ones and so it can run out of available ports.
	//
	// We change DefaultTransport as the whole codebase uses it.
	http.DefaultTransport.(*http.Transport).MaxIdleConnsPerHost = runtime.GOMAXPROCS(0) * 4
}

type rpcDebug struct {
	payload rpcPayload
	url     string
}

func (c rpcDebug) Url() string {
	return c.url
}

func (c rpcDebug) Body() string {
	return `curl -X POST [{headers}] --data '[{payload}]' [{url}]`
}

func (c rpcDebug) Headers() string {
	ret := `-H "Content-Type: application/json"`
	return ret
}

func (c rpcDebug) Method() string {
	return c.payload.Method
}

func (c rpcDebug) Payload() string {
	bytes, _ := json.MarshalIndent(c.payload, "", "")
	payloadStr := strings.Replace(string(bytes), "\n", " ", -1)
	return payloadStr
}
