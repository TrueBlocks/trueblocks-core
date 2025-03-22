package ledger

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) GetAssets(txs []*types.Transaction) ([]*types.Name, bool, error) {
	ethTransfers, tokenTransfers, _, err := r.getTransfersInternal(txs)
	if err != nil {
		return nil, false, err
	}

	transfers := append(ethTransfers, tokenTransfers...)

	finished := false
	slice := make([]*types.Name, 0, len(transfers))
	for _, item := range transfers {
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
			slice = append(slice, &name)
		}
	}

	sort.Slice(slice, func(i, j int) bool {
		return slice[i].Address.LessThan(slice[j].Address)
	})

	results := make([]*types.Name, 0, len(slice))
	for _, item := range slice {
		var passes bool
		passes, finished = r.Opts.AppFilters.ApplyCountFilter()
		if passes {
			results = append(results, item)
		}
		if finished {
			break
		}
	}

	return results, finished, nil
}
