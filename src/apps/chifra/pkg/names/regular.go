package names

import (
	"io"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, thePath string, terms []string, parts Parts, ret *map[types.Address]types.SimpleName) error {
	// TODO: This should use gocsv instead of the custom code below
	reader, err := NewNameReader(thePath)
	if err != nil {
		log.Fatal(err)
	}

	for {
		n, err := reader.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}
		if doSearch(&n, terms, parts) {
			(*ret)[n.Address] = n
		}
	}

	return nil
}
