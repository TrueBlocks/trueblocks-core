package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoInit tests the When sdk function
func DoInit() {
	logger.Info("DoInit")

	opts := sdk.InitOptions{
		All: true,
	}

	buf := bytes.Buffer{}
	if err := opts.InitBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/init.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *InitOptions) InitAll() ([]bool, *types.MetaData, error) {
