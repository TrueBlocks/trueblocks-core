package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// getUnreconciledTransfers returns a list of transfers from a given transaction
func (r *Reconciler) getUnreconciledTransfers(trans *types.Transaction) ([]*types.Transfer, error) {
	var xfrs []*types.Transfer
	if types.IsAssetOfInterest(base.FAKE_ETH_ADDRESS, r.Opts.AssetFilters) {
		var xfr *types.Transfer
		if r.Opts.UseTraces {
			if traces, err := r.Connection.GetTracesByTransactionHash(trans.Hash.Hex(), trans); err != nil {
				return nil, err
			} else {
				var err error
				if xfr, err = trans.FetchTransferTraces(traces, r.Opts.AccountFor); err != nil {
					return nil, err
				}
			}
		} else {
			var err error
			if xfr, err = trans.FetchTransfer(r.Opts.AccountFor); err != nil {
				return nil, err
			}
		}
		// Append only if the statement is material
		if xfr.IsMaterial() {
			xfrs = append(xfrs, xfr)
		}
	}

	if trans.Receipt != nil {
		if logXfrs, err := trans.Receipt.FetchTransfers(r.Opts.AccountFor, r.Opts.AssetFilters, r.Opts.AppFilters); err != nil {
			return nil, err
		} else {
			for _, logXfr := range logXfrs {
				logXfr.Transaction = trans
				if logXfr.IsMaterial() {
					xfrs = append(xfrs, logXfr)
				}
			}
		}
	}

	return xfrs, nil
}

func (r *Reconciler) ConvertToStatement(xfr *types.Transfer, trans *types.Transaction) *types.Statement {
	sym := "WEI"
	if r.Opts.AsEther {
		sym = "ETH"
	}
	stmt := xfr.ToStatement(trans, xfr.Holder, sym)
	return stmt
}
