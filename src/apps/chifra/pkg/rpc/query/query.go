package query

import (
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"runtime"
	"strings"
	"sync/atomic"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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

var devDebug = false
var devDebugMethod = ""

func init() {
	// We need to increase MaxIdleConnsPerHost, otherwise chifra will keep trying to open too
	// many ports. It can lead to bind errors.
	// The default value is too low, so Go closes ports too fast. In the meantime, chifra tries
	// to get new ones and so it can run out of available ports.
	//
	// We change DefaultTransport as the whole codebase uses it.
	http.DefaultTransport.(*http.Transport).MaxIdleConnsPerHost = runtime.GOMAXPROCS(0) * 4

	devDebugMethod = os.Getenv("TB_DEBUG_CURL")
	devDebug = len(devDebugMethod) > 0
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

	debugCurl(payloadToSend, rpcProvider)

	plBytes, err := json.Marshal(payloadToSend)
	if err != nil {
		return err
	}

	return sendRpcRequest(rpcProvider, plBytes, ret)
}

// QuerySlice returns a slice of results for given method and params.
func QuerySlice[T any](chain string, method string, params Params) ([]T, error) {
	var response rpcResponse[[]T]

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

	return response.Result, err
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
		debugCurl(theLoad, rpcProvider)
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

func debugCurl(payload rpcPayload, rpcProvider string) {
	if !devDebug {
		return
	}

	if devDebugMethod != "file" && devDebugMethod != "true" && devDebugMethod != "testing" && payload.Method != devDebugMethod {
		return
	}

	var bytes []byte
	var payloadStr string
	if devDebugMethod == "testing" {
		rpcProvider = "--rpc-provider--"
		parts := strings.Split(strings.Replace(payloadStr, "]", "[", -1), "[")
		parts[1] = "[ --params-- ]"
		payloadStr = strings.Join(parts, "")
	} else {
		bytes, _ = json.MarshalIndent(payload, "", "")
		payloadStr = strings.Replace(string(bytes), "\n", " ", -1)
	}

	var curlCmd = `curl -X POST -H "Content-Type: application/json" --data '[{payload}]' [{rpcProvider}]`
	curlCmd = strings.Replace(curlCmd, "[{payload}]", payloadStr, -1)
	curlCmd = strings.Replace(curlCmd, "[{rpcProvider}]", rpcProvider, -1)
	if devDebugMethod == "file" {
		_ = file.AppendToAsciiFile("./curl.log", curlCmd+"\n")
	} else {
		logger.ToggleDecoration()
		logger.Info(curlCmd)
		logger.ToggleDecoration()
	}
}

func CloseDebugger() {
	if !devDebug {
		return
	}
	logger.Info("Closing curl debugger")
}
