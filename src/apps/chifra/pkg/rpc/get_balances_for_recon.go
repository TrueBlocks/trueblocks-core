package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type BalanceOptions struct {
	PrevAppBlk base.Blknum  // the block of the previous appearance
	CurrBlk    base.Blknum  // the block for this appearance
	Asset      base.Address // the asset whose balance we need
	Holder     base.Address // the holder whose balance we need
}

func (c *Connection) LoadReconcilationBalances(opts *BalanceOptions, s *types.Statement) error {
	prevBal, _ := c.GetBalanceAtToken(opts.Asset, opts.Holder, opts.PrevAppBlk)
	begBal, _ := c.GetBalanceAtToken(opts.Asset, opts.Holder, opts.CurrBlk-1)
	endBal, _ := c.GetBalanceAtToken(opts.Asset, opts.Holder, opts.CurrBlk)
	if opts.CurrBlk == 0 {
		prevBal = base.ZeroWei
		begBal = base.ZeroWei
	}
	s.PrevBal = *prevBal
	s.BegBal = *begBal
	s.EndBal = *endBal
	return nil
}
