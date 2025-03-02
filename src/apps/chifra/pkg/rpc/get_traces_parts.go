// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// GetTracesByTransactionId returns a slice of traces in a given transaction
func (conn *Connection) GetTracesByTransactionId(bn base.Blknum, txid base.Txnum) ([]types.Trace, error) {
	if conn.Store != nil {
		// walk.Cache_Traces
		traceGroup := &types.TraceGroup{
			BlockNumber:      bn,
			TransactionIndex: txid,
		}
		if err := conn.Store.Read(traceGroup); err == nil {
			return traceGroup.Traces, nil
		}
	}

	tx, err := conn.GetTransactionByNumberAndId(bn, txid)
	if err != nil {
		return []types.Trace{{
			Action: &types.TraceAction{},
			Result: &types.TraceResult{},
		}}, err
	}

	return conn.GetTracesByTransactionHash(tx.Hash.Hex(), tx)
}

// GetTracesCountInBlock returns the number of traces in a block
func (conn *Connection) GetTracesCountInBlock(bn base.Blknum) (uint64, error) {
	if traces, err := conn.GetTracesByBlockNumber(bn); err != nil {
		return base.NOPOS, err
	} else {
		return uint64(len(traces)), nil
	}
}
