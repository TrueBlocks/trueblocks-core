// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

// GetTracesByBlockNumber returns a slice of traces in the given block
func (conn *Connection) GetTracesByBlockNumber(bn uint64) ([]types.SimpleTrace, error) {
	if conn.StoreReadable() {
		traceGroup := &types.SimpleTraceGroup{
			BlockNumber:      bn,
			TransactionIndex: utils.NOPOS,
		}
		if err := conn.Store.Read(traceGroup, nil); err == nil {
			return traceGroup.Traces, nil
		}
	}

	method := "trace_block"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if rawTraces, err := query.Query[[]types.RawTrace](conn.Chain, method, params); err != nil {
		return []types.SimpleTrace{}, err

	} else if rawTraces == nil || len(*rawTraces) == 0 {
		return []types.SimpleTrace{}, nil

	} else {
		curApp := types.SimpleAppearance{BlockNumber: uint32(^uint32(0))}
		curTs := conn.GetBlockTimestamp(bn)
		var idx uint64

		// TODO: This could be loadTrace in the same way load Blocks works
		var ret []types.SimpleTrace
		for _, rawTrace := range *rawTraces {
			traceAction := types.SimpleTraceAction{
				Address:        base.HexToAddress(rawTrace.Action.Address),
				Author:         base.HexToAddress(rawTrace.Action.Author),
				Balance:        *base.NewWei(0).SetUint64(utils.MustParseUint(rawTrace.Action.Balance)),
				CallType:       rawTrace.Action.CallType,
				From:           base.HexToAddress(rawTrace.Action.From),
				Gas:            utils.MustParseUint(rawTrace.Action.Gas),
				Init:           rawTrace.Action.Init,
				Input:          rawTrace.Action.Input,
				RefundAddress:  base.HexToAddress(rawTrace.Action.RefundAddress),
				RewardType:     rawTrace.Action.RewardType,
				SelfDestructed: base.HexToAddress(rawTrace.Action.SelfDestructed),
				To:             base.HexToAddress(rawTrace.Action.To),
				Value:          *base.NewWei(0).SetUint64(utils.MustParseUint(rawTrace.Action.Value)),
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
			if trace.TransactionIndex != uint64(curApp.TransactionIndex) {
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

		if conn.StoreWritable() && conn.EnabledMap["traces"] && base.IsFinal(conn.LatestBlockTimestamp, curTs) {
			traceGroup := &types.SimpleTraceGroup{
				Traces:           ret,
				BlockNumber:      bn,
				TransactionIndex: utils.NOPOS,
			}
			_ = conn.Store.Write(traceGroup, nil)
		}
		return ret, nil
	}
}

// GetTracesByTransactionId returns a slice of traces in a given transaction
func (conn *Connection) GetTracesByTransactionId(bn, txid uint64) ([]types.SimpleTrace, error) {
	if conn.StoreReadable() {
		traceGroup := &types.SimpleTraceGroup{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}
		if err := conn.Store.Read(traceGroup, nil); err == nil {
			return traceGroup.Traces, nil
		}
	}

	tx, err := conn.GetTransactionByNumberAndId(bn, txid)
	if err != nil {
		return []types.SimpleTrace{}, err
	}

	return conn.GetTracesByTransactionHash(tx.Hash.Hex(), tx)
}

// GetTracesByTransactionHash returns a slice of traces in a given transaction's hash
func (conn *Connection) GetTracesByTransactionHash(txHash string, transaction *types.SimpleTransaction) ([]types.SimpleTrace, error) {
	if conn.StoreReadable() && transaction != nil {
		traceGroup := &types.SimpleTraceGroup{
			Traces:           make([]types.SimpleTrace, 0, len(transaction.Traces)),
			BlockNumber:      transaction.BlockNumber,
			TransactionIndex: transaction.TransactionIndex,
		}
		if err := conn.Store.Read(traceGroup, nil); err == nil {
			return traceGroup.Traces, nil
		}
	}

	method := "trace_transaction"
	params := query.Params{txHash}

	var ret []types.SimpleTrace
	if rawTraces, err := query.Query[[]types.RawTrace](conn.Chain, method, params); err != nil {
		return ret, ethereum.NotFound

	} else if rawTraces == nil || len(*rawTraces) == 0 {
		return []types.SimpleTrace{}, nil

	} else {
		curApp := types.SimpleAppearance{BlockNumber: uint32(^uint32(0))}
		var idx uint64

		for _, rawTrace := range *rawTraces {
			value := base.NewWei(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := base.NewWei(0)
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

			// TODO: This could be loadTrace in the same way load Blocks works
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

		if transaction != nil {
			if conn.StoreWritable() && conn.EnabledMap["traces"] && base.IsFinal(conn.LatestBlockTimestamp, transaction.Timestamp) {
				traceGroup := &types.SimpleTraceGroup{
					Traces:           ret,
					BlockNumber:      transaction.BlockNumber,
					TransactionIndex: transaction.TransactionIndex,
				}
				_ = conn.Store.Write(traceGroup, nil)
			}
		}

		return ret, nil
	}
}

// GetTracesByFilter returns a slice of traces in a given transaction's hash
func (conn *Connection) GetTracesByFilter(filter string) ([]types.SimpleTrace, error) {
	var f types.SimpleTraceFilter
	ff, _ := f.ParseBangString(conn.Chain, filter)

	method := "trace_filter"
	params := query.Params{ff}

	var ret []types.SimpleTrace
	if rawTraces, err := query.Query[[]types.RawTrace](conn.Chain, method, params); err != nil {
		return ret, fmt.Errorf("trace filter %s returned an error: %w", filter, ethereum.NotFound)

	} else if rawTraces == nil || len(*rawTraces) == 0 {
		return []types.SimpleTrace{}, nil

	} else {
		curApp := types.SimpleAppearance{BlockNumber: uint32(^uint32(0))}
		curTs := conn.GetBlockTimestamp(utils.MustParseUint(f.FromBlock))
		var idx uint64

		// TODO: This could be loadTrace in the same way load Blocks works
		for _, rawTrace := range *rawTraces {
			value := base.NewWei(0)
			value.SetString(rawTrace.Action.Value, 0)
			balance := base.NewWei(0)
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

			if trace.BlockNumber != uint64(curApp.BlockNumber) {
				curTs = conn.GetBlockTimestamp(trace.BlockNumber)
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
	}
	return ret, nil
}

// GetTracesCountInBlock returns the number of traces in a block
func (conn *Connection) GetTracesCountInBlock(bn uint64) (uint64, error) {
	if traces, err := conn.GetTracesByBlockNumber(bn); err != nil {
		return utils.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}
