// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package configPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *ConfigOptions) validateConfig() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	err := validate.ValidateEnum("modes", opts.Mode, "[show|edit]")
	if err != nil {
		return err
	}

	if !opts.Globals.TestMode && opts.Mode == "edit" && os.Getenv("EDITOR") == "" {
		return validate.Usage("You must set the EDITOR environment variable to use the 'edit' mode.")
	}

	return opts.Globals.Validate()
}
