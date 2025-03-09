package rpc

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type BalanceOptions struct {
	PrevAppBlk base.Blknum  // the block of the previous appearance
	CurrBlk    base.Blknum  // the block for this appearance
	Asset      base.Address // the asset whose balance we need
	Holder     base.Address // the holder whose balance we need
}

func (c *Connection) GetReconBalances(opts *BalanceOptions) (base.Wei, base.Wei, base.Wei, error) {
	zero := *base.ZeroWei

	prevBal, err := c.GetBalanceAtToken(opts.Asset, opts.Holder, opts.PrevAppBlk)
	if err != nil {
		return zero, zero, zero, err
	}

	begBal, err := c.GetBalanceAtToken(opts.Asset, opts.Holder, opts.CurrBlk-1)
	if err != nil {
		return zero, zero, zero, err
	}

	endBal, err := c.GetBalanceAtToken(opts.Asset, opts.Holder, opts.CurrBlk)
	if err != nil {
		return zero, zero, zero, err
	}

	if opts.CurrBlk == 0 {
		prevBal = base.ZeroWei
		begBal = base.ZeroWei
	}

	return *prevBal, *begBal, *endBal, nil
}
