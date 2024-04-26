package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoSlurp tests the When sdk function
func DoSlurp() {
	logger.Info("DoSlurp")

	opts := sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}

	buf := bytes.Buffer{}
	if err := opts.SlurpBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/slurp.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *SlurpOptions) Slurp() ([]types.Slurp, *types.MetaData, error) {
// func (opts *SlurpOptions) SlurpAppearances() ([]types.Appearance, *types.MetaData, error) {
// func (opts *SlurpOptions) SlurpCount() ([]types.SlurpCount, *types.MetaData, error) {
