package names

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/gocarina/gocsv"
)

// loadCustomMap loads the custom names from the cache
func loadCustomMap(chain string, thePath string, terms []string, parts Parts, ret *map[types.Address]Name) error {
	callbackFunc := func(n Name) error {
		if doSearch(n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
		return nil
	}

	if parts&Testing == 0 {
		if theFile, err := os.OpenFile(thePath, os.O_RDWR|os.O_CREATE, os.ModePerm); err != nil {
			return err
		} else {
			defer theFile.Close()
			if err := gocsv.UnmarshalToCallback(theFile, callbackFunc); err != nil {
				return err
			}
		}
	} else {
		for i := 1; i < 5; i++ {
			addr := fmt.Sprintf("0x%040d", i)
			num := fmt.Sprintf("%d", i)
			n := Name{
				Address:  addr,
				Name:     "Account_" + num,
				Tags:     "81-Custom",
				Decimals: num,
				Symbol:   "SYM_" + num,
				Source:   "Testing",
				Petname:  AddrToPetname(addr, "-"),
				IsCustom: true,
			}
			callbackFunc(n)
		}
	}

	return nil
}
