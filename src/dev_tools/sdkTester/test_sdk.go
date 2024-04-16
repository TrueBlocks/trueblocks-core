package main

import (
	"bytes"
	"encoding/json"
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/v0/sdk"
)

func (tr *Runner) RunSdkTest(t *TestCase) (bool, bool, error) {
	tr.AppendLog(t)

	working := t.WorkingPath
	if !file.FolderExists(working) {
		file.EstablishFolder(working)
	}

	wasTested := false
	passedTest := false

	os.Setenv("TEST_MODE", "true")
	logger.SetTestMode(true)

	parts := strings.Split(t.PathTool, "/")
	workFn := filepath.Join(working, parts[1]+"_"+t.Filename+".txt")
	goldFn := strings.Replace(workFn, "working", "gold", -1)

	workFile, _ := os.Create(workFn)
	logger.SetLoggerWriter(workFile)
	logger.ToggleDecoration()

	defer func() {
		logger.ToggleDecoration()
		logger.SetLoggerWriter(os.Stderr)
		eol := "\r"
		if wasTested && !passedTest {
			eol = "\n"
		}

		msg := "[passed " + cm["greenCheck"] + "]"
		if wasTested && !passedTest {
			msg = "[failed " + cm["redX"] + "]"
		}

		skip := strings.Repeat(" ", utils.Max(0, 120-len(workFn)))
		colors.ColorsOn()
		fmt.Printf("   %sTesting %d of %d %s %s%s%s%s", colors.Green, tr.NTested, tr.NFiltered, msg, workFn, skip, colors.Off, eol)
		colors.ColorsOff()
	}()

	logger.Info(t.Route + "?" + t.SdkOptions)

	var buff bytes.Buffer
	var results string
	wasTested = true
	if err := t.SdkTest(&buff); err != nil {
		type E struct {
			Errors []string `json:"errors"`
		}
		e := E{Errors: []string{err.Error()}}
		bytes, _ := json.MarshalIndent(e, "", "  ")
		results = string(bytes)
	} else {
		results = strings.Trim(buff.String(), "\n\r")
	}

	if len(results) > 0 {
		results = strings.Replace(results, "3735928559", "\"0xdeadbeef\"", -1)
		logger.Info(results)
	}

	if workFile != nil {
		workFile.Close()
		newContents := file.AsciiFileToString(workFn)
		oldContents := file.AsciiFileToString(goldFn)
		passedTest = newContents == oldContents
	}

	return wasTested, passedTest, nil
}

// SdkTest runs a test case through the SDK and returns the results in the provided bytes.Buffer
func (t *TestCase) SdkTest(buf *bytes.Buffer) error {
	logger.Info("Args:", t.SdkOptionsArray)

	switch t.Route {
	case "list":
		opts, err := sdk.GetListOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ListBytes(buf)

	case "export":
		opts, err := sdk.GetExportOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ExportBytes(buf)

	case "config":
		opts, err := sdk.GetConfigOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ConfigBytes(buf)

	case "status":
		opts, err := sdk.GetStatusOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.StatusBytes(buf)

	case "chunks":
		opts, err := sdk.GetChunksOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ChunksBytes(buf)

	case "init":
		opts, err := sdk.GetInitOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.InitBytes(buf)

	case "names":
		opts, err := sdk.GetNamesOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.NamesBytes(buf)

	case "slurp":
		opts, err := sdk.GetSlurpOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.SlurpBytes(buf)

	case "blocks":
		opts, err := sdk.GetBlocksOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.BlocksBytes(buf)

	case "transactions":
		opts, err := sdk.GetTransactionsOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.TransactionsBytes(buf)

	case "receipts":
		opts, err := sdk.GetReceiptsOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ReceiptsBytes(buf)

	case "logs":
		opts, err := sdk.GetLogsOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.LogsBytes(buf)

	case "traces":
		opts, err := sdk.GetTracesOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.TracesBytes(buf)

	case "state":
		opts, err := sdk.GetStateOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.StateBytes(buf)

	case "tokens":
		opts, err := sdk.GetTokensOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.TokensBytes(buf)

	case "abis":
		opts, err := sdk.GetAbisOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.AbisBytes(buf)

	case "when":
		opts, err := sdk.GetWhenOptions(t.SdkOptionsArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.WhenBytes(buf)

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

var cm = map[string]string{
	"greenCheck":    colors.Green + "✓" + colors.Off,
	"yellowCaution": colors.Yellow + "!!" + colors.Off,
	"redX":          colors.Red + "X" + colors.Off,
	"whiteStar":     colors.White + "*" + colors.Off,
}
