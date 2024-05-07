package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoNames tests the names sdk function
func DoNames() {
	file.EstablishFolder("sdkFuzzer-output/names")
	globs := noCache(noEther(noRaw(globals)))

	expands := []bool{false, true}
	matches := []bool{false, true}
	alls := []bool{false, true}
	customs := []bool{false, true}
	prefunds := []bool{false, true}
	regular := []bool{false, true}

	opts := sdk.NamesOptions{
		Terms: []string{"0xf"},
	}
	ShowHeader("DoNames-bools", &opts)

	for _, expand := range expands {
		for _, match := range matches {
			for _, all := range alls {
				for _, custom := range customs {
					for _, prefund := range prefunds {
						for _, reg := range regular {
							opts := sdk.NamesOptions{
								Terms:     []string{"0xf"},
								Expand:    expand,
								MatchCase: match,
								All:       all,
								Custom:    custom,
								Prefund:   prefund,
								Regular:   reg,
							}
							for _, g := range globs {
								opts.Globals = g
								fn := getFilenameNames("", &opts)
								TestNames("names", fn, &opts)
								fn = getFilenameNames("addrs", &opts)
								TestNames("addrs", fn, &opts)
								opts.Terms = []string{"0"}
								fn = getFilenameNames("tags", &opts)
								TestNames("tags", fn, &opts)
							}
						}
					}
				}
			}
		}
	}

	ShowHeader("DoNames-other", &opts)
	fn := getFilenameNames("autoname", &opts)
	TestNames("autoname", fn, &opts)

	// DryRun    bool     `json:"dryRun,omitempty"`
	// func (opts *NamesOptions) NamesClean() ([]types.Message, *types.MetaData, error) {
	// func (opts *NamesOptions) NamesCreate() ([]types.Name, *types.MetaData, error) {
	// func (opts *NamesOptions) NamesUpdate() ([]types.Name, *types.MetaData, error) {
	// func (opts *NamesOptions) NamesDelete() ([]types.Name, *types.MetaData, error) {
	// func (opts *NamesOptions) NamesUndelete() ([]types.Name, *types.MetaData, error) {
	// func (opts *NamesOptions) NamesRemove() ([]types.Name, *types.MetaData, error) {
}

func getFilenameNames(pre string, opts *sdk.NamesOptions) string {
	baseFn := "names/names"
	if len(pre) > 0 {
		baseFn += "-" + pre
	}
	if opts.Expand {
		baseFn += "-expand"
	}
	if opts.MatchCase {
		baseFn += "-match"
	}
	if opts.All {
		baseFn += "-all"
	}
	if opts.Custom {
		baseFn += "-custom"
	}
	if opts.Prefund {
		baseFn += "-prefund"
	}
	if opts.Regular {
		baseFn += "-regular"
	}
	return getFilename(baseFn, &opts.Globals)
}

func TestNames(which, fn string, opts *sdk.NamesOptions) {
	switch which {
	case "names":
		if names, _, err := opts.Names(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Name](fn, names); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "addrs":
		if names, _, err := opts.NamesAddr(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Name](fn, names); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "tags":
		if names, _, err := opts.NamesTags(); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Name](fn, names); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "autoname":
		addr := base.HexToAddress("0xde30da39c46104798bb5aa3fe8b9e0e1f348163f")
		if names, _, err := opts.NamesAutoname(addr); err != nil {
			ReportError(fn, err)
		} else {
			if err := SaveToFile[types.Message](fn, names); err != nil {
				ReportError(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
