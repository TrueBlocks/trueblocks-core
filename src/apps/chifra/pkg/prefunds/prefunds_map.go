package prefunds

import (
	"fmt"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var loadedPrefundNames map[base.Address]types.Name = map[base.Address]types.Name{}
var loadedPrefundNamesMutex sync.Mutex

// LoadPrefundMap loads the prefund names from file if not already loaded or from the cache
// if it is. A pointer to the data is returned, so the caller should not modify it.
func LoadPrefundMap(chain string, thePath string) (*map[base.Address]types.Name, error) {
	if len(loadedPrefundNames) != 0 {
		return &loadedPrefundNames, nil
	}

	loadedPrefundNamesMutex.Lock()
	defer loadedPrefundNamesMutex.Unlock()

	if prefunds, err := LoadPrefunds(chain, thePath, nil); err != nil {
		return nil, err
	} else {
		for i, prefund := range prefunds {
			n := types.Name{
				Tags:      "80-Prefund",
				Address:   prefund.Address,
				Name:      "Prefund_" + fmt.Sprintf("%04d", i),
				Source:    "Genesis",
				IsPrefund: true,
				Prefund:   prefund.Balance,
			}
			loadedPrefundNames[n.Address] = n
		}
	}

	return &loadedPrefundNames, nil
}
