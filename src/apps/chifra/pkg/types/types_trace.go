package types

import "github.com/ethereum/go-ethereum/common"

type SimpleTrace struct {
	BlockHash        common.Hash        `json:"blockHash"`
	BlockNumber      Blknum             `json:"blockNumber"`
	Timestamp        int64              `json:"timestamp"`
	Subtraces        uint64             `json:"subtraces"`
	TraceAddress     []string           `json:"traceAddress"`
	TransactionHash  common.Hash        `json:"transactionHash"`
	TransactionIndex Blknum             `json:"transactionIndex"`
	TraceType        string             `json:"traceType"`
	Error            string             `json:"error"`
	Action           *SimpleTraceAction `json:"action"`
	Result           *SimpleTraceResult `json:"result"`
	ArticulatedTrace *SimpleFunction    `json:"articulatedTrace"`
}
