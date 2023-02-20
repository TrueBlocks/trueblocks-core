package names

import (
	"fmt"
	"math/big"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/gocarina/gocsv"
)

// loadPrefundMap loads the prefund names from the cache
func loadPrefundMap(chain string, ret *map[types.Address]Name, terms []string, parts Parts) {
	prefunds, _ := LoadPrefunds(chain)
	for i, prefund := range prefunds {
		n := Name{
			Tags:      "80-Prefund",
			Address:   prefund.Address.Hex(),
			Name:      "Prefund_" + fmt.Sprintf("%04d", i),
			Source:    "Genesis",
			Petname:   AddrToPetname(prefund.Address.Hex(), "-"),
			IsPrefund: true,
		}
		if doSearch(n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
	}
}

// Allocation is a single allocation in the genesis file
type Allocation struct {
	Address types.Address `json:"address" csv:"address"`
	Balance big.Int       `json:"balance" csv:"balance"`
}

// emptyAllocs is a list of empty allocations. We use this to return at least one allocation
var emptyAllocs = []Allocation{{Address: types.HexToAddress("0x0"), Balance: *big.NewInt(0)}}

// LoadPrefunds loads the prefunds from the genesis file
func LoadPrefunds(chain string) ([]Allocation, error) {
	allocations := make([]Allocation, 0, 4000)
	callbackFunc := func(record Allocation) error {
		if validate.IsValidAddress(record.Address.Hex()) {
			allocations = append(allocations, Allocation{
				Address: record.Address,
				Balance: record.Balance,
			})
		}
		return nil
	}

	thePath := config.GetPathToChainConfig(chain) + "allocs.csv"
	if theFile, err := os.OpenFile(thePath, os.O_RDWR|os.O_CREATE, os.ModePerm); err != nil {
		return emptyAllocs, err

	} else {
		defer theFile.Close()
		if err := gocsv.UnmarshalToCallback(theFile, callbackFunc); err != nil {
			return emptyAllocs, err
		}
	}

	if len(allocations) == 0 {
		allocations = emptyAllocs
	}

	return allocations, nil
}
