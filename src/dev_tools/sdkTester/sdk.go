package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// SdkTest runs a test case through the SDK and returns the results in the provided bytes.Buffer
func (t *TestCase) SdkTest(buf *bytes.Buffer) error {
	logger.Info("Args:", t.Options)

	switch t.Route {
	case "list":
		opts, err := sdk.GetListOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.List(buf)

	case "export":
		opts, err := sdk.GetExportOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Export(buf)

	case "config":
		opts, err := sdk.GetConfigOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Config(buf)

	case "status":
		opts, err := sdk.GetStatusOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Status(buf)

	case "chunks":
		opts, err := sdk.GetChunksOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Chunks(buf)

	case "init":
		opts, err := sdk.GetInitOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Init(buf)

	case "names":
		opts, err := sdk.GetNamesOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Names(buf)

	case "slurp":
		opts, err := sdk.GetSlurpOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Slurp(buf)

	case "blocks":
		opts, err := sdk.GetBlocksOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Blocks(buf)

	case "transactions":
		opts, err := sdk.GetTransactionsOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Transactions(buf)

	case "receipts":
		opts, err := sdk.GetReceiptsOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Receipts(buf)

	case "logs":
		opts, err := sdk.GetLogsOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Logs(buf)

	case "traces":
		opts, err := sdk.GetTracesOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Traces(buf)

	case "state":
		opts, err := sdk.GetStateOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.State(buf)

	case "tokens":
		opts, err := sdk.GetTokensOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Tokens(buf)

	case "abis":
		opts, err := sdk.GetAbisOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.Abis(buf)

	case "when":
		opts, err := sdk.GetWhenOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.When(buf)

	default:
		// case "monitors":
		// case "daemon":
		// case "scrape":
		// case "explore":
		logger.Info(colors.Red + "Unknown sdk endpoint: " + t.Route + colors.Off)

	}
	return nil
}

func reportOpts(s fmt.Stringer) {
	if s == nil {
		return
	}
	logger.Info("Opts:", s.String())
}
