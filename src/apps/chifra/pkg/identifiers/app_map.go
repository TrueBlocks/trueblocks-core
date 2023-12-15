package identifiers

import (
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func AsSliceOfMaps[T types.MappedType](chain string, ids []Identifier) ([]map[types.SimpleAppearance]*T, int, error) {
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
		// TODO: Do we want this to be configurable? Maybe, maybe not
		if len(curMap) == types.AppMapSize {
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
