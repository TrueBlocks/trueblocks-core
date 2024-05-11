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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoState tests the State sdk function
func DoState() {
	file.EstablishFolder("sdkFuzzer-output/state")
	opts := sdk.StateOptions{}
	ShowHeader("DoState", opts)

	globs := noRaw(globals)
	// EXISTING_CODE
	art := []bool{false, true}

	changes := []bool{false, true}
	noZeros := []bool{false, true}
	parts := []sdk.StateParts{
		sdk.SPBalance,
		sdk.SPNonce,
		sdk.SPCode,
		sdk.SPProxy,
		sdk.SPDeployed,
		sdk.SPAccttype,
		sdk.SPSome,
		sdk.SPAll,
	}
	opts = sdk.StateOptions{
		BlockIds: []string{"10092000"},
		Addrs:    []string{"0x3d9819210A31b4961b30EF54bE2aeD79B9c9Cd3B"},
	}
	// state,command,default|caching|ether|
	for _, c := range changes {
		for _, z := range noZeros {
			for _, p := range parts {
				baseFn := "state/state"
				if c {
					baseFn += "-changes"
				}
				if z {
					baseFn += "-noZero"
				}
				if p != sdk.NoSP {
					baseFn += "-" + strings.ToLower(p.String())
				}
				opts.Changes = c
				opts.NoZero = z
				opts.Parts = p
				for _, g := range globs {
					opts.Globals = g
					fn := getFilename(baseFn, &opts.Globals)
					TestState("state", "", fn, &opts)
				}
			}
		}
	}

	opts = sdk.StateOptions{
		BlockIds: []string{"18000000"},
		Addrs:    []string{"unchainedindex.eth"},
	}
	ShowHeader("DoState-Call", opts)

	for _, a := range art {
		baseFn := "state/state-call"
		if a {
			baseFn += "-art"
		}
		opts.Articulate = a
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestState("call", "manifestHashMap(0x02f2b09b33fdbd406ead954a31f98bd29a2a3492,\"mainnet\")", fn, &opts)
		}
	}

	opts = sdk.StateOptions{
		BlockIds: []string{"10092000"},
		Addrs:    []string{"0x3d9819210A31b4961b30EF54bE2aeD79B9c9Cd3B"},
	}
	ShowHeader("DoState-Call-Proxy", opts)

	for _, a := range art {
		baseFn := "state/state-call-proxy"
		if a {
			baseFn += "-art"
		}
		opts.ProxyFor = base.HexToAddress("0xbb2b8038a1640196fbe3e38816f3e67cba72d940")
		opts.Articulate = a
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestState("state", "0x0902f1ac()", fn, &opts)
		}
	}
	// EXISTING_CODE
	Wait()
}

func TestState(which, value, fn string, opts *sdk.StateOptions) {
	fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	// EXISTING_CODE
	// EXISTING_CODE

	switch which {
	case "state":
		if state, _, err := opts.State(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.State](fn, state); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "call":
		if call, _, err := opts.StateCall(value); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Result](fn, call); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	default:
		ReportError(fn, opts, fmt.Errorf("unknown which: %s", which))
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
