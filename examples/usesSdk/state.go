package main

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoState tests the state sdk function
func DoState() {
	opts := sdk.StateOptions{
		BlockIds: []string{"10092000"},
		Addrs:    []string{"0x3d9819210A31b4961b30EF54bE2aeD79B9c9Cd3B"},
	}
	logger.Info("DoState", opts)

	// Addrs      []string     `json:"addrs,omitempty"`
	// BlockIds   []string     `json:"blocks,omitempty"`
	// Parts      StateParts   `json:"parts,omitempty"`
	// Changes    bool         `json:"changes,omitempty"`
	// NoZero     bool         `json:"noZero,omitempty"`
	// Articulate bool         `json:"articulate,omitempty"`
	// ProxyFor   base.Address `json:"proxyFor,omitempty"`
	//  func (opts *StateOptions) State() ([]types.State, *types.MetaData, error) {
	// func (opts *StateOptions) StateCall(val string) ([]types.Result, *types.MetaData, error) {
	// StateParts: NoSP, SPBalance, SPNonce, SPCode, SPProxy, SPDeployed, SPAccttype, SPSome, SPAll

	globs := noRaw(globals)

	changes := []bool{false, true}
	noZeros := []bool{false, true}
	parts := []sdk.StateParts{sdk.NoSP, sdk.SPAll} //, sdk.SPBalance /*sdk.SPNonce,*/, sdk.SPCode, sdk.SPProxy, sdk.SPDeployed, sdk.SPAccttype, sdk.SPSome, sdk.SPAll}
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
	art := []bool{false, true}
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
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.State](fn, state); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}

func TestStateCall(call, fn string, opts *sdk.StateOptions) {
	fn = strings.ReplaceAll(fn, ".json", "-call.json")
	if stateResult, _, err := opts.StateCall(call); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.Result](fn, stateResult); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}
