package ledger

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
)

// -----------------------------------------------------------------
// balanceOptions defines the parameters for fetching balances, including the asset, holder, and block number.
type balanceOptions struct {
	Asset       base.Address
	Holder      base.Address
	BlockNumber base.Blknum
}

// -----------------------------------------------------------------
func (r *Reconciler) getReconBalances(opts *balanceOptions, balances map[balanceOptions]*base.Wei) (*base.Wei, *base.Wei, error) {
	var ok bool
	var begBal, endBal *base.Wei
	if opts.BlockNumber == 0 {
		if endBal, ok = balances[*opts]; !ok {
			return base.ZeroWei, base.ZeroWei, errors.New("error fetching endBal")
		} else {
			return base.ZeroWei, endBal, nil
		}
	}

	if endBal, ok = balances[*opts]; !ok {
		return base.ZeroWei, base.ZeroWei, errors.New("error fetching endBal")
	}

	o := *opts
	o.BlockNumber = base.Blknum(int(o.BlockNumber) - 1)
	if begBal, ok = balances[o]; !ok {
		return base.ZeroWei, base.ZeroWei, errors.New("error fetching begBal")
	}

	return begBal, endBal, nil
}
