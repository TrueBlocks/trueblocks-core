// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *NamesOptions) validateNames() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Tags && (opts.Addr || opts.anyBase()) {
		return validate.Usage("The {0} option is not available{1}.", "--tags", " with any other option")
	}

	if opts.Addr && (opts.Tags || opts.anyBase()) {
		return validate.Usage("The {0} option is not available{1}.", "--addr", " with any other option")
	}

	if opts.MatchCase && len(opts.Terms) == 0 {
		return validate.Usage("The {0} option requires at least one {1}.", "--match_case", "term")
	}

	if opts.Named {
		return validate.Usage("The --named option has been deprecated. Use --all instead.")
	}

	if opts.Prefund && opts.anyCrud() {
		return validate.Usage("You may not use the {0} option when editing names.", "--prefund")
	}

	addr := types.HexToAddress(opts.Autoname)
	if len(opts.Autoname) > 0 && (!validate.IsValidAddress(opts.Autoname) || addr.IsZero()) {
		return validate.Usage("You must provide an address to the {0} option.", "--autoname")
	}

	return opts.Globals.Validate()
}

func (opts *NamesOptions) anyBase() bool {
	return opts.Expand ||
		opts.MatchCase ||
		opts.All ||
		opts.Prefund ||
		opts.Named ||
		opts.Clean
}

func (opts *NamesOptions) anyCrud() bool {
	return opts.Clean ||
		opts.Create ||
		opts.Update ||
		opts.Delete ||
		opts.Undelete ||
		opts.Remove ||
		len(opts.Autoname) > 0
}
