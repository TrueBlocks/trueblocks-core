package identifiers

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type ResolvedId struct {
	BlockNumber      uint64
	TransactionIndex uint64
	Original         string
}

func (r *ResolvedId) String() string {
	return r.Original
}

type mappedType interface {
	types.SimpleTransaction |
		types.SimpleBlock[string] |
		types.SimpleBlock[types.SimpleTransaction] |
		types.SimpleAppearance |
		types.SimpleResult
}

// AsMap takes command line identifiers for blocks or transactions and returns a map of appearances to allocated
// pointers to SimpleTransactions or SimpleBlock[string]. The map is keyed by the appearance and the value is
// the allocated pointer. We don't know what type of identifier we have until we try to resolve it.
func AsMap[T mappedType](chain string, ids []Identifier) (map[ResolvedId]*T, int, error) {
	ret := make(map[ResolvedId]*T)
	for index, rng := range ids {
		if rawIds, err := rng.ResolveTxs(chain); err != nil {
			if blockIds, err := rng.ResolveBlocks(chain); err != nil {
				return nil, 0, err
			} else {
				for _, raw := range blockIds {
					s := ResolvedId{
						BlockNumber: uint64(raw),
						Original:    strings.Replace(ids[index].Orig, "-", ".", -1),
					}
					ret[s] = new(T)
				}
			}
		} else {
			for _, raw := range rawIds {
				s := ResolvedId{
					BlockNumber:      uint64(raw.BlockNumber),
					TransactionIndex: uint64(raw.TransactionIndex),
					Original:         strings.Replace(ids[index].Orig, "-", ".", -1),
				}
				ret[s] = new(T)
			}
		}
	}

	return ret, len(ret), nil
}
