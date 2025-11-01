package names

import (
	"io"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

var regularNamesLoaded = false
var regularNames = map[base.Address]types.Name{}
var regularNamesMutex sync.Mutex

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, terms []string, parts types.Parts, ret *map[base.Address]types.Name) error {
	regularNamesMutex.Lock()
	defer func() {
		regularNamesLoaded = true
		regularNamesMutex.Unlock()
	}()

	if regularNamesLoaded {
		for _, name := range regularNames {
			if doSearch(&name, terms, parts) {
				name.Parts = types.Regular
				if existing, ok := (*ret)[name.Address]; ok {
					name.Parts |= existing.Parts
				}
				(*ret)[name.Address] = name
			}
		}
		return nil
	}

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
		name, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			return err
		}
		regularNames[name.Address] = name
		if doSearch(&name, terms, parts) {
			name.Parts = types.Regular
			if existing, ok := (*ret)[name.Address]; ok {
				name.Parts |= existing.Parts
			}
			(*ret)[name.Address] = name
		}
	}

	return nil
}

// loadKnownBadresses loads the known bad addresses from the cache
func loadKnownBadresses(unused string, terms []string, parts types.Parts, ret *map[base.Address]types.Name) error {
	_ = unused // linter
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
	for _, name := range knownBadAddresses {
		if doSearch(&name, terms, parts) {
			name.Parts = types.Baddress
			(*ret)[name.Address] = name
		}
	}
	return nil
}

func BaddressCount() int {
	m := make(map[base.Address]types.Name)
	_ = loadKnownBadresses("unused", []string{}, types.Baddress, &m)
	return len(m)
}
