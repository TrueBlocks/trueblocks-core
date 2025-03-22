package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type BalanceOptions struct {
	Asset       base.Address
	Holder      base.Address
	BlockNumber base.Blknum
}

func (r *Reconciler) GetReconBalances(opts *BalanceOptions, balances map[BalanceOptions]*base.Wei) (*base.Wei, *base.Wei, error) {
	if opts.BlockNumber == 0 {
		endBal, err := r.Connection.GetBalanceAtToken(opts.Asset, opts.Holder, opts.BlockNumber)
		return base.ZeroWei, endBal, err
	}

	endBal, err := r.Connection.GetBalanceAtToken(opts.Asset, opts.Holder, opts.BlockNumber)
	if err != nil {
		return base.ZeroWei, base.ZeroWei, err
	}

	begBal, err := r.Connection.GetBalanceAtToken(opts.Asset, opts.Holder, opts.BlockNumber-1)
	return begBal, endBal, err
}
