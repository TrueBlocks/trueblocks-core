package names

import (
	"io"
	"os"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var loadedRegularNames = map[base.Address]types.Name{}
var loadedRegularNamesMutex sync.Mutex

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, terms []string, parts Parts, ret *map[base.Address]types.Name) error {
	if len(loadedRegularNames) != 0 {
		// We have already loaded the data
		for _, name := range loadedRegularNames {
			if doSearch(&name, terms, parts) {
				(*ret)[name.Address] = name
			}
		}
		return nil
	}

	loadedRegularNamesMutex.Lock()
	defer loadedRegularNamesMutex.Unlock()

	db, err := openDatabaseFile(chain, DatabaseRegular, os.O_RDONLY)
	if err != nil {
		return err
	}
	defer db.Close()

	reader, err := NewNameReader(db)
	if err != nil {
		return err
	}

	for {
		n, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			logger.Fatal(err)
		}
		loadedRegularNames[n.Address] = n
		if doSearch(&n, terms, parts) {
			(*ret)[n.Address] = n
		}
	}

	if parts&Baddress != 0 {
		loadKnownBadresses(terms, parts, ret)
	}

	return nil
}

// loadKnownBadresses loads the known bad addresses from the cache
func loadKnownBadresses(terms []string, parts Parts, ret *map[base.Address]types.Name) {
	knownBadAddresses := []types.Name{
		{
			Address: base.PrefundSender,
			Name:    "PrefundSender",
			Tags:    "75-Baddress",
		},
		{
			Address: base.BlockRewardSender,
			Name:    "BlockRewardSender",
			Tags:    "75-Baddress",
		},
		{
			Address: base.UncleRewardSender,
			Name:    "UncleRewardSender",
			Tags:    "75-Baddress",
		},
		{
			Address: base.WithdrawalSender,
			Name:    "WithdrawalSender",
			Tags:    "75-Baddress",
		},
	}
	for _, n := range knownBadAddresses {
		if doSearch(&n, terms, parts) {
			(*ret)[n.Address] = n
		}
	}
}
