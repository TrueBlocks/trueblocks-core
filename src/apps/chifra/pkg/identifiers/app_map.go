package identifiers

import (
	"sort"
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

func AsMap2[T mappedType](chain string, ids []Identifier) ([]map[types.SimpleAppearance]*T, int, error) {
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

	sort.Slice(ret, func(i, j int) bool {
		if ret[i].BlockNumber == ret[j].BlockNumber {
			return ret[i].TransactionIndex < ret[j].TransactionIndex
		}
		return ret[i].BlockNumber < ret[j].BlockNumber
	})

	arrayOfMaps := make([]map[types.SimpleAppearance]*T, 0, len(ret))
	curMap := make(map[types.SimpleAppearance]*T)
	for i := 0; i < len(ret); i++ {
		if len(curMap) == 10 {
			arrayOfMaps = append(arrayOfMaps, curMap)
			curMap = make(map[types.SimpleAppearance]*T)
		}
		curMap[ret[i]] = nil
	}

	if len(curMap) > 0 {
		arrayOfMaps = append(arrayOfMaps, curMap)
	}

	return arrayOfMaps, len(ret), nil
}
