package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoAbis tests the abis sdk function
func DoAbis() {
	opts := sdk.AbisOptions{
		Addrs: []string{"uniswap.eth"},
	}
	logger.Info("DoAbis", opts)

	// ProxyFor base.Address `json:"proxyFor,omitempty"`
	// Find     []string     `json:"find,omitempty"`
	// Hint     []string     `json:"hint,omitempty"`
	// Encode   string       `json:"encode,omitempty"`
	// Globals
	// func (opts *AbisOptions) Abis() ([]types.Function, *types.MetaData, error) {

	known := []bool{false, true}
	globs := noEther(noRaw(globals))

	for _, k := range known {
		for _, g := range globs {
			baseFn := "abis/abis"
			opts.Known = k
			if !k || (!g.Cache && !g.Decache) {
				if k {
					baseFn += "-known"
				}
				opts.Globals = g
				fn := getFilename(baseFn, &opts.Globals)
				TestAbis(fn, &opts)
			}
		}
	}
}

func TestAbis(fn string, opts *sdk.AbisOptions) {
	// logger.Progress(true, "TestAbis", fn)
	if abis, _, err := opts.Abis(); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.Function](fn, abis); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}
