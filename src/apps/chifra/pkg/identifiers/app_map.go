package identifiers

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func IdsToApps(chain string, ids []Identifier) ([]types.SimpleAppearance, int, error) {
	ret := make([]types.SimpleAppearance, 0, 100 /* good guess */)
	for index, rng := range ids {
		if rawIds, err := rng.ResolveTxs(chain); err != nil {
			if blockIds, err := rng.ResolveBlocks(chain); err != nil {
				return nil, 0, err
			} else {
				for _, raw := range blockIds {
					s := types.SimpleAppearance{
						BlockNumber: uint32(raw),
						Reason:      strings.Replace(ids[index].Orig, "-", ".", -1),
					}
					ret = append(ret, s)
				}
			}
		} else {
			for _, raw := range rawIds {
				s := types.SimpleAppearance{
					BlockNumber:      uint32(raw.BlockNumber),
					TransactionIndex: uint32(raw.TransactionIndex),
					Reason:           strings.Replace(ids[index].Orig, "-", ".", -1),
				}
				ret = append(ret, s)
			}
		}
	}
	return ret, len(ret), nil
}
