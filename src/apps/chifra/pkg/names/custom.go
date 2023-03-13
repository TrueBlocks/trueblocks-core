package names

import (
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// loadCustomMap loads the custom names from the cache
func loadCustomMap(chain string, thePath string, terms []string, parts Parts, ret *map[types.Address]types.SimpleName) error {
	if parts&Testing == 0 {
		// TODO: This should use gocsv instead of the custom code below
		reader, err := NewNameReader(thePath)
		if err != nil {
			logger.Fatal(err)
		}

		for {
			n, err := reader.Read()
			if err == io.EOF {
				break
			}
			if err != nil {
				logger.Fatal(err)
			}
			if doSearch(&n, terms, parts) {
				(*ret)[n.Address] = n
			}
		}

		return nil
	} else {
		for i := 1; i < 5; i++ {
			addr := fmt.Sprintf("0x%040d", i)
			num := fmt.Sprintf("%d", i)
			n := types.SimpleName{
				Address:  types.HexToAddress(addr),
				Name:     "Account_" + num,
				Tags:     "81-Custom",
				Decimals: uint64(i),
				Symbol:   "SYM_" + num,
				Source:   "Testing",
				Petname:  AddrToPetname(addr, "-"),
				IsCustom: true,
			}
			if doSearch(&n, terms, parts) {
				(*ret)[n.Address] = n
			}
		}
	}

	return nil
}
