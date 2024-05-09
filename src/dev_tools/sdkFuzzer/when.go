package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoWhen tests the When sdk function
func DoWhen() {
	file.EstablishFolder("sdkFuzzer-output/when")
	globs := noEther(noRaw(globals))

	// Truncate base.Blknum `json:"truncate,omitempty"`
	// Repair   bool        `json:"repair,omitempty"`
	// Check    bool        `json:"check,omitempty"`
	// Update   bool        `json:"update,omitempty"`
	// Deep     bool        `json:"deep,omitempty"`

	types := []string{"when", "list", "timestamps", "count"}
	for _, t := range types {
		opts := sdk.WhenOptions{}
		if t != "list" && t != "when" {
			opts.BlockIds = testBlocks
		}
		ShowHeader("DoWhen-"+t, opts)
		for _, g := range globs {
			baseFn := "when/when"
			opts.Globals = g
			baseFn += "-" + t
			fn := getFilename(baseFn, &opts.Globals)
			TestWhen(t, fn, &opts)
		}
	}
}

func TestWhen(which, fn string, opts *sdk.WhenOptions) {
	switch which {
	case "when":
		if blocks, _, err := opts.When(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.NamedBlock](fn, blocks); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "list":
		if blocks, _, err := opts.WhenList(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.NamedBlock](fn, blocks); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "timestamps":
		if timestamps, _, err := opts.WhenTimestamps(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.Timestamp](fn, timestamps); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	case "count":
		if counts, _, err := opts.WhenCount(); err != nil {
			ReportError(fn, opts, err)
		} else {
			if err := SaveToFile[types.TimestampCount](fn, counts); err != nil {
				ReportError2(fn, err)
			} else {
				ReportOkay(fn)
			}
		}
	}
}
