// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package namesPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *NamesOptions) validateNames() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	isDryRunnable := opts.Clean || len(opts.Autoname) > 0
	if opts.DryRun && !isDryRunnable {
		return validate.Usage("The {0} option is is only available with the {1} options.", "--dry_run", "--clean or --autoname")
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

	if opts.Prefund && (opts.Clean || len(opts.Autoname) > 0 || opts.anyCrud()) {
		return validate.Usage("You may not use the {0} option when editing names.", "--prefund")
	}

	// TODO: BOGUS
	// if opts.anyCrud() {
	// err := validate.ValidateAtLeastOneAddr(opts.Terms)
	// if err != nil {
	// 	return err
	// }
	// }

	addr := base.HexToAddress(opts.Autoname)
	zero := addr.IsZero()
	if strings.Contains(opts.Autoname, ".eth") {
		zero = false
	}
	if len(opts.Autoname) > 0 && (!base.IsValidAddress(opts.Autoname) || zero) {
		return validate.Usage("You must provide an address to the {0} option.", "--autoname")
	}

	return opts.Globals.Validate()
}
