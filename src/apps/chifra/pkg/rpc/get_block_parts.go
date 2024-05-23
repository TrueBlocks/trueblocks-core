// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/ethereum/go-ethereum/common"
)

// GetBlockTimestamp returns the timestamp associated with a given block
func (conn *Connection) GetBlockTimestamp(bn base.Blknum) base.Timestamp {
	if ec, err := conn.getClient(); err != nil {
		logger.Error("Could not connect to RPC client", err)
		return 0
	} else {
		defer ec.Close()

		r, err := ec.HeaderByNumber(context.Background(), base.BiFromBn(bn))
		if err != nil {
			logger.Error("Could not connect to RPC client", err)
			return 0
		}

		ts := base.Timestamp(r.Time)
		if ts == 0 {
			// The RPC does not return a timestamp for block zero, so we simulate it with ts from block one less 13 seconds
			// TODO: Chain specific
			return conn.GetBlockTimestamp(1) - 13
		}

		return ts
	}
}

// GetBlockHashByHash returns a block's hash if it's a valid block
func (conn *Connection) GetBlockHashByHash(hash string) (base.Hash, error) {
	if ec, err := conn.getClient(); err != nil {
		return base.Hash{}, err
	} else {
		defer ec.Close()

		ethBlock, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return base.Hash{}, err
		}

		return base.HexToHash(ethBlock.Hash().Hex()), nil
	}
}

// GetBlockNumberByHash returns a block's hash if it's a valid block
func (conn *Connection) GetBlockNumberByHash(hash string) (base.Blknum, error) {
	if ec, err := conn.getClient(); err != nil {
		return 0, err
	} else {
		defer ec.Close()

		ethBlock, err := ec.BlockByHash(context.Background(), common.HexToHash(hash))
		if err != nil {
			return 0, err
		}

		return base.Blknum(ethBlock.NumberU64()), nil
	}
}

// GetBlockHashByNumber returns a block's hash if it's a valid block
func (conn *Connection) GetBlockHashByNumber(bn base.Blknum) (base.Hash, error) {
	if block, err := conn.GetBlockHeaderByNumber(bn); err != nil {
		return base.Hash{}, err
	} else {
		return block.Hash, err
	}
}
