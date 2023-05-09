package names

import (
	"fmt"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/prefunds"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var loadedPrefundNames map[base.Address]types.SimpleName = map[base.Address]types.SimpleName{}
var loadedPrefundNamesMutex sync.Mutex

// loadPrefundMap loads the prefund names from the cache
func loadPrefundMap(chain string, thePath string, terms []string, parts Parts, ret *map[base.Address]types.SimpleName) {
	if len(loadedPrefundNames) != 0 {
		// We have already loaded the data
		for _, name := range loadedPrefundNames {
			if doSearch(&name, terms, parts) {
				(*ret)[name.Address] = name
			}
		}
		return
	}
	loadedPrefundNamesMutex.Lock()
	defer loadedPrefundNamesMutex.Unlock()

	prefunds, _ := prefunds.LoadPrefunds(chain, thePath, nil)
	for i, prefund := range prefunds {
		n := types.SimpleName{
			Tags:      "80-Prefund",
			Address:   prefund.Address,
			Name:      "Prefund_" + fmt.Sprintf("%04d", i),
			Source:    "Genesis",
			Petname:   AddrToPetname(prefund.Address.Hex(), "-"),
			IsPrefund: true,
		}
		loadedPrefundNames[n.Address] = n
		if doSearch(&n, terms, parts) {
			(*ret)[n.Address] = n
		}
	}
}
