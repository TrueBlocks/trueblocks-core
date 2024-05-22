// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTracesByBlockNumber returns a slice of traces in the given block
func (conn *Connection) GetTracesByBlockNumber(bn base.Blknum) ([]types.Trace, error) {
	if conn.StoreReadable() {
		traceGroup := &types.TraceGroup{
			BlockNumber:      bn,
			TransactionIndex: base.NOPOSN, // no tx id means we're storing the whole block
		}
		if err := conn.Store.Read(traceGroup, nil); err == nil {
			return traceGroup.Traces, nil
		}
	}

	curTs := conn.GetBlockTimestamp(bn) // same for every trace
	isFinal := base.IsFinal(conn.LatestBlockTimestamp, curTs)

	method := "trace_block"
	params := query.Params{fmt.Sprintf("0x%x", bn)}

	if rawTraces, err := query.Query[[]types.Trace](conn.Chain, method, params); err != nil {
		return []types.Trace{}, err

	} else if rawTraces == nil || len(*rawTraces) == 0 {
		return []types.Trace{}, nil

	} else {
		curTx := base.NOPOSN

		var traceIndex base.Tracenum
		for i := range *rawTraces {
			rawTrace := &(*rawTraces)[i]
			rawTrace.Timestamp = curTs
			if rawTrace.Result == nil {
				rawTrace.Result = &types.TraceResult{}
			}
			rawTrace.TransactionIndex = rawTrace.TransactionPosition
			if rawTrace.TransactionIndex != base.Txnum(curTx) {
				curTx = rawTrace.TransactionIndex
				traceIndex = 0
			}
			rawTrace.TraceIndex = traceIndex
			traceIndex++
		}

		if conn.StoreWritable() && conn.EnabledMap["traces"] && isFinal {
			traceGroup := &types.TraceGroup{
				Traces:           *rawTraces,
				BlockNumber:      bn,
				TransactionIndex: base.NOPOSN,
			}
			_ = conn.Store.Write(traceGroup, nil)
		}
		return *rawTraces, nil
	}
}

// GetTracesByTransactionHash returns a slice of traces in a given transaction's hash
func (conn *Connection) GetTracesByTransactionHash(txHash string, transaction *types.Transaction) ([]types.Trace, error) {
	if conn.StoreReadable() && transaction != nil {
		traceGroup := &types.TraceGroup{
			Traces:           make([]types.Trace, 0, len(transaction.Traces)),
			BlockNumber:      transaction.BlockNumber,
			TransactionIndex: transaction.TransactionIndex,
		}
		if err := conn.Store.Read(traceGroup, nil); err == nil {
			return traceGroup.Traces, nil
		}
	}

	method := "trace_transaction"
	params := query.Params{txHash}

	if rawTraces, err := query.Query[[]types.Trace](conn.Chain, method, params); err != nil {
		return []types.Trace{{
			Result: &types.TraceResult{},
		}}, err

	} else if rawTraces == nil || len(*rawTraces) == 0 {
		return []types.Trace{{
			Result: &types.TraceResult{},
		}}, err

	} else {
		curApp := types.Appearance{BlockNumber: uint32(^uint32(0))}
		var traceIndex base.Tracenum
		for i := range *rawTraces {
			rawTrace := &(*rawTraces)[i]
			if rawTrace.Result == nil {
				rawTrace.Result = &types.TraceResult{}
			}
			if transaction != nil {
				rawTrace.Timestamp = transaction.Timestamp
			}
			rawTrace.TransactionIndex = rawTrace.TransactionPosition
			if rawTrace.BlockNumber != base.Blknum(curApp.BlockNumber) || rawTrace.TransactionIndex != base.Txnum(curApp.TransactionIndex) {
				curApp = types.Appearance{
					BlockNumber:      uint32(rawTrace.BlockNumber),
					TransactionIndex: uint32(rawTrace.TransactionIndex),
				}
				traceIndex = 0
			}
			rawTrace.TraceIndex = traceIndex
			traceIndex++
		}

		if transaction != nil {
			if conn.StoreWritable() && conn.EnabledMap["traces"] && base.IsFinal(conn.LatestBlockTimestamp, transaction.Timestamp) {
				traceGroup := &types.TraceGroup{
					Traces:           *rawTraces,
					BlockNumber:      transaction.BlockNumber,
					TransactionIndex: transaction.TransactionIndex,
				}
				_ = conn.Store.Write(traceGroup, nil)
			}
		}

		return *rawTraces, nil
	}
}
