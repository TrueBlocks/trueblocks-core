package identifiers

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func IdsToApps(chain string, ids []Identifier) ([]types.Appearance, int, error) {
	ret := make([]types.Appearance, 0, 100 /* good guess */)
	for index, rng := range ids {
		if apps, err := rng.ResolveTxs(chain); err != nil {
			if blockIds, err := rng.ResolveBlocks(chain); err != nil {
				return nil, 0, err
			} else {
				for _, bn := range blockIds {
					s := types.Appearance{
						BlockNumber: uint32(bn),
						Reason:      strings.ReplaceAll(ids[index].Orig, "-", "."),
					}
					ret = append(ret, s)
				}
			}
		} else {
			for _, app := range apps {
				s := types.Appearance{
					BlockNumber:      uint32(app.BlockNumber),
					TransactionIndex: uint32(app.TransactionIndex),
					Reason:           strings.ReplaceAll(ids[index].Orig, "-", "."),
				}
				ret = append(ret, s)
			}
		}
	}
	return ret, len(ret), nil
}
