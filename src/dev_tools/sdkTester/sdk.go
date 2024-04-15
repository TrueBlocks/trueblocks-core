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

func (t *TestCase) RunSdkTest(i, n int) (bool, bool) {
	testing := []string{
		"list",
		"export",
		"monitors",
		"config",
		"status",
		"daemon",
		"scrape",
		"chunks",
		"init",
		"explore",
		"names",
		// "slurp",
		"abis",
		"blocks",
		"transactions",
		"receipts",
		"logs",
		"state",
		"tokens",
		"traces",
		"when",
	}
	interesting := false
	for _, test := range testing {
		if test == t.Route && t.PathTool != "apps/chifra" {
			interesting = true
			break
		}
	}
	// interesting = t.Route == "init" && t.Filename == "fmt_txt"
	if !interesting {
		return false, false
	}

	parts := strings.Split(t.PathTool, "/")
	if len(parts) != 2 {
		fmt.Fprintf(os.Stderr, "Invalid pathTool: %s\n", t.PathTool)
		return false, false
	}

	var ff *os.File
	folder := t.WorkingPath
	if !file.FolderExists(folder) {
		file.EstablishFolder(folder)
	}

	wasTested := false
	passedTest := false

	fn := filepath.Join(folder, parts[1]+"_"+t.Filename+".txt")
	if interesting {
		os.Setenv("TEST_MODE", "true")
		logger.SetTestMode(true)
		if !debugging {
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

				msg := "[passed " + cm["greenCheck"] + "]"
				if wasTested && !passedTest {
					msg = "[failed " + cm["redX"] + "]"
				}

				skip := strings.Repeat(" ", utils.Max(0, 120-len(fn)))
				fmt.Printf("   Testing %d of %d %s %s%s%s", i, n, msg, fn, skip, eol)
			}()
		}
		logger.Info(t.Route + "?" + t.Cannonical)

	} else {
		logger.Info()
		logger.Info(strings.Repeat("=", 40), t.Filename, strings.Repeat("=", 40))
		logger.Info(fmt.Sprintf("Route: %s, PathTool: %s, Enabled: %v, Options: %v", t.Route, t.PathTool, t.Enabled, t.OptionArray))
		logger.Info("\t" + strings.Trim(fmt.Sprintf("chifra %s %s", t.Route, t.Clean()), " "))
		return false, false
	}

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
	return wasTested, passedTest
}

// SdkTest runs a test case through the SDK and returns the results in the provided bytes.Buffer
func (t *TestCase) SdkTest(buf *bytes.Buffer) error {
	logger.Info("Args:", t.OptionArray)

	switch t.Route {
	case "list":
		opts, err := sdk.GetListOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ListBytes(buf)

	case "export":
		opts, err := sdk.GetExportOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ExportBytes(buf)

	case "config":
		opts, err := sdk.GetConfigOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ConfigBytes(buf)

	case "status":
		opts, err := sdk.GetStatusOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.StatusBytes(buf)

	case "chunks":
		opts, err := sdk.GetChunksOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ChunksBytes(buf)

	case "init":
		opts, err := sdk.GetInitOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.InitBytes(buf)

	case "names":
		opts, err := sdk.GetNamesOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.NamesBytes(buf)

	case "slurp":
		opts, err := sdk.GetSlurpOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.SlurpBytes(buf)

	case "blocks":
		opts, err := sdk.GetBlocksOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.BlocksBytes(buf)

	case "transactions":
		opts, err := sdk.GetTransactionsOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.TransactionsBytes(buf)

	case "receipts":
		opts, err := sdk.GetReceiptsOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.ReceiptsBytes(buf)

	case "logs":
		opts, err := sdk.GetLogsOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.LogsBytes(buf)

	case "traces":
		opts, err := sdk.GetTracesOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.TracesBytes(buf)

	case "state":
		opts, err := sdk.GetStateOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.StateBytes(buf)

	case "tokens":
		opts, err := sdk.GetTokensOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.TokensBytes(buf)

	case "abis":
		opts, err := sdk.GetAbisOptions(t.OptionArray)
		reportOpts(opts)
		if err != nil {
			return err
		}
		return opts.AbisBytes(buf)

	case "when":
		opts, err := sdk.GetWhenOptions(t.OptionArray)
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
