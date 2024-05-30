// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *GlobalOptions) OutputOpts() output.OutputOptions {
	extraOpts := map[string]any{
		"ether": opts.Ether,
	}

	return output.OutputOptions{
		Writer:     opts.Writer,
		Chain:      opts.Chain,
		TestMode:   opts.TestMode,
		NoHeader:   opts.NoHeader,
		Verbose:    opts.Verbose,
		Format:     opts.Format,
		OutputFn:   opts.OutputFn,
		Append:     opts.Append,
		JsonIndent: "  ",
		Extra:      extraOpts,
	}
}

func (opts *GlobalOptions) OutputOptsWithExtra(extraOpts map[string]any) output.OutputOptions {
	if extraOpts != nil {
		extraOpts["ether"] = opts.Ether
	}

	return output.OutputOptions{
		Writer:     opts.Writer,
		Chain:      opts.Chain,
		TestMode:   opts.TestMode,
		NoHeader:   opts.NoHeader,
		Verbose:    opts.Verbose,
		Format:     opts.Format,
		OutputFn:   opts.OutputFn,
		Append:     opts.Append,
		JsonIndent: "  ",
		Extra:      extraOpts,
	}
}

func (opts *GlobalOptions) ShowProgress() bool {
	if opts.TestMode || utils.IsFuzzing() {
		return false
	}
	return len(opts.OutputFn) > 0 || !logger.IsTerminal()
}

func (opts *GlobalOptions) ShowProgressNotTesting() bool {
	return !opts.TestMode && !utils.IsFuzzing()
}
