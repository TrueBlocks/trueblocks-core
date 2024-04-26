package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoLogs tests the When sdk function
func DoLogs() {
	logger.Info("DoLogs")

	opts := sdk.LogsOptions{
		TransactionIds: []string{"10001002.0"},
	}

	buf := bytes.Buffer{}
	if err := opts.LogsBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/logs.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *LogsOptions) Logs() ([]types.Log, *types.MetaData, error) {
