package main

import (
	"bytes"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

// SdkTest runs a test case through the SDK and returns the results in the provided bytes.Buffer
func (t *TestCase) SdkTest(buf *bytes.Buffer) error {
	switch t.Route {
	case "list":
		if opts, err := sdk.GetListOptions(t.Options); err != nil {
			return err
		} else {
			return opts.List(buf)
		}
	case "export":
		if opts, err := sdk.GetExportOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Export(buf)
		}
	case "monitors":
		if opts, err := sdk.GetMonitorsOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Monitors(buf)
		}
	case "names":
		if opts, err := sdk.GetNamesOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Names(buf)
		}
	case "abis":
		if opts, err := sdk.GetAbisOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Abis(buf)
		}
	case "blocks":
		if opts, err := sdk.GetBlocksOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Blocks(buf)
		}
	case "transactions":
		if opts, err := sdk.GetTransactionsOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Transactions(buf)
		}
	case "receipts":
		if opts, err := sdk.GetReceiptsOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Receipts(buf)
		}
	case "logs":
		if opts, err := sdk.GetLogsOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Logs(buf)
		}
	case "traces":
		if opts, err := sdk.GetTracesOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Traces(buf)
		}
	case "when":
		if opts, err := sdk.GetWhenOptions(t.Options); err != nil {
			return err
		} else {
			return opts.When(buf)
		}
	case "state":
		if opts, err := sdk.GetStateOptions(t.Options); err != nil {
			return err
		} else {
			return opts.State(buf)
		}
	case "tokens":
		if opts, err := sdk.GetTokensOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Tokens(buf)
		}
	case "config":
		if opts, err := sdk.GetConfigOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Config(buf)
		}
	case "status":
		if opts, err := sdk.GetStatusOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Status(buf)
		}
	// case "daemon":
	// 	if opts, err := sdk.GetDaemonOptions(t.Options); err != nil {
	// 		return err
	// 	} else {
	// 		return opts.Daemon(buf)
	// 	}
	case "scrape":
		if opts, err := sdk.GetScrapeOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Scrape(buf)
		}
	case "chunks":
		if opts, err := sdk.GetChunksOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Chunks(buf)
		}
	case "init":
		if opts, err := sdk.GetInitOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Init(buf)
		}
	// case "explore":
	// 	if opts, err := sdk.GetExploreOptions(t.Options); err != nil {
	// 		return err
	// 	} else {
	// 		return opts.Explore(buf)
	// 	}
	case "slurp":
		if opts, err := sdk.GetSlurpOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Slurp(buf)
		}
	default:
		logger.Info(*t)
	}

	return nil
}
