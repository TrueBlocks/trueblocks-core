package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getUnreconciledTransfers returns a list of transfers from a given transaction
func (r *Reconciler) getUnreconciledTransfers(trans *types.Transaction) ([]*types.Transfer, []*types.Transfer, error) {
	var ethTransfers, tokenTransfers []*types.Transfer
	if types.IsAssetOfInterest(base.FAKE_ETH_ADDRESS, r.Opts.AssetFilters) {
		var xfr *types.Transfer
		if r.Opts.UseTraces {
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				return nil, nil, err
			} else {
				var err error
				if xfr, err = trans.TracesToTransfer(traces, r.Opts.AccountFor); err != nil {
					return nil, nil, err
				}
			}
		} else {
			var err error
			if xfr, err = trans.ToTransfer(r.Opts.AccountFor); err != nil {
				return nil, nil, err
			}
		}
		// Append only if the statement is material
		if xfr.IsMaterial() {
			ethTransfers = append(ethTransfers, xfr)
		}
	}

	if trans.Receipt != nil {
		if logXfrs, err := trans.Receipt.ToTranfers(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, nil, err
		} else {
			for _, logXfr := range logXfrs {
				logXfr.Transaction = trans
				if logXfr.IsMaterial() {
					tokenTransfers = append(tokenTransfers, logXfr)
				}
			}
		}
	}

	return ethTransfers, tokenTransfers, nil
}
