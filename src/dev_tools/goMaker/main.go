package main

import (
	"github.com/TrueBlocks/trueblocks-core/goMaker/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

func main() {
	codeBase, err := types.LoadCodebase()
	if err != nil {
		logger.Fatal(err)
	}
	codeBase.Generate()
}
