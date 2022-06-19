// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package initPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *InitOptions) ValidateInit() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	// TODO: Multi-chain missing feature on other chains
	if opts.Globals.Chain != "mainnet" {
		return validate.Usage("The {0} command is currently available only on the {1} chain.", "init", "mainnet")
	}

	return opts.Globals.ValidateGlobals()
}
