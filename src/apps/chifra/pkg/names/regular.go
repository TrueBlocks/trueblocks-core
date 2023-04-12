package names

import (
	"io"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, thePath string, terms []string, parts Parts, ret *map[base.Address]types.SimpleName) error {
	db, err := OpenDatabaseFile(chain, DatabaseRegular, os.O_RDONLY)
	if err != nil {
		return err
	}
	defer db.Close()
	// TODO: This should use gocsv instead of the custom code below
	reader, err := NewNameReader(db, NameReaderTab)
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
}
