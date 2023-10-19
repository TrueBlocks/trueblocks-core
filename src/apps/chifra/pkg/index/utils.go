package index

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

func IsInitialized(chain, expectedTag string, unused bool /* unused */) error {
	fileName := config.PathToIndex(chain) + "finalized/000000000-000000000.bin"
	if !file.FileExists(fileName) {
		const indexNotInitialized string = `

	  The Unchained Index does not appear to be initialized. You must run 'chifra init'
	  (and allow it to complete) or 'chifra scrape' before using this command.
	  
	  Path: [{PATH}]

	  [{VERSION}]

	`
		msg := strings.Replace(indexNotInitialized, "{0}", "{v0.40.0-beta}", -1)
		msg = strings.Replace(msg, "[{VERSION}]", version.LibraryVersion, -1)
		msg = strings.Replace(msg, "[{PATH}]", fileName, -1)
		msg = strings.Replace(msg, "{", colors.Green, -1)
		msg = strings.Replace(msg, "}", colors.Off, -1)
		return fmt.Errorf(msg)
	}

	var err error
	var idx Index
	if idx.File, err = os.OpenFile(fileName, os.O_RDONLY, 0); err != nil {
		return err

	} else {
		defer idx.File.Close()
		if _, err = idx.ReadHeader(expectedTag, false /* unused */); err != nil {
			msg := fmt.Sprintf("%s: %s", err, fileName)
			logger.Fatal(msg)
		}

		return nil
	}
}
