package identifiers

import "strings"

type ResolvedId struct {
	BlockNumber      uint64
	TransactionIndex uint64
	Original         string
}

func (r *ResolvedId) String() string {
	return r.Original
}

func AsMap[T any](chain string, ids []Identifier) (map[ResolvedId]*T, error) {
	ret := make(map[ResolvedId]*T)
	for index, rng := range ids {
		if rawIds, err := rng.ResolveTxs(chain); err != nil {
			return nil, err
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
	return ret, nil
}
