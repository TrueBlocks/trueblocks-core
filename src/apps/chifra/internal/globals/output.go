// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package globals

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *GlobalOptions) OutputOpts() output.OutputOptions {
	return output.OutputOptions{
		Writer:     opts.Writer,
		Chain:      opts.Chain,
		TestMode:   opts.TestMode,
		NoHeader:   opts.NoHeader,
		ShowRaw:    opts.ShowRaw,
		Verbose:    opts.Verbose,
		Format:     opts.Format,
		OutputFn:   opts.OutputFn,
		Append:     opts.Append,
		JsonIndent: "  ",
	}
}

func (opts *GlobalOptions) OutputOptsWithExtra(extra map[string]interface{}) output.OutputOptions {
	return output.OutputOptions{
		Writer:     opts.Writer,
		Chain:      opts.Chain,
		TestMode:   opts.TestMode,
		NoHeader:   opts.NoHeader,
		ShowRaw:    opts.ShowRaw,
		Verbose:    opts.Verbose,
		Format:     opts.Format,
		OutputFn:   opts.OutputFn,
		Append:     opts.Append,
		JsonIndent: "  ",
		Extra:      extra,
	}
}
