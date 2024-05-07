package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoList tests the List sdk function
func DoList() {
	file.EstablishFolder("sdkFuzzer-output/list")
	globs := noCache(noEther(noRaw(globals)))

	// Addrs       []string     `json:"addrs,omitempty"`
	// NoZero      bool         `json:"noZero,omitempty"`
	// Unripe      bool         `json:"unripe,omitempty"`
	// Silent      bool         `json:"silent,omitempty"`
	// FirstRecord uint64       `json:"firstRecord,omitempty"`
	// MaxRecords  uint64       `json:"maxRecords,omitempty"`
	// Reversed    bool         `json:"reversed,omitempty"`
	// Publisher   base.Address `json:"publisher,omitempty"`
	// FirstBlock  base.Blknum  `json:"firstBlock,omitempty"`
	// LastBlock   base.Blknum  `json:"lastBlock,omitempty"`
	// func (opts *ListOptions) List() ([]types.Appearance, *types.MetaData, error) {
	// func (opts *ListOptions) ListCount() ([]types.AppearanceCount, *types.MetaData, error) {
	// func (opts *ListOptions) ListBounds() ([]types.Bounds, *types.MetaData, error) {

	types := []string{"list", "count", "bounds"}
	for _, t := range types {
		opts := sdk.ListOptions{
			Addrs: []string{testAddrs[0]},
		}
		ShowHeader("DoList-"+t, opts)
		nzs := []bool{false, true}
		for _, nz := range nzs {
			baseFn := "list/list"
			if nz && t == "count" {
				opts.NoZero = true
				baseFn += "-noZero"
			}
			for _, g := range globs {
				opts.Globals = g
				fn2 := baseFn + "-" + t
				fn := getFilename(fn2, &opts.Globals)
				TestList(t, fn, &opts)
			}
		}
	}
}

func TestList(which, fn string, opts *sdk.ListOptions) {
	switch which {
	case "list":
		if apps, _, err := opts.List(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Appearance](fn, apps); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if counts, _, err := opts.ListCount(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.AppearanceCount](fn, counts); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "bounds":
		if bounds, _, err := opts.ListBounds(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Bounds](fn, bounds); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}

// func DoList() {
// 	opts := sdk.ListOptions{
// 		Addrs: []string{testAddrs[0]},
// 	}
// 	ShowHeader("DoList", &opts)

// 	if appearances, _, err := opts.List(); err != nil {
// 		logger.Error(err)
// 	} else {
// 		if err := SaveToFile[types.Appearance]("sdkFuzzer/list.json", appearances); err != nil {
// 			logger.Error(err)
// 		}
// 	}

// 	if appearancesCount, _, err := opts.ListCount(); err != nil {
// 		logger.Error(err)
// 	} else {
// 		if err := SaveToFile[types.AppearanceCount]("sdkFuzzer/listCount.json", appearancesCount); err != nil {
// 			logger.Error(err)
// 		}
// 	}

// 	if bounds, _, err := opts.ListBounds(); err != nil {
// 		logger.Error(err)
// 	} else {
// 		if err := SaveToFile[types.Bounds]("sdkFuzzer/listBounds.json", bounds); err != nil {
// 			logger.Error(err)
// 		}
// 	}
// }
