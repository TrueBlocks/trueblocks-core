package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

func reportOpts(s fmt.Stringer) {
	if s == nil {
		return
	}
	logger.Info("Opts:", s.String())
}

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
		if _, err := sdk.GetExportOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Export(buf)
		}

	case "monitors":
		if _, err := sdk.GetMonitorsOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Monitors(buf)
		}

	case "config":
		if _, err := sdk.GetConfigOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Config(buf)
		}

	case "status":
		if _, err := sdk.GetStatusOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Status(buf)
		}

	case "scrape":
		if _, err := sdk.GetScrapeOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Scrape(buf)
		}

	case "chunks":
		if _, err := sdk.GetChunksOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Chunks(buf)
		}

	case "init":
		if _, err := sdk.GetInitOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Init(buf)
		}

	// case "daemon":
	// case "explore":

	case "names":
		if _, err := sdk.GetNamesOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Names(buf)
		}

	case "slurp":
		if _, err := sdk.GetSlurpOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Slurp(buf)
		}

	case "blocks":
		if _, err := sdk.GetBlocksOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Blocks(buf)
		}

	case "transactions":
		if _, err := sdk.GetTransactionsOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Transactions(buf)
		}

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
		if _, err := sdk.GetTracesOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Traces(buf)
		}

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
		if _, err := sdk.GetAbisOptions(t.Options); err != nil {
			// logger.Info("Opts:", opts.String())
			return err
		} else {
			// logger.Info("Opts:", opts.String())
			return nil // return opts.Abis(buf)
		}

	case "when":
		opts, err := sdk.GetWhenOptions(t.Options)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.When(buf)

	default:
		logger.Info(colors.Red + "Unknown: " + t.Route + colors.Off)

	}
	return nil
}
