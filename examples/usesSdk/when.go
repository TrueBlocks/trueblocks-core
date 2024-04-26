package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoWhen tests the When sdk function
func DoWhen() {
	logger.Info("DoWhen")

	opts := sdk.WhenOptions{
		Globals: sdk.Globals{
			Verbose: true,
			Cache:   true,
		},
		BlockIds: testBlocks,
	}

	buf := bytes.Buffer{}
	if err := opts.WhenBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/when.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *WhenOptions) When() ([]types.NamedBlock, *types.MetaData, error) {
// func (opts *WhenOptions) WhenTimestamps() ([]types.Timestamp, *types.MetaData, error) {
// func (opts *WhenOptions) WhenCount() ([]types.TimestampCount, *types.MetaData, error) {
