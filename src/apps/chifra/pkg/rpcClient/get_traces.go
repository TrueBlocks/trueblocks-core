// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

// GetCountTracesInBlock returns the number of traces in a block
func (options *Options) GetCountTracesInBlock(chain string, bn uint64) (uint64, error) {
	if traces, err := options.GetTracesByNumber(chain, bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}

// GetTracesByNumber returns a slice of traces in the given block
func (options *Options) GetTracesByNumber(chain string, bn uint64) ([]types.SimpleTrace, error) {
	method := "trace_block"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if rawTraces, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return []types.SimpleTrace{}, err
	} else {
		curApp := types.SimpleAppearance{BlockNumber: uint32(^uint32(0))}
		curTs := options.GetBlockTimestamp(chain, &bn)
		var idx uint64

		// TODO: This could be loadTrace in the same way loadBlocks works
		var ret []types.SimpleTrace
		for _, rawTrace := range rawTraces {
			traceAction := types.SimpleTraceAction{
				Address:        base.HexToAddress(rawTrace.Action.Address),
				Author:         base.HexToAddress(rawTrace.Action.Author),
				Balance:        *big.NewInt(0).SetUint64(utils.MustParseUint(rawTrace.Action.Balance)),
				CallType:       rawTrace.Action.CallType,
				From:           base.HexToAddress(rawTrace.Action.From),
				Gas:            utils.MustParseUint(rawTrace.Action.Gas),
				Init:           rawTrace.Action.Init,
				Input:          rawTrace.Action.Input,
				RefundAddress:  base.HexToAddress(rawTrace.Action.RefundAddress),
				RewardType:     rawTrace.Action.RewardType,
				SelfDestructed: base.HexToAddress(rawTrace.Action.SelfDestructed),
				To:             base.HexToAddress(rawTrace.Action.To),
				Value:          *big.NewInt(0).SetUint64(utils.MustParseUint(rawTrace.Action.Value)),
			}
			traceResult := types.SimpleTraceResult{}
			if rawTrace.Result != nil {
				traceResult.Address = base.HexToAddress(rawTrace.Result.Address)
				traceResult.Code = rawTrace.Result.Code
				traceResult.GasUsed = utils.MustParseUint(rawTrace.Result.GasUsed)
				traceResult.Output = rawTrace.Result.Output
			}
			trace := types.SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        base.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				TransactionHash:  base.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				TraceType:        rawTrace.TraceType,
				Timestamp:        curTs,
				Action:           &traceAction,
				Result:           &traceResult,
			}
			if trace.BlockNumber != uint64(curApp.BlockNumber) || trace.TransactionIndex != uint64(curApp.TransactionIndex) {
				curApp = types.SimpleAppearance{
					BlockNumber:      uint32(trace.BlockNumber),
					TransactionIndex: uint32(trace.TransactionIndex),
				}
				curTs = options.GetBlockTimestamp(chain, &trace.BlockNumber)
				idx = 0
			}
			trace.TraceIndex = idx
			idx++
			trace.SetRaw(&rawTrace)
			ret = append(ret, trace)
		}
		return ret, nil
	}
}

// GetTracesByTransactionID returns a slice of traces in a given transaction
func (options *Options) GetTracesByTransactionID(chain string, bn, txid uint64) ([]types.SimpleTrace, error) {
	var ret []types.SimpleTrace
	if options.HasStore() {
		traceGroup := &types.SimpleTraceGroup{
			BlockNumber:      bn,
			TransactionIndex: int(txid),
		}

		if err := options.Store.Read(traceGroup, nil); err == nil {
			// success
			return traceGroup.Traces, nil
		}
	}

	txHash, err := options.GetTransactionHashByNumberAndID(chain, bn, txid)
	if err != nil {
		return ret, err
	}
	return options.GetTracesByTransactionHash(chain, txHash, nil)
}

// GetCountTracesInTransaction returns the number of traces in a given transaction
func (options *Options) GetCountTracesInTransaction(chain string, txHash string) (uint64, error) {
	traces, err := options.GetTracesByTransactionHash(chain, txHash, nil)
	if err != nil {
		return 0, err
	}
	return uint64(len(traces)), nil
}

// GetTracesByFilter returns a slice of traces in a given transaction's hash
func (options *Options) GetTracesByFilter(chain string, filter string) ([]types.SimpleTrace, error) {
	method := "trace_filter"
	var f types.SimpleTraceFilter
	ff := f.ParseBangString(filter)
	params := rpc.Params{ff}

	var ret []types.SimpleTrace
	if rawTraces, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return ret, fmt.Errorf("trace filter %s returned an error: %w", filter, ethereum.NotFound)
	} else {
		curApp := types.SimpleAppearance{BlockNumber: uint32(^uint32(0))}
		curTs := options.GetBlockTimestamp(chain, utils.PointerOf(utils.MustParseUint(f.FromBlock)))
		var idx uint64

		// TODO: This could be loadTrace in the same way loadBlocks works
		for _, rawTrace := range rawTraces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			rawTrace := rawTrace

			value := big.NewInt(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := big.NewInt(0)
			balance.SetString(rawTrace.Action.Balance, 0)

			action := types.SimpleTraceAction{
				CallType:       rawTrace.Action.CallType,
				From:           base.HexToAddress(rawTrace.Action.From),
				Gas:            utils.MustParseUint(rawTrace.Action.Gas),
				Input:          rawTrace.Action.Input,
				To:             base.HexToAddress(rawTrace.Action.To),
				Value:          *value,
				Balance:        *balance,
				Address:        base.HexToAddress(rawTrace.Action.Address),
				RefundAddress:  base.HexToAddress(rawTrace.Action.RefundAddress),
				SelfDestructed: base.HexToAddress(rawTrace.Action.SelfDestructed),
				Init:           rawTrace.Action.Init,
			}
			action.SetRaw(&rawTrace.Action)

			var result *types.SimpleTraceResult
			if rawTrace.Result != nil {
				result = &types.SimpleTraceResult{
					GasUsed: utils.MustParseUint(rawTrace.Result.GasUsed),
					Output:  rawTrace.Result.Output,
					Code:    rawTrace.Result.Code,
				}
				if len(rawTrace.Result.Address) > 0 {
					result.Address = base.HexToAddress(rawTrace.Result.Address)
				}
				result.SetRaw(rawTrace.Result)
			}

			trace := types.SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        base.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				TransactionHash:  base.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				TraceType:        rawTrace.TraceType,
				Timestamp:        curTs,
				Action:           &action,
				Result:           result,
			}
			if trace.BlockNumber != uint64(curApp.BlockNumber) || trace.TransactionIndex != uint64(curApp.TransactionIndex) {
				curApp = types.SimpleAppearance{
					BlockNumber:      uint32(trace.BlockNumber),
					TransactionIndex: uint32(trace.TransactionIndex),
				}
				curTs = options.GetBlockTimestamp(chain, utils.PointerOf(trace.BlockNumber))
				idx = 0
			}
			trace.TraceIndex = idx
			idx++

			trace.SetRaw(&rawTrace)
			ret = append(ret, trace)
		}
	}
	return ret, nil
}

