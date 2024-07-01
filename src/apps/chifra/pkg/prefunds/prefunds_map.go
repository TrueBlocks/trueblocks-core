package prefunds

import (
	"fmt"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var prefundNamesLoaded = false
var prefundNames = map[base.Address]types.Name{}
var prefundNamesMutex sync.Mutex

// LoadPrefundMap loads the prefund names from file if not already loaded or from the cache
// if it is. A pointer to the data is returned, so the caller should not modify it.
func LoadPrefundMap(chain string, thePath string) (*map[base.Address]types.Name, error) {
	if prefundNamesLoaded {
		return &prefundNames, nil
	}

	prefundNamesMutex.Lock()
	defer func() {
		prefundNamesLoaded = true
		prefundNamesMutex.Unlock()
	}()

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
			prefundNames[n.Address] = n
		}
	}

	return &prefundNames, nil
}
