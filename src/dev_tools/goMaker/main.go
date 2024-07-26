package main

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/goMaker/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func main() {
	codeBase, err := types.LoadCodebase()
	if err != nil {
		logger.Error(err)
		os.Exit(1)
	}
	codeBase.Generate()
}
