// TODO: This can be auto generated
package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
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
