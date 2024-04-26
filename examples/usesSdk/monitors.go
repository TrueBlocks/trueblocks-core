package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// DoMonitors tests the When sdk function
func DoMonitors() {
	logger.Info("DoMonitors")

	opts := sdk.MonitorsOptions{
		Addrs: []string{testAddrs[0]},
		List:  true,
	}

	buf := bytes.Buffer{}
	if err := opts.MonitorsBytes(&buf); err != nil {
		logger.Fatal(err)
	}

	file.StringToAsciiFile("usesSDK/monitors.json", buf.String())
	fmt.Println(buf.String())
}
// func (opts *MonitorsOptions) Monitors() ([]bool, *types.MetaData, error) {
// func (opts *MonitorsOptions) MonitorsClean() ([]types.MonitorClean, *types.MetaData, error) {
// func (opts *MonitorsOptions) MonitorsList() ([]types.Monitor, *types.MetaData, error) {
