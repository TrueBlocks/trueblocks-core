package ledger

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) GetAssets(txs []*types.Transaction) ([]*types.Name, bool, error) {
	ethTransfers, tokenTransfers, err := r.getTransfersInternal(txs)
	if err != nil {
		return nil, false, err
	}

	transfers := append(ethTransfers, tokenTransfers...)
	sort.Slice(transfers, func(i, j int) bool {
		if r.Opts.Reversed {
			i, j = j, i
		}
		return transfers[i].Asset.LessThan(transfers[j].Asset)
	})

	finished := false
	newAssets := make([]*types.Name, 0, 50)
	for _, item := range transfers {
		var passes bool
		passes, finished = r.Opts.AppFilters.ApplyCountFilter()
		if passes {
			key := NewAssetHolderKey(item.Asset, r.Opts.AccountFor)
			if _, ok := r.AssetMap[key]; !ok {
				var name types.Name
				if name, ok = r.Names[item.Asset]; !ok {
					name = types.Name{
						Address:  item.Asset,
						Name:     item.Asset.Display(3, 3),
						Decimals: 18,
					}
				}
				r.AssetMap[key] = &name
				newAssets = append(newAssets, &name)
			}
		}
		if finished {
			break
		}
	}

	sort.Slice(newAssets, func(i, j int) bool {
		if r.Opts.Reversed {
			i, j = j, i
		}
		return newAssets[i].Address.LessThan(newAssets[j].Address)
	})

	return newAssets, finished, nil
}
