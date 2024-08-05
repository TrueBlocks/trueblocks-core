package names

import (
	"io"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var regularNamesLoaded = false
var regularNames = map[base.Address]types.Name{}
var regularNamesMutex sync.Mutex

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, terms []string, parts types.Parts, ret *map[base.Address]types.Name) error {
	if regularNamesLoaded {
		for _, name := range regularNames {
			if doSearch(&name, terms, parts) {
				(*ret)[name.Address] = name
			}
		}
		return nil
	}

	regularNamesMutex.Lock()
	defer func() {
		regularNamesLoaded = true
		regularNamesMutex.Unlock()
	}()

	db, err := openDatabaseForRead(chain, DatabaseRegular)
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
		regularNames[n.Address] = n
		if doSearch(&n, terms, parts) {
			(*ret)[n.Address] = n
		}
	}

	return nil
}

// loadKnownBadresses loads the known bad addresses from the cache
func loadKnownBadresses(unused string, terms []string, parts types.Parts, ret *map[base.Address]types.Name) error {
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
	return nil
}
