// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

// TODO: The following most likely does not work for non-mainnet chains which
// TODO: don't know anything about the Known blocks.

// getBlockReward returns the block reward for a given block number
func (conn *Connection) getBlockReward(bn base.Blknum) *base.Wei {
	if bn == 0 {
		return base.NewWei(0)
	} else if bn < base.KnownBlock(conn.Chain, base.Byzantium) {
		return base.NewWei(5000000000000000000)
	} else if bn < base.KnownBlock(conn.Chain, base.Constantinople) {
		return base.NewWei(3000000000000000000)
	} else if bn < base.KnownBlock(conn.Chain, base.Merge) {
		return base.NewWei(2000000000000000000)
	} else {
		return base.NewWei(0)
	}
}
