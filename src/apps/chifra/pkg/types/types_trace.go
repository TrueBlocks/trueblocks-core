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
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

// TODO: CompressedTrace is not part of the raw trace data
// EXISTING_CODE

type RawTrace struct {
	Action           RawTraceAction  `json:"action"`
	BlockHash        string          `json:"blockHash"`
	BlockNumber      uint64          `json:"blockNumber"`
	Error            string          `json:"error,omitempty"`
	Result           *RawTraceResult `json:"result"`
	Subtraces        uint64          `json:"subtraces"`
	TraceAddress     []uint64        `json:"traceAddress"`
	TransactionHash  string          `json:"transactionHash"`
	TransactionIndex uint64          `json:"transactionPosition"`
	Type             string          `json:"type"`
}

type SimpleTrace struct {
	Action           *SimpleTraceAction `json:"action"`
	ArticulatedTrace *SimpleFunction    `json:"articulatedTrace"`
	BlockHash        common.Hash        `json:"blockHash"`
	BlockNumber      uint64             `json:"blockNumber"`
	CompressedTrace  string             `json:"compressedTrace,omitempty"`
	Error            string             `json:"error,omitempty"`
	Result           *SimpleTraceResult `json:"result"`
	Subtraces        uint64             `json:"subtraces"`
	Timestamp        int64              `json:"timestamp"`
	TraceAddress     []uint64           `json:"traceAddress"`
	TransactionHash  common.Hash        `json:"transactionHash"`
	TransactionIndex uint64             `json:"transactionIndex"`
	Type             string             `json:"type,omitempty"`
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
		"articulatedTrace": s.ArticulatedTrace,
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"result":           s.Result,
		"subtraces":        s.Subtraces,
		"timestamp":        s.Timestamp,
		"traceAddress":     s.TraceAddress,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
	}

	order := []string{
		"articulatedTrace",
		"blockHash",
		"blockNumber",
		"result",
		"subtraces",
		"timestamp",
		"traceAddress",
		"transactionHash",
		"transactionIndex",
	}

	// EXISTING_CODE
	if format == "json" {
		if len(s.Error) > 0 {
			model["error"] = s.Error
		}
		if len(s.Type) > 0 {
			model["type"] = s.Type
		}
		if s.Action != nil {
			model["action"] = s.Action.Model(showHidden, format, extraOptions).Data
		}
		if s.Result != nil {
			model["result"] = s.Result.Model(showHidden, format, extraOptions).Data
		}
	}
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
			trace := SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        common.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				TransactionHash:  common.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				Type:             rawTrace.Type,
				raw:              &rawTrace,
				// Action:           rawTrace.Action,
				// Result:           rawTrace.Result,
			}
			ret = append(ret, trace)
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

	txHash, err := rpc.TxHashFromNumberAndId(chain, bn, txid)
	if err != nil {
		return ret, err
	}

	return GetTracesByTransactionHash(chain, txHash)
}

// GetTracesCountByTransactionHash returns the number of traces in a given transaction
func GetTracesCountByTransactionHash(chain string, txHash string) (uint64, error) {
	traces, err := GetTracesByTransactionHash(chain, txHash)
	if err != nil {
		return 0, err
	}
	return uint64(len(traces)), nil
}

// GetTracesByTransactionHash returns a slice of traces in a given transaction's hash
func GetTracesByTransactionHash(chain string, txHash string) ([]SimpleTrace, error) {
	method := "trace_transaction"
	params := rpc.Params{txHash}

	var ret []SimpleTrace
	if rawTraces, err := rpc.QuerySlice[RawTrace](chain, method, params); err != nil {
		return ret, fmt.Errorf("transaction at %s returned an error: %w", txHash, ethereum.NotFound)

	} else {
		for _, rawTrace := range rawTraces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			rawTrace := rawTrace

			value := big.NewInt(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := big.NewInt(0)
			balance.SetString(rawTrace.Action.Balance, 0)

			action := SimpleTraceAction{
				CallType:       rawTrace.Action.CallType,
				From:           common.HexToAddress(rawTrace.Action.From),
				Gas:            mustParseUint(rawTrace.Action.Gas),
				Input:          rawTrace.Action.Input,
				To:             common.HexToAddress(rawTrace.Action.To),
				Value:          *value,
				Balance:        *balance,
				Address:        common.HexToAddress(rawTrace.Action.Address),
				RefundAddress:  common.HexToAddress(rawTrace.Action.RefundAddress),
				SelfDestructed: common.HexToAddress(rawTrace.Action.SelfDestructed),
				Init:           rawTrace.Action.Init,
			}
			action.SetRaw(&rawTrace.Action)

			var result *SimpleTraceResult
			if rawTrace.Result != nil {
				result = &SimpleTraceResult{
					GasUsed: mustParseUint(rawTrace.Result.GasUsed),
					Output:  rawTrace.Result.Output,
					Code:    rawTrace.Result.Code,
				}
				if len(rawTrace.Result.NewContract) > 0 {
					result.NewContract = common.HexToAddress(rawTrace.Result.NewContract)
				} else if len(rawTrace.Result.Address) > 0 {
					result.NewContract = common.HexToAddress(rawTrace.Result.Address)
					result.NewContract = common.HexToAddress(rawTrace.Result.Address)
				}
				result.SetRaw(rawTrace.Result)
			}

			trace := SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        common.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				TransactionHash:  common.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				Type:             rawTrace.Type,
				Action:           &action,
				Result:           result,
			}
			trace.SetRaw(&rawTrace)
			ret = append(ret, trace)
		}
		return ret, nil
	}
}

func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

// EXISTING_CODE
