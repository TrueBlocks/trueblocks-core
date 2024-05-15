// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoStatus tests the Status sdk function
func DoStatus() {
	file.EstablishFolder("sdkFuzzer-output/status")
	opts := sdk.StatusOptions{}
	ShowHeader("DoStatus", opts)

	globs := noCache(noRaw(noEther(globals)))
	chains := []bool{false, true}
	// firstRecord is not fuzzed
	// maxRecords is not fuzzed
	// Fuzz Loop
	// EXISTING_CODE
	// func (opts *StatusOptions) StatusDiagnose() ([]bool, *types.MetaData, error) {

	firsts := []uint64{0, 10}
	maxes := []uint64{0, 500}
	// status,command,default|
	for _, c := range chains {
		for _, f := range firsts {
			for _, m := range maxes {
				for _, g := range globs {
					baseName := "status/status"
					opts.Chains = c
					if c {
						baseName += "-chains"
					}
					if f > 0 {
						baseName += fmt.Sprintf("-first-%d", f)
					}
					if m > 0 {
						baseName += fmt.Sprintf("-max-%d", m)
					}
					opts.FirstRecord = f
					opts.MaxRecords = m
					opts.Globals = g

					fn := getFilename(baseName, &opts.Globals)
					TestStatus("index", "", fn, &opts)
					TestStatus("blooms", "", fn, &opts)
					TestStatus("blocks", "", fn, &opts)
					TestStatus("transactions", "", fn, &opts)
					TestStatus("traces", "", fn, &opts)
					TestStatus("logs", "", fn, &opts)
					TestStatus("statements", "", fn, &opts)
					TestStatus("results", "", fn, &opts)
					TestStatus("state", "", fn, &opts)
					TestStatus("tokens", "", fn, &opts)
					TestStatus("monitors", "", fn, &opts)
					TestStatus("names", "", fn, &opts)
					TestStatus("abis", "", fn, &opts)
					TestStatus("slurps", "", fn, &opts)
					TestStatus("staging", "", fn, &opts)
					TestStatus("unripe", "", fn, &opts)
					TestStatus("maps", "", fn, &opts)
					TestStatus("some", "", fn, &opts)
					TestStatus("all", "", fn, &opts)
				}
			}
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestStatus(which, value, fn string, opts *sdk.StatusOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "index":
		if index, _, err := opts.StatusIndex(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, index); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "blooms":
		if blooms, _, err := opts.StatusBlooms(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, blooms); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "blocks":
		if blocks, _, err := opts.StatusBlocks(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, blocks); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "transactions":
		if transactions, _, err := opts.StatusTransactions(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, transactions); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "traces":
		if traces, _, err := opts.StatusTraces(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, traces); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "logs":
		if logs, _, err := opts.StatusLogs(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, logs); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "statements":
		if statements, _, err := opts.StatusStatements(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, statements); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "results":
		if results, _, err := opts.StatusResults(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, results); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "state":
		if state, _, err := opts.StatusState(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, state); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "tokens":
		if tokens, _, err := opts.StatusTokens(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, tokens); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "monitors":
		if monitors, _, err := opts.StatusMonitors(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, monitors); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "names":
		if names, _, err := opts.StatusNames(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, names); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "abis":
		if abis, _, err := opts.StatusAbis(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, abis); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "slurps":
		if slurps, _, err := opts.StatusSlurps(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, slurps); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "staging":
		if staging, _, err := opts.StatusStaging(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, staging); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "unripe":
		if unripe, _, err := opts.StatusUnripe(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, unripe); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "maps":
		if maps, _, err := opts.StatusMaps(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, maps); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "some":
		if some, _, err := opts.StatusSome(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, some); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "all":
		if all, _, err := opts.StatusAll(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, all); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "diagnose":
		if diagnose, _, err := opts.StatusDiagnose(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Status](fn, diagnose); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		logger.Fatal("Quitting...")
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
