package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

func (t *TestCase) SdkTest() {
	switch t.Route {
	case "list":
		lo, err := sdk.GetListOpts(t.Options)
		logger.Info("\tlo", lo)
		if err != nil {
			logger.Error(err)
			return
		}
		var buf *bytes.Buffer
		if err := lo.List(buf); err != nil {
			logger.Error(err)
			return
		}
		fmt.Println(buf.String())
	case "export":
		fallthrough
	case "monitors":
		fallthrough
	case "names":
		fallthrough
	case "abis":
		fallthrough
	case "blocks":
		fallthrough
	case "transactions":
		fallthrough
	case "receipts":
		fallthrough
	case "logs":
		fallthrough
	case "traces":
		fallthrough
	case "when":
		fallthrough
	case "state":
		fallthrough
	case "tokens":
		fallthrough
	case "config":
		fallthrough
	case "status":
		fallthrough
	case "daemon":
		fallthrough
	case "scrape":
		fallthrough
	case "chunks":
		fallthrough
	case "init":
		fallthrough
	case "explore":
		fallthrough
	case "slurp":
		logger.Info(*t)
	}
}
