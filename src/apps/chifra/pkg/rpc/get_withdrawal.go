// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// GetMinerAndWithdrawals returns the miner and withdrawals for a block
func (conn *Connection) GetMinerAndWithdrawals(bn base.Blknum) ([]types.Withdrawal, base.Address, error) {
	if bn < base.KnownBlock(conn.Chain, base.Merge) {
		return []types.Withdrawal{}, base.ZeroAddr, nil
	}

	if block, err := conn.GetBlockHeaderByNumber(bn); err != nil {
		return []types.Withdrawal{}, base.ZeroAddr, nil
	} else {
		if withdrawals, err := conn.GetWithdrawalsByNumber(bn); err != nil {
			return []types.Withdrawal{}, base.ZeroAddr, nil
		} else {
			return withdrawals, block.Miner, nil
		}
	}
}

// GetWithdrawalsByNumber returns all withdrawals in a block
func (conn *Connection) GetWithdrawalsByNumber(bn base.Blknum) ([]types.Withdrawal, error) {
	if bn < base.KnownBlock(conn.Chain, base.Shanghai) {
		return []types.Withdrawal{}, nil
	}

	withdrawalGroup := &types.WithdrawalGroup{
		BlockNumber:      bn,
		TransactionIndex: base.NOPOSN,
	}
	if err := conn.ReadFromCache(withdrawalGroup); err == nil {
		return withdrawalGroup.Withdrawals, nil
	}

	if withdrawals, ts, err := conn.getWithdrawals(bn); err != nil {
		return withdrawals, err
	} else {
		withdrawalGroup := &types.WithdrawalGroup{
			BlockNumber:      bn,
			TransactionIndex: base.NOPOSN,
			Withdrawals:      withdrawals,
		}
		err := conn.WriteToCache(withdrawalGroup, walk.Cache_Withdrawals, ts)
		return withdrawals, err
	}
}

// getWithdrawals fetches the withdrawals from a block
func (conn *Connection) getWithdrawals(bn base.Blknum) ([]types.Withdrawal, base.Timestamp, error) {
	if block, err := conn.GetBlockHeaderByNumber(bn); err != nil {
		return []types.Withdrawal{}, base.NOPOSI, err
	} else {
		return block.Withdrawals, block.Timestamp, nil
	}
}
