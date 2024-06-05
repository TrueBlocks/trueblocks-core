package prefunds

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/gocarina/gocsv"
)

func GetPrefundPath(chain string) string {
	return filepath.Join(config.MustGetPathToChainConfig(chain), "allocs.csv")
}

// Allocation is a single allocation in the genesis file
type Allocation struct {
	Address base.Address `json:"address" csv:"address"`
	Balance base.Wei     `json:"balance" csv:"balance"`
}

// emptyAllocs is a list of empty allocations. We use this to return at least one allocation
var emptyAllocs = []Allocation{}

type allocCallback func(*Allocation, *any) (bool, error)

// TODO: In the c++ code, the prefunds were cached in memory. We should do the same here.

// LoadPrefunds loads the prefunds from the genesis file and processes each with provided callback if present
func LoadPrefunds(chain string, thePath string, userCallback allocCallback) ([]Allocation, error) {
	allocations := make([]Allocation, 0, 4000)
	callbackFunc := func(record Allocation) error {
		if base.IsValidAddress(record.Address.Hex()) {
			alloc := Allocation{
				Address: record.Address,
				Balance: record.Balance,
			}
			// fmt.Println(alloc)
			allocations = append(allocations, alloc)
			if userCallback != nil {
				if cont, err := userCallback(&alloc, nil); !cont || err != nil {
					return err
				}
			}
		}
		return nil
	}

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

func GetLargestPrefund(chain, thePath string) (Allocation, error) {
	largest := Allocation{}
	getLargest := func(alloc *Allocation, data *any) (bool, error) {
		if alloc.Balance.Cmp(&largest.Balance) > 0 {
			largest = *alloc
		}
		return true, nil
	}

	_, err := LoadPrefunds(chain, thePath, getLargest)
	if err != nil {
		return Allocation{}, err
	}

	return largest, nil
}
