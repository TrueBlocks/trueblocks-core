// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// GetMinerAndWithdrawals returns the miner and withdrawals for a block
func (conn *Connection) GetMinerAndWithdrawals(bn base.Blknum) ([]types.SimpleWithdrawal, base.Address, error) {
	if bn < base.KnownBlock(conn.Chain, base.Merge) {
		return []types.SimpleWithdrawal{}, base.ZeroAddr, nil
	}

	if block, err := conn.GetBlockHeaderByNumber(bn); err != nil {
		return []types.SimpleWithdrawal{}, base.ZeroAddr, nil
	} else {
		if withdrawals, err := conn.GetWithdrawalsByNumber(bn); err != nil {
			return []types.SimpleWithdrawal{}, base.ZeroAddr, nil
		} else {
			return withdrawals, block.Miner, nil
		}
	}
}

// GetWithdrawalsByNumber returns all withdrawals in a block
func (conn *Connection) GetWithdrawalsByNumber(bn base.Blknum) ([]types.SimpleWithdrawal, error) {
	if bn < base.KnownBlock(conn.Chain, base.Shanghai) {
		return []types.SimpleWithdrawal{}, nil
	}

	if conn.StoreReadable() {
		withdrawalGroup := &types.SimpleWithdrawalGroup{
			BlockNumber:      bn,
			TransactionIndex: utils.NOPOS,
		}
		if err := conn.Store.Read(withdrawalGroup, nil); err == nil {
			return withdrawalGroup.Withdrawals, nil
		}
	}

	if withdrawals, ts, err := conn.getWithdrawalsSimple(bn); err != nil {
		return withdrawals, err
	} else {
		if conn.StoreWritable() && conn.EnabledMap["withdrawals"] && base.IsFinal(conn.LatestBlockTimestamp, ts) {
			withdrawalGroup := &types.SimpleWithdrawalGroup{
				Withdrawals:      withdrawals,
				BlockNumber:      bn,
				TransactionIndex: utils.NOPOS,
			}
			if err = conn.Store.Write(withdrawalGroup, nil); err != nil {
				logger.Warn("Failed to write withdrawals to cache", err)
			}
		}

		return withdrawals, err
	}
}

// getWithdrawalsSimple fetches the withdrawals from a block
func (conn *Connection) getWithdrawalsSimple(bn base.Blknum) ([]types.SimpleWithdrawal, base.Timestamp, error) {
	if block, err := conn.GetBlockHeaderByNumber(bn); err != nil {
		return []types.SimpleWithdrawal{}, utils.NOPOSI, err
	} else {
		return block.Withdrawals, block.Timestamp, nil
	}
}
