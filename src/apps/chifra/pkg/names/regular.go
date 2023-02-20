package names

import (
	"io"
	"log"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/gocarina/gocsv"
)

// loadRegularMap loads the regular names from the cache
func loadRegularMap(chain string, thePath string, terms []string, parts Parts, ret *map[types.Address]Name) error {
	callbackFunc := func(n Name) error {
		if doSearch(&n, terms, parts) {
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

func nameMapFromFile(chain string, thePath string, terms []string, parts Parts, ret *map[types.Address]Name) {
	gr, err := NewNameReader(thePath)
	if err != nil {
		log.Fatal(err)
	}

	for {
		n, err := gr.Read()
		if err == io.EOF {
			break
		}
		if err != nil {
			log.Fatal(err)
		}
		if doSearch(&n, terms, parts) {
			(*ret)[types.HexToAddress(n.Address)] = n
		}
	}
}
