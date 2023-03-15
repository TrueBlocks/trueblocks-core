// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/common"
)

// GetTraceCountByBlockNumber returns the number of traces in a block
func GetTraceCountByBlockNumber(chain string, bn uint64) (uint64, error) {
	if traces, err := GetTracesByBlockNumber(chain, bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}

// GetTracesByBlockNumber returns a slice of traces in the given block
func GetTracesByBlockNumber(chain string, bn uint64) ([]types.SimpleTrace, error) {
	method := "trace_block"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if rawTraces, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return []types.SimpleTrace{}, err
	} else {
		var ret []types.SimpleTrace
		for _, rawTrace := range rawTraces {
			trace := types.SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        common.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				TransactionHash:  common.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				Type:             rawTrace.Type,
				// raw:              &rawTrace,
				// Action:           rawTrace.Action,
				// Result:           rawTrace.Result,
			}
			trace.SetRaw(&rawTrace)
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
func GetTracesByTransactionId(chain string, bn, txid uint64) ([]types.SimpleTrace, error) {
	var ret []types.SimpleTrace

	txHash, err := rpc.TxHashFromNumberAndId(chain, bn, txid)
	if err != nil {
		return ret, err
	}
	return GetTracesByTransactionHash(chain, txHash, nil)
}

// GetTracesCountByTransactionHash returns the number of traces in a given transaction
func GetTracesCountByTransactionHash(chain string, txHash string) (uint64, error) {
	traces, err := GetTracesByTransactionHash(chain, txHash, nil)
	if err != nil {
		return 0, err
	}
	return uint64(len(traces)), nil
}

// GetTracesByFilter returns a slice of traces in a given transaction's hash
func GetTracesByFilter(chain string, filter string) ([]types.SimpleTrace, error) {
	method := "trace_filter"
	type Thing struct {
		FromBlock string   `json:"fromBlock"`
		ToBlock   string   `json:"toBlock"`
		ToAddress []string `json:"toAddress"`
		After     int      `json:"after"`
		Count     int      `json:"count"`
	}
	v := Thing{
		FromBlock: "0x2ed0c4",
		ToBlock:   "0x2ed128",
		ToAddress: []string{"0x8bbb73bcb5d553b5a556358d27625323fd781d37"},
		After:     1000,
		Count:     100,
	}
	params := rpc.Params{v}

	var ret []types.SimpleTrace
	if rawTraces, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return ret, fmt.Errorf("trace filter %s returned an error: %w", filter, ethereum.NotFound)
	} else {
		for _, rawTrace := range rawTraces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			rawTrace := rawTrace

			value := big.NewInt(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := big.NewInt(0)
			balance.SetString(rawTrace.Action.Balance, 0)

			action := types.SimpleTraceAction{
				CallType:       rawTrace.Action.CallType,
				From:           types.HexToAddress(rawTrace.Action.From),
				Gas:            mustParseUint(rawTrace.Action.Gas),
				Input:          rawTrace.Action.Input,
				To:             types.HexToAddress(rawTrace.Action.To),
				Value:          *value,
				Balance:        *balance,
				Address:        types.HexToAddress(rawTrace.Action.Address),
				RefundAddress:  types.HexToAddress(rawTrace.Action.RefundAddress),
				SelfDestructed: types.HexToAddress(rawTrace.Action.SelfDestructed),
				Init:           rawTrace.Action.Init,
			}
			action.SetRaw(&rawTrace.Action)

			var result *types.SimpleTraceResult
			if rawTrace.Result != nil {
				result = &types.SimpleTraceResult{
					GasUsed: mustParseUint(rawTrace.Result.GasUsed),
					Output:  rawTrace.Result.Output,
					Code:    rawTrace.Result.Code,
				}
				if len(rawTrace.Result.Address) > 0 {
					result.Address = types.HexToAddress(rawTrace.Result.Address)
				}
				result.SetRaw(rawTrace.Result)
			}

			trace := types.SimpleTrace{
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
			// if transaction != nil {
			// 	trace.Timestamp = transaction.Timestamp
			// }
			trace.SetRaw(&rawTrace)
			ret = append(ret, trace)
		}
	}
	return ret, nil
}

// GetTracesByTransactionHash returns a slice of traces in a given transaction's hash
func GetTracesByTransactionHash(chain string, txHash string, transaction *types.SimpleTransaction) ([]types.SimpleTrace, error) {
	method := "trace_transaction"
	params := rpc.Params{txHash}

	var ret []types.SimpleTrace
	if rawTraces, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return ret, fmt.Errorf("transaction at %s returned an error: %w", txHash, ethereum.NotFound)

	} else {
		for _, rawTrace := range rawTraces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			rawTrace := rawTrace

			value := big.NewInt(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := big.NewInt(0)
			balance.SetString(rawTrace.Action.Balance, 0)

			action := types.SimpleTraceAction{
				CallType:       rawTrace.Action.CallType,
				From:           types.HexToAddress(rawTrace.Action.From),
				Gas:            mustParseUint(rawTrace.Action.Gas),
				Input:          rawTrace.Action.Input,
				To:             types.HexToAddress(rawTrace.Action.To),
				Value:          *value,
				Balance:        *balance,
				Address:        types.HexToAddress(rawTrace.Action.Address),
				RefundAddress:  types.HexToAddress(rawTrace.Action.RefundAddress),
				SelfDestructed: types.HexToAddress(rawTrace.Action.SelfDestructed),
				Init:           rawTrace.Action.Init,
			}
			action.SetRaw(&rawTrace.Action)

			var result *types.SimpleTraceResult
			if rawTrace.Result != nil {
				result = &types.SimpleTraceResult{
					GasUsed: mustParseUint(rawTrace.Result.GasUsed),
					Output:  rawTrace.Result.Output,
					Code:    rawTrace.Result.Code,
				}
				if len(rawTrace.Result.Address) > 0 {
					result.Address = types.HexToAddress(rawTrace.Result.Address)
				}
				result.SetRaw(rawTrace.Result)
			}

			trace := types.SimpleTrace{
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
			if transaction != nil {
				trace.Timestamp = transaction.Timestamp
			}
			trace.SetRaw(&rawTrace)
			ret = append(ret, trace)
		}
		return ret, nil
	}
}
