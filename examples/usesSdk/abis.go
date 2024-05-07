package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoAbis tests the abis sdk function
func DoAbis() {
	file.EstablishFolder("usesSdk-output/abis")
	opts := sdk.AbisOptions{
		Addrs: []string{"uniswap.eth"},
	}
	logger.Info("DoAbis", opts)

	// ProxyFor base.Address `json:"proxyFor,omitempty"`
	// Find     []string     `json:"find,omitempty"`
	// Hint     []string     `json:"hint,omitempty"`
	// Encode   string       `json:"encode,omitempty"`

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

	// opts = sdk.AbisOptions{}
	logger.Info("DoAbis-Encode", opts)

	sigs := []string{
		"	event Transfer(address indexed from, address indexed to, uint256 amount);",
		"function transfer(address dst, uint rawAmount) external returns (bool)",
		"transfer(address,uint)",
		"transfer",
		"x",
	}
	globs = noCache(globs)
	for _, s := range sigs {
		baseFn := "abis/abis-encode" + s[:base.Max(1, base.Min(5, len(s)))]
		for _, g := range globs {
			opts.Globals = g
			fn := getFilename(baseFn, &opts.Globals)
			TestAbisEncode(s, fn, &opts)
		}
	}
}

func TestAbis(fn string, opts *sdk.AbisOptions) {
	if funcs, _, err := opts.Abis(); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.Function](fn, funcs); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}

func TestAbisEncode(sig, fn string, opts *sdk.AbisOptions) {
	if funcs, _, err := opts.AbisEncode(sig); err != nil {
		ReportError(fn, err)
	} else {
		if err := SaveToFile[types.Function](fn, funcs); err != nil {
			ReportError(fn, err)
		} else {
			ReportOkay(fn)
		}
	}
}
