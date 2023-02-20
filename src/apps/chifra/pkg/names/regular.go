package names

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/gocarina/gocsv"
)

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, ret *map[types.Address]Name, thePath string, terms []string, parts Parts) error {
	callbackFunc := func(n Name) error {
		if doSearch(n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
		return nil
	}

	if theFile, err := os.OpenFile(thePath, os.O_RDWR|os.O_CREATE, os.ModePerm); err != nil {
		return err
	} else {
		defer theFile.Close()
		if err := gocsv.UnmarshalToCallback(theFile, callbackFunc); err != nil {
			return err
		}
	}

	return nil
}
