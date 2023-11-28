package identifiers

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type mappedType interface {
	types.SimpleTransaction |
		types.SimpleBlock[string] |
		types.SimpleBlock[types.SimpleTransaction] |
		types.SimpleAppearance |
		types.SimpleWithdrawal |
		types.SimpleResult
}

// AsMap takes command line identifiers for blocks or transactions and returns a map of appearances to allocated
// pointers to SimpleTransactions or SimpleBlock[string]. The map is keyed by the appearance and the value is
// the allocated pointer. We don't know what type of identifier we have until we try to resolve it.
func AsMap[T mappedType](chain string, ids []Identifier) (map[types.SimpleAppearance]*T, int, error) {
	ret := make(map[types.SimpleAppearance]*T)
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
					ret[s] = new(T)
				}
			}
		} else {
			for _, raw := range rawIds {
				s := types.SimpleAppearance{
					BlockNumber:      uint32(raw.BlockNumber),
					TransactionIndex: uint32(raw.TransactionIndex),
					Reason:           strings.Replace(ids[index].Orig, "-", ".", -1),
				}
				ret[s] = new(T)
			}
		}
	}

	return ret, len(ret), nil
}
