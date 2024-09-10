package main

import (
	"fmt"
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
)

func publishNames() {
	filePath := filepath.Join(config.MustGetPathToChainConfig("mainnet"), string(names.DatabaseCustom))
	if os.Getenv("TB_NAMEMANAGER_REGULAR") == "true" {
		filePath = filepath.Join(config.MustGetPathToChainConfig("mainnet"), string(names.DatabaseRegular))
	}
	fmt.Println("Would have published the database: ", filePath)
}
