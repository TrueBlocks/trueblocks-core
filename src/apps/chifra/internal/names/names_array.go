package namesPkg

import (
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// loadNamesArray loads the names from the cache and returns an array of names
func loadNamesArray(chain string, parts types.Parts, sortBy types.SortBy, terms []string) ([]types.Name, error) {
	var ret []types.Name
	if namesMap, err := names.LoadNamesMap(chain, parts, terms); err != nil {
		return nil, err
	} else {
		for _, name := range namesMap {
			// Custom names with Individual tag or tags under 30 are private during testing
			isTesting := parts&types.Testing != 0
			isPrivate := strings.Contains(name.Tags, "Individual") || (name.IsCustom && name.Tags < "3")
			if !isTesting || !isPrivate {
				ret = append(ret, name)
			}
		}
	}

	sort.Slice(ret, func(i, j int) bool {
		switch sortBy {
		case types.SortByTags:
			return ret[i].Tags < ret[j].Tags
		case types.SortByAddress:
			fallthrough
		default:
			return ret[i].Address.Hex() < ret[j].Address.Hex()
		}
	})

	isTesting := parts&types.Testing != 0
	isTags := sortBy == types.SortByTags
	if isTesting && !isTags {
		ret = ret[:min(200, len(ret))]
	}

	return ret, nil
}
