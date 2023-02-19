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

type Allocation struct {
	Address types.Address `json:"address" csv:"address"`
	Balance big.Int       `json:"balance" csv:"balance"`
}

// We want to return at least one valid record
var emptyAllocs = []Allocation{{Address: types.HexToAddress("0x0"), Balance: *big.NewInt(0)}}

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

func loadPrefundMap(chain string, ret *NamesMap, terms []string, parts Parts) {
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
