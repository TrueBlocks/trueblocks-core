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

	if traces, err := query.Query[[]types.Trace](conn.Chain, method, params); err != nil {
		return []types.Trace{{
			Action: &types.TraceAction{},
			Result: &types.TraceResult{},
		}}, err

	} else if traces == nil || len(*traces) == 0 {
		return []types.Trace{{
			Action: &types.TraceAction{},
			Result: &types.TraceResult{},
		}}, err

	} else {
		curTx := base.NOPOSN

		var traceIndex base.Tracenum
		for i := range *traces {
			traces := &(*traces)[i]
			traces.Timestamp = curTs
			if traces.Result == nil {
				traces.Result = &types.TraceResult{}
			}
			traces.TransactionIndex = traces.TransactionPosition
			if traces.TransactionIndex != base.Txnum(curTx) {
				curTx = traces.TransactionIndex
				traceIndex = 0
			}
			traces.TraceIndex = traceIndex
			traceIndex++
		}

		if isFinal && conn.StoreWritable() && conn.EnabledMap["traces"] {
			traceGroup := &types.TraceGroup{
				Traces:           *traces,
				BlockNumber:      bn,
				TransactionIndex: base.NOPOSN,
			}
			_ = conn.Store.Write(traceGroup, nil)
		}
		return *traces, nil
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

	if traces, err := query.Query[[]types.Trace](conn.Chain, method, params); err != nil {
		return []types.Trace{{
			Action: &types.TraceAction{},
			Result: &types.TraceResult{},
		}}, err

	} else if traces == nil || len(*traces) == 0 {
		return []types.Trace{{
			Action: &types.TraceAction{},
			Result: &types.TraceResult{},
		}}, err

	} else {
		curApp := types.Appearance{BlockNumber: uint32(^uint32(0))}
		var traceIndex base.Tracenum
		for i := range *traces {
			traces := &(*traces)[i]
			if transaction != nil {
				traces.Timestamp = transaction.Timestamp
			}
			if traces.Result == nil {
				traces.Result = &types.TraceResult{}
			}
			traces.TransactionIndex = traces.TransactionPosition
			if traces.BlockNumber != base.Blknum(curApp.BlockNumber) || traces.TransactionIndex != base.Txnum(curApp.TransactionIndex) {
				curApp = types.Appearance{
					BlockNumber:      uint32(traces.BlockNumber),
					TransactionIndex: uint32(traces.TransactionIndex),
				}
				traceIndex = 0
			}
			traces.TraceIndex = traceIndex
			traceIndex++
		}

		if transaction != nil {
			isFinal := base.IsFinal(conn.LatestBlockTimestamp, transaction.Timestamp)
			if isFinal && conn.StoreWritable() && conn.EnabledMap["traces"] {
				traceGroup := &types.TraceGroup{
					Traces:           *traces,
					BlockNumber:      transaction.BlockNumber,
					TransactionIndex: transaction.TransactionIndex,
				}
				_ = conn.Store.Write(traceGroup, nil)
			}
		}

		return *traces, nil
	}
}
