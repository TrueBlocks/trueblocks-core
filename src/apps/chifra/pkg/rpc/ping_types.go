package rpc

import "encoding/json"

// PingResult contains detailed information about an RPC endpoint probe
type PingResult struct {
	URL           string `json:"url"`
	OK            bool   `json:"ok"`
	ChainID       string `json:"chainId,omitempty"`
	ChainName     string `json:"chainName,omitempty"`
	ExpectedChain string `json:"expectedChainId,omitempty"`
	ClientVersion string `json:"clientVersion,omitempty"`
	Mode          string `json:"mode"` // head or json
	Error         string `json:"error,omitempty"`
	StatusCode    int    `json:"statusCode,omitempty"`
	CheckedAt     int64  `json:"checkedAt"`
	LatencyMS     int64  `json:"latencyMs,omitempty"`
	FallbackJSON  bool   `json:"fallbackJson,omitempty"`
	Updated       bool   `json:"updated,omitempty"` // true if draft chainId was updated
}

// RpcRequest represents a JSON-RPC request structure
type RpcRequest struct {
	JSONRPC string      `json:"jsonrpc"`
	ID      int         `json:"id"`
	Method  string      `json:"method"`
	Params  interface{} `json:"params"`
}

// RpcResponse represents a JSON-RPC response structure
type RpcResponse struct {
	JSONRPC string          `json:"jsonrpc"`
	ID      int             `json:"id"`
	Result  json.RawMessage `json:"result"`
	Error   *RpcRespErr     `json:"error"`
}

// RpcRespErr represents a JSON-RPC error response
type RpcRespErr struct {
	Code    int    `json:"code"`
	Message string `json:"message"`
}
