// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common"
)

// TODO: CompressedTrace is not part of the raw trace data
// EXISTING_CODE

type RawTrace struct {
	Error            string         `json:"error"`
	BlockHash        string         `json:"blockHash"`
	BlockNumber      uint64         `json:"blockNumber"`
	Timestamp        int64          `json:"timestamp"`
	TransactionHash  string         `json:"transactionHash"`
	TransactionIndex uint64         `json:"transactionIndex"`
	TraceAddress     []uint64       `json:"traceAddress"`
	Subtraces        uint64         `json:"subtraces"`
	Type             string         `json:"type"`
	Action           RawTraceAction `json:"action"`
	Result           RawTraceResult `json:"result"`
	CompressedTrace  string         `json:"compressedTrace"`
}

type SimpleTrace struct {
	Error            string             `json:"error,omitempty"`
	BlockHash        common.Hash        `json:"blockHash"`
	BlockNumber      uint64             `json:"blockNumber"`
	Timestamp        int64              `json:"timestamp"`
	TransactionHash  common.Hash        `json:"transactionHash"`
	TransactionIndex uint64             `json:"transactionIndex"`
	TraceAddress     []uint64           `json:"traceAddress"`
	Subtraces        uint64             `json:"subtraces"`
	Type             string             `json:"type,omitempty"`
	Action           *SimpleTraceAction `json:"action"`
	Result           *SimpleTraceResult `json:"result"`
	ArticulatedTrace *SimpleFunction    `json:"articulatedTrace"`
	CompressedTrace  string             `json:"compressedTrace,omitempty"`
	raw              *RawTrace
}

func (s *SimpleTrace) Raw() *RawTrace {
	return s.raw
}

func (s *SimpleTrace) SetRaw(raw *RawTrace) {
	s.raw = raw
}

func (s *SimpleTrace) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"timestamp":        s.Timestamp,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
		"subtraces":        s.Subtraces,
		"action":           s.Action,
		"result":           s.Result,
		"articulatedTrace": s.ArticulatedTrace,
	}

	order := []string{
		"blockHash",
		"blockNumber",
		"timestamp",
		"transactionHash",
		"transactionIndex",
		"subtraces",
		"action",
		"result",
		"articulatedTrace",
	}

	// EXISTING_CODE
	// TODO: BOGUS - HANDLE OMITEMPTY HERE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// GetTraceCountByBlockNumber returns the number of traces in a block
func GetTraceCountByBlockNumber(chain string, bn uint64) (uint64, error) {
	if traces, err := GetTracesByBlockNumber(chain, bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}

// GetTracesByBlockNumber returns a slice of traces in the given block
func GetTracesByBlockNumber(chain string, bn uint64) ([]SimpleTrace, error) {
	method := "trace_block"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if rawTraces, err := rpc.QuerySlice[RawTrace](chain, method, params); err != nil {
		return []SimpleTrace{}, err
	} else {
		var ret []SimpleTrace
		for _, rawTrace := range rawTraces {
			ret = append(ret, SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        common.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				Timestamp:        rawTrace.Timestamp,
				TransactionHash:  common.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				Type:             rawTrace.Type,
				// Action:           rawTrace.Action,
				// Result:           rawTrace.Result,
				CompressedTrace: rawTrace.CompressedTrace,
			})
		}
		return ret, nil
	}
}

// GetTracesCountByTransactionId returns the number of traces in a given transaction
func GetTracesCountByTransactionId(chain string, bn, txid uint64) (uint64, error) {
	traces, err := GetTracesByTransactionId(chain, bn, txid)
	if err != nil {
		return 0, err
	}
	return uint64(len(traces)), nil
}

// GetTracesByTransactionId returns a slice of traces in a given transaction
func GetTracesByTransactionId(chain string, bn, txid uint64) ([]SimpleTrace, error) {
	var ret []SimpleTrace

	// txHash, err := rpcClient.TxHashFromNumberAndId(chain, arg, validate.IsBlockHash)
	// if err != nil {
	return ret, err
	// }

	// method := "trace_transaction"
	// params := rpc.Params{fmt.Sprintf("0x%s", txHash)}

	// if rawTraces, err := rpc.QuerySlice[RawTrace](chain, method, params); err != nil {
	// 	return ret, err

	// } else {
	// 	for _, rawTrace := range rawTraces {
	// 		ret = append(ret, SimpleTrace{
	// 			Error:            rawTrace.Error,
	// 			BlockHash:        common.HexToHash(rawTrace.BlockHash),
	// 			BlockNumber:      rawTrace.BlockNumber,
	// 			Timestamp:        rawTrace.Timestamp,
	// 			TransactionHash:  common.HexToHash(rawTrace.TransactionHash),
	// 			TransactionIndex: rawTrace.TransactionIndex,
	// 			TraceAddress:     rawTrace.TraceAddress,
	// 			Subtraces:        rawTrace.Subtraces,
	// 			Type:             rawTrace.Type,
	// 			// Action:           rawTrace.Action,
	// 			// Result:           rawTrace.Result,
	// 			CompressedTrace: rawTrace.CompressedTrace,
	// 		})
	// 	}
	// 	return ret, nil
	// }

	// params := , "[\""+str_2_Hash(hashIn)+"\"]", true) + "]"
	// return true

	// traces, err := GetTracesByBlockNumber(chain, bn)
	// if err != nil {
	// 	return []SimpleTrace{}, err
	// }
	// var ret []SimpleTrace
	// for _, trace := range traces {
	// 	// fmt.Println(trace.TransactionIndex)
	// 	if trace.TransactionIndex == txid {
	// 		ret = append(ret, trace)
	// 	}
	// }
	// return ret, nil
}

// func mustParseUint(input any) (result uint64) {
// 	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
// 	return
// }

// EXISTING_CODE
