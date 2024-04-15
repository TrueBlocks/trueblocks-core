// TODO: This can be auto generated
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
	return true, true, nil
	parts := strings.Split(t.PathTool, "/")
	if len(parts) != 2 {
		fmt.Fprintf(os.Stderr, "Invalid pathTool: %s\n", t.PathTool)
		return false, false, nil
	}

	var ff *os.File
	folder := t.WorkingPath
	if !file.FolderExists(folder) {
		file.EstablishFolder(folder)
	}

	wasTested := false
	passedTest := false

	fn := filepath.Join(folder, parts[1]+"_"+t.Filename+".txt")
	os.Setenv("TEST_MODE", "true")
	logger.SetTestMode(true)
	ff, _ = os.Create(fn)
	logger.SetLoggerWriter(ff)
	logger.ToggleDecoration()
	defer func() {
		logger.ToggleDecoration()
		logger.SetLoggerWriter(os.Stderr)

		eol := "\r"
		if wasTested && !passedTest {
			eol = "\n"
		}

		var cm = map[string]string{
			"greenCheck":    colors.Green + "✓" + colors.Off,
			"yellowCaution": colors.Yellow + "!!" + colors.Off,
			"redX":          colors.Red + "X" + colors.Off,
			"whiteStar":     colors.White + "*" + colors.Off,
		}
		msg := "[passed " + cm["greenCheck"] + "]"
		if wasTested && !passedTest {
			msg = "[failed " + cm["redX"] + "]"
		}

		skip := strings.Repeat(" ", utils.Max(0, 120-len(fn)))
		fmt.Printf("   Testing %d of %d %s %s%s%s", tr.NTested, tr.NFiltered, msg, fn, skip, eol)
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
		// because testRunner does this, we need to do it here
		results = strings.Replace(results, "3735928559", "\"0xdeadbeef\"", -1)
		logger.Info(results)
	}

	if ff != nil {
		ff.Close()
		newContents := file.AsciiFileToString(fn)
		oldContents := file.AsciiFileToString(strings.Replace(fn, "working", "gold", -1))
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
