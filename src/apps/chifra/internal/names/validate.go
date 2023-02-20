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

	if opts.Prefund && (opts.Clean || len(opts.Autoname) > 0 || opts.anyCrud()) {
		return validate.Usage("You may not use the {0} option when editing names.", "--prefund")
	}

	// Validate some of the simpler curd commands here and the rest in HandleCrud
	b2I := func(b bool) int {
		if b {
			return 1
		} else {
			return 0
		}
	}
	if b2I(opts.Undelete)+b2I(opts.Delete)+b2I(opts.Remove) > 1 {
		return validate.Usage("THe {0} option may not be used with {1} or {2}.", "--undelete", "--delete", "--remove")
	}

	if opts.anyCrud() {
		// TODO: BOGUS
		err := validate.ValidateAtLeastOneAddr(opts.Terms)
		if err != nil {
			return err
		}
	} else if opts.ToCustom {
		return validate.Usage("The {0} option is only available when editing names.", "--to_custom")
	}

	addr := types.HexToAddress(opts.Autoname)
	if len(opts.Autoname) > 0 && (!validate.IsValidAddress(opts.Autoname) || addr.IsZero()) {
		return validate.Usage("You must provide an address to the {0} option.", "--autoname")
	}

	return opts.Globals.Validate()
}
