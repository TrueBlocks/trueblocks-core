package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoSlurp tests the slurp sdk function
func DoSlurp() {
	file.EstablishFolder("sdkFuzzer-output/slurp")
	opts := sdk.SlurpOptions{
		Addrs:   []string{testAddrs[0]},
		PerPage: 10,
	}
	ShowHeader("DoSlurp", &opts)

	// Page       uint64      `json:"page,omitempty"`
	// PageId     string      `json:"pageId,omitempty"`
	// PerPage    uint64      `json:"perPage,omitempty"`

	types1 := []sdk.SlurpTypes{
		// sdk.NoST,
		// sdk.STExt,
		// sdk.STInt,
		// sdk.STToken,
		// sdk.STNfts,
		// sdk.ST1155,
		// sdk.STMiner,
		// sdk.STUncles,
		// sdk.STWithdrawals,
		sdk.STSome,
		// sdk.STAll,
	}
	sources := []sdk.SlurpSource{
		sdk.SSEtherscan,
		// sdk.SSKey,
		// sdk.SSCovalent,
		// sdk.SSAlchemy,
	}
	art := []bool{false, true}
	globs := noCache(globals)

	for _, s := range sources {
		for _, t := range types1 {
			for _, a := range art {
				baseFn := "slurp/slurp" + "-" + t.String() + "-" + t.String()
				if a {
					baseFn += "-articulate"
				}
				opts.Source = s
				opts.Types = t
				opts.Articulate = a
				for _, g := range globs {
					opts.Globals = g
					fn := getFilename(baseFn, &opts.Globals)
					TestSlurp("slurp", fn, &opts)
					fn = getFilename(baseFn+"-apps", &opts.Globals)
					TestSlurp("appearances", fn, &opts)
					fn = getFilename(baseFn+"-count", &opts.Globals)
					TestSlurp("count", fn, &opts)
				}
			}
		}
	}
}

func TestSlurp(which, fn string, opts *sdk.SlurpOptions) {
	switch which {
	case "slurp":
		if slurp, _, err := opts.Slurp(); err != nil {
			logger.Error(err)
		} else {
			if err := SaveToFile[types.Slurp](fn, slurp); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "appearances":
		if appearances, _, err := opts.SlurpAppearances(); err != nil {
			logger.Error(err)
		} else {
			if err := SaveToFile[types.Appearance](fn, appearances); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if count, _, err := opts.SlurpCount(); err != nil {
			logger.Error(err)
		} else {
			if err := SaveToFile[types.SlurpCount](fn, count); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
