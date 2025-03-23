package ledger

import (
	"errors"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

type BalanceOptions struct {
	Asset       base.Address
	Holder      base.Address
	BlockNumber base.Blknum
}

func (r *Reconciler) GetReconBalances(opts *BalanceOptions, balances map[BalanceOptions]*base.Wei) (*base.Wei, *base.Wei, error) {
	var ok bool
	var begBal, endBal *base.Wei
	if opts.BlockNumber == 0 {
		if endBal, ok = balances[*opts]; !ok {
			// logger.TestLog(true, "no endBal found for", opts.BlockNumber)
			return base.ZeroWei, base.ZeroWei, errors.New("error fetching endBal")
		} else {
			return base.ZeroWei, endBal, nil
		}
	}

	if endBal, ok = balances[*opts]; !ok {
		// logger.TestLog(true, "error fetching endBal")
		return base.ZeroWei, base.ZeroWei, errors.New("error fetching endBal")
	}

	o := *opts
	o.BlockNumber = base.Blknum(int(o.BlockNumber) - 1)
	if begBal, ok = balances[o]; !ok {
		// logger.TestLog(true, "error fetching begBal")
		return base.ZeroWei, base.ZeroWei, errors.New("error fetching begBal")
	}

	return begBal, endBal, nil
}