// GetTracesByTransactionHash returns a slice of traces in a given transaction's hash
func (options *Options) GetTracesByTransactionHash(chain string, txHash string, transaction *types.SimpleTransaction) ([]types.SimpleTrace, error) {
	if options.HasStore() && transaction != nil {
		traceGroup := types.NewSimpleTraceGroup(transaction)

		if err := options.Store.Read(traceGroup, nil); err == nil {
			// success
			return traceGroup.Traces, nil
		}
	}

	method := "trace_transaction"
	params := rpc.Params{txHash}

	var ret []types.SimpleTrace
	if rawTraces, err := rpc.QuerySlice[types.RawTrace](chain, method, params); err != nil {
		return ret, fmt.Errorf("transaction at %s returned an error: %w", txHash, ethereum.NotFound)

	} else {
		curApp := types.SimpleAppearance{BlockNumber: uint32(^uint32(0))}
		var idx uint64

		for _, rawTrace := range rawTraces {
			// Note: This is needed because of a GoLang bug when taking the pointer of a loop variable
			rawTrace := rawTrace

			value := big.NewInt(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := big.NewInt(0)
			balance.SetString(rawTrace.Action.Balance, 0)

			action := types.SimpleTraceAction{
				CallType:       rawTrace.Action.CallType,
				From:           base.HexToAddress(rawTrace.Action.From),
				Gas:            utils.MustParseUint(rawTrace.Action.Gas),
				Input:          rawTrace.Action.Input,
				To:             base.HexToAddress(rawTrace.Action.To),
				Value:          *value,
				Balance:        *balance,
				Address:        base.HexToAddress(rawTrace.Action.Address),
				RefundAddress:  base.HexToAddress(rawTrace.Action.RefundAddress),
				SelfDestructed: base.HexToAddress(rawTrace.Action.SelfDestructed),
				Init:           rawTrace.Action.Init,
			}
			action.SetRaw(&rawTrace.Action)

			var result *types.SimpleTraceResult
			if rawTrace.Result != nil {
				result = &types.SimpleTraceResult{
					GasUsed: utils.MustParseUint(rawTrace.Result.GasUsed),
					Output:  rawTrace.Result.Output,
					Code:    rawTrace.Result.Code,
				}
				if len(rawTrace.Result.Address) > 0 {
					result.Address = base.HexToAddress(rawTrace.Result.Address)
				}
				result.SetRaw(rawTrace.Result)
			}

			// TODO: This could be loadTrace in the same way loadBlocks works
			trace := types.SimpleTrace{
				Error:            rawTrace.Error,
				BlockHash:        base.HexToHash(rawTrace.BlockHash),
				BlockNumber:      rawTrace.BlockNumber,
				TransactionHash:  base.HexToHash(rawTrace.TransactionHash),
				TransactionIndex: rawTrace.TransactionIndex,
				TraceAddress:     rawTrace.TraceAddress,
				Subtraces:        rawTrace.Subtraces,
				TraceType:        rawTrace.TraceType,
				Action:           &action,
				Result:           result,
			}
			if transaction != nil {
				trace.Timestamp = transaction.Timestamp
			}
			if trace.BlockNumber != uint64(curApp.BlockNumber) || trace.TransactionIndex != uint64(curApp.TransactionIndex) {
				curApp = types.SimpleAppearance{
					BlockNumber:      uint32(trace.BlockNumber),
					TransactionIndex: uint32(trace.TransactionIndex),
				}
				idx = 0
			}
			trace.TraceIndex = idx
			idx++

			trace.SetRaw(&rawTrace)
			ret = append(ret, trace)
		}

		if options.HasStore() && options.enabledMap["traces"] && transaction != nil {
			var writeOptions *cache.WriteOptions
			if !options.Store.ReadOnly() {
				writeOptions = &cache.WriteOptions{
					// Check if the block is final
					Pending: (&types.SimpleBlock[string]{Timestamp: transaction.Timestamp}).Pending(options.LatestBlockTimestamp),
				}
			}
			traceGroup := types.NewSimpleTraceGroup(transaction)
			traceGroup.Traces = ret
			options.Store.Write(traceGroup, writeOptions)
		}

		return ret, nil
	}
}
