// package main

// import (
// 	"bytes"

// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
// 	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
// 	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
// )

// // SdkTest runs a test case through the SDK and returns the results in the provided bytes.Buffer
// func (t *TestCase) SdkTest(buf *bytes.Buffer) error {
// 	switch t.Route {
// 	case "list":
// 		if _, err := sdk.GetListOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.List(buf)
// 		}
// 	case "export":
// 		if _, err := sdk.GetExportOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Export(buf)
// 		}
// 	case "monitors":
// 		if _, err := sdk.GetMonitorsOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Monitors(buf)
// 		}
// 	case "names":
// 		if _, err := sdk.GetNamesOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Names(buf)
// 		}
// 	case "abis":
// 		if _, err := sdk.GetAbisOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Abis(buf)
// 		}
// 	case "blocks":
// 		if _, err := sdk.GetBlocksOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Blocks(buf)
// 		}
// 	case "transactions":
// 		if _, err := sdk.GetTransactionsOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Transactions(buf)
// 		}
// 	case "receipts":
// 		if _, err := sdk.GetReceiptsOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Receipts(buf)
// 		}
// 	case "logs":
// 		if _, err := sdk.GetLogsOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Logs(buf)
// 		}
// 	case "traces":
// 		if _, err := sdk.GetTracesOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Traces(buf)
// 		}
// 	case "when":
// 		if _, err := sdk.GetWhenOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.When(buf)
// 		}
// 	case "state":
// 		if _, err := sdk.GetStateOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.State(buf)
// 		}
// 	case "tokens":
// 		if _, err := sdk.GetTokensOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Tokens(buf)
// 		}
// 	case "config":
// 		if _, err := sdk.GetConfigOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Config(buf)
// 		}
// 	case "status":
// 		if _, err := sdk.GetStatusOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Status(buf)
// 		}
// 	// case "daemon":
// 	// 	if _, err := sdk.GetDaemonOptions(t.Options); err != nil {
// 	// 		return err
// 	// 	} else {
// 	// 		return opts.Daemon(buf)
// 	// 	}
// 	case "scrape":
// 		if _, err := sdk.GetScrapeOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Scrape(buf)
// 		}
// 	case "chunks":
// 		if _, err := sdk.GetChunksOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Chunks(buf)
// 		}
// 	case "init":
// 		if _, err := sdk.GetInitOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Init(buf)
// 		}
// 	// case "explore":
// 	// 	if _, err := sdk.GetExploreOptions(t.Options); err != nil {
// 	// 		return err
// 	// 	} else {
// 	// 		return opts.Explore(buf)
// 	// 	}
// 	case "slurp":
// 		if _, err := sdk.GetSlurpOptions(t.Options); err != nil {
// 			return err
// 		} else {
// 			return nil // return opts.Slurp(buf)
// 		}
// 	default:
// 		logger.Info(colors.Red + "Unknown: " + t.Route + colors.Off)
// 	}

//		return nil
//	}
package main
<<<<<<< HEAD
=======

import (
	"bytes"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
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
		if _, err := sdk.GetExportOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Export(buf)
		}
	case "monitors":
		if _, err := sdk.GetMonitorsOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Monitors(buf)
		}
	case "names":
		if _, err := sdk.GetNamesOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Names(buf)
		}
	case "abis":
		if _, err := sdk.GetAbisOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Abis(buf)
		}
	case "blocks":
		if _, err := sdk.GetBlocksOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Blocks(buf)
		}
	case "transactions":
		if _, err := sdk.GetTransactionsOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Transactions(buf)
		}
	case "receipts":
		if opts, err := sdk.GetReceiptsOptions(t.Options); err != nil {
			return err
		} else {
			return opts.Receipts(buf)
		}
	case "logs":
		if _, err := sdk.GetLogsOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Logs(buf)
		}
	case "traces":
		if _, err := sdk.GetTracesOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Traces(buf)
		}
	case "when":
		if opts, err := sdk.GetWhenOptions(t.Options); err != nil {
			return err
		} else {
			return opts.When(buf)
		}
	case "state":
		if _, err := sdk.GetStateOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.State(buf)
		}
	case "tokens":
		if _, err := sdk.GetTokensOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Tokens(buf)
		}
	case "config":
		if _, err := sdk.GetConfigOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Config(buf)
		}
	case "status":
		if _, err := sdk.GetStatusOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Status(buf)
		}
	// case "daemon":
	// 	if _, err := sdk.GetDaemonOptions(t.Options); err != nil {
	// 		return err
	// 	} else {
	// 		return opts.Daemon(buf)
	// 	}
	case "scrape":
		if _, err := sdk.GetScrapeOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Scrape(buf)
		}
	case "chunks":
		if _, err := sdk.GetChunksOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Chunks(buf)
		}
	case "init":
		if _, err := sdk.GetInitOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Init(buf)
		}
	// case "explore":
	// 	if _, err := sdk.GetExploreOptions(t.Options); err != nil {
	// 		return err
	// 	} else {
	// 		return opts.Explore(buf)
	// 	}
	case "slurp":
		if _, err := sdk.GetSlurpOptions(t.Options); err != nil {
			return err
		} else {
			return nil // return opts.Slurp(buf)
		}
	default:
		logger.Info(colors.Red + "Unknown: " + t.Route + colors.Off)
	}

	return nil
}
>>>>>>> ec00f9d435535d8bad8b2049a4d6980dd03473db
