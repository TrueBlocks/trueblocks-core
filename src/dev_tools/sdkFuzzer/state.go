package main

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoState tests the state sdk function
func DoState() {
	file.EstablishFolder("sdkFuzzer-output/state")
	opts := sdk.StateOptions{
		BlockIds: []string{"10092000"},
		Addrs:    []string{"0x3d9819210A31b4961b30EF54bE2aeD79B9c9Cd3B"},
	}
	ShowHeader("DoState", opts)

	art := []bool{false, true}
	globs := noRaw(globals)

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
					TestState(fn, &opts)
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
			TestStateCall("manifestHashMap(0x02f2b09b33fdbd406ead954a31f98bd29a2a3492,\"mainnet\")", fn, &opts)
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
			TestStateCall("0x0902f1ac()", fn, &opts)
		}
	}
}

func TestState(fn string, opts *sdk.StateOptions) {
	if state, _, err := opts.State(); err != nil {
		ReportError(fn, opts, err)
	} else {
		if err := SaveToFile[types.State](fn, state); err != nil {
			ReportError2(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}

func TestStateCall(call, fn string, opts *sdk.StateOptions) {
	fn = strings.ReplaceAll(fn, ".json", "-call.json")
	if stateResult, _, err := opts.StateCall(call); err != nil {
		ReportError(fn, opts, err)
	} else {
		if err := SaveToFile[types.Result](fn, stateResult); err != nil {
			ReportError2(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}
