package ledger

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) getTransfersInternal(txs []*types.Transaction, fetchBalances bool) ([]*types.Transfer, []*types.Transfer, map[BalanceOptions]*base.Wei, error) {
	var allEthTransfers []*types.Transfer
	var allTokenTransfers []*types.Transfer
	balances := make(map[BalanceOptions]*base.Wei)

	for _, tx := range txs {
		ethXfers, tokenXfers, err := r.getUnreconciledTransfers(tx)
		if err != nil {
			return nil, nil, nil, err
		}

		for _, xfer := range append(ethXfers, tokenXfers...) {
			bn := xfer.BlockNumber
			key := AssetHolder{Asset: xfer.Asset, Holder: xfer.Holder}

			if xfer.Asset == base.FAKE_ETH_ADDRESS {
				allEthTransfers = append(allEthTransfers, xfer)
			} else {
				allTokenTransfers = append(allTokenTransfers, xfer)
			}

			if fetchBalances {
				// Get balance for current block number if not already present
				ahbKey := BalanceOptions{Asset: key.Asset, Holder: key.Holder, BlockNumber: bn}
				if _, exists := balances[ahbKey]; !exists {
					balance, err := r.Connection.GetBalanceAtToken(key.Asset, key.Holder, bn)
					if err != nil {
						return nil, nil, nil, err
					}
					balances[ahbKey] = balance
				}

				// Get balance for previous block if bn > 0 and not already present
				if bn > 0 {
					prevBn := bn - 1
					prevAhbKey := BalanceOptions{Asset: key.Asset, Holder: key.Holder, BlockNumber: prevBn}
					if _, exists := balances[prevAhbKey]; !exists {
						balance, err := r.Connection.GetBalanceAtToken(key.Asset, key.Holder, prevBn)
						if err != nil {
							return nil, nil, nil, err
						}
						balances[prevAhbKey] = balance
					}
				}
			}
		}
	}

	return allEthTransfers, allTokenTransfers, balances, nil
}
