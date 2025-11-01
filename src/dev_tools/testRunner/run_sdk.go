package main

import (
	"bytes"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	sdk "github.com/TrueBlocks/trueblocks-sdk/v6"
)

// SdkTest runs a test case through the SDK and returns the results in the provided bytes.Buffer
func (t *TestCase) SdkTest() (string, error) {
	reportFunc := func(s fmt.Stringer) {
		if s == nil {
			return
		}
		logger.Info("Opts:", s.String())
	}

	var buff bytes.Buffer
	buf := &buff

	logger.Info("Args:", t.SdkOptionsArray)

	switch t.Route {
	case "list":
		opts, err := sdk.GetListOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.ListBytes(buf)
		return buf.String(), err

	case "export":
		opts, err := sdk.GetExportOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.ExportBytes(buf)
		return buf.String(), err

	case "config":
		opts, err := sdk.GetConfigOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.ConfigBytes(buf)
		return buf.String(), err

	case "status":
		opts, err := sdk.GetStatusOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.StatusBytes(buf)
		return buf.String(), err

	case "chunks":
		opts, err := sdk.GetChunksOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.ChunksBytes(buf)
		return buf.String(), err

	case "init":
		opts, err := sdk.GetInitOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.InitBytes(buf)
		return buf.String(), err

	case "names":
		opts, err := sdk.GetNamesOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.NamesBytes(buf)
		return buf.String(), err

	case "slurp":
		opts, err := sdk.GetSlurpOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.SlurpBytes(buf)
		return buf.String(), err

	case "blocks":
		opts, err := sdk.GetBlocksOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.BlocksBytes(buf)
		return buf.String(), err

	case "transactions":
		opts, err := sdk.GetTransactionsOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.TransactionsBytes(buf)
		return buf.String(), err

	case "receipts":
		opts, err := sdk.GetReceiptsOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.ReceiptsBytes(buf)
		return buf.String(), err

	case "logs":
		opts, err := sdk.GetLogsOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.LogsBytes(buf)
		return buf.String(), err

	case "traces":
		opts, err := sdk.GetTracesOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.TracesBytes(buf)
		return buf.String(), err

	case "state":
		opts, err := sdk.GetStateOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.StateBytes(buf)
		return buf.String(), err

	case "tokens":
		opts, err := sdk.GetTokensOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.TokensBytes(buf)
		return buf.String(), err

	case "abis":
		opts, err := sdk.GetAbisOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.AbisBytes(buf)
		return buf.String(), err

	case "when":
		opts, err := sdk.GetWhenOptions(t.SdkOptionsArray)
		reportFunc(opts)
		if err != nil {
			return "", err
		}
		err = opts.WhenBytes(buf)
		return buf.String(), err

	default:
		// case "monitors":
		// case "daemon":
		// case "scrape":
		// case "explore":
		logger.Info("Unknown sdk endpoint: " + t.Route)

	}

	return "", nil
}

var cm = map[string]string{
	"greenCheck":    colors.Green + "✓" + colors.Off,
	"yellowCaution": colors.Yellow + "!!" + colors.Off,
	"redX":          colors.Red + "X" + colors.Off,
	"whiteStar":     colors.White + "*" + colors.Off,
}
