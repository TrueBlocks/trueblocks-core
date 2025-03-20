package ledger

import (
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) GetAssets(txs []*types.Transaction) ([]*types.Name, bool, error) {
	assetMap := make(map[base.Address]types.Name)

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
	slice := make([]*types.Name, 0, len(assetMap))
	for _, item := range transfers {
		var passes bool
		passes, finished = r.Opts.AppFilters.ApplyCountFilter()
		if passes {
			if _, ok := assetMap[item.Asset]; !ok {
				if name, ok := r.Names[item.Asset]; !ok {
					name = types.Name{
						Address:  item.Asset,
						Name:     item.Asset.Display(3, 3),
						Decimals: 18,
					}
					assetMap[item.Asset] = name
				} else {
					assetMap[item.Asset] = name
				}
			}
		}
		if finished {
			break
		}
	}

	for _, name := range assetMap {
		slice = append(slice, &name)
	}
	sort.Slice(slice, func(i, j int) bool {
		if r.Opts.Reversed {
			i, j = j, i
		}
		return slice[i].Address.LessThan(slice[j].Address)
	})

	return slice, finished, nil
}
