// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"log"
	"path/filepath"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) validateMonitors() error {
	chain := opts.Globals.Chain

	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.List {
		// All other options are ignored

	} else {
		if opts.Watch {
			if opts.Globals.IsApiMode() {
				return validate.Usage("The {0} options is not available from the API", "--watch")
			}

			// phonied up just to make sure we have at least one bloom filter
			var expOpts exportPkg.ExportOptions
			expOpts.Addrs = append(expOpts.Addrs, "0x0000000000000000000000000000000000000001")
			expOpts.Globals.Chain = chain
			expOpts.Conn = expOpts.Globals.FinishParse([]string{}, map[string]bool{})
			err := expOpts.Validate()
			if err != nil {
				return validate.Usage(err.Error())
			}

			// The user must have specified a command file -- there is no default
			if len(opts.Globals.File) == 0 {
				return validate.Usage("The {0} option is required with the {1} option.", "--file <cmd_file>", "--watch")
			} else {
				// Clean it up if it exists
				if cmdFile, err := filepath.Abs(opts.Globals.File); err != nil {
					return validate.Usage("The file you specified ({0}) could not be found.", opts.Globals.File)
				} else {
					opts.Globals.File = cmdFile
				}
				if file.FileSize(opts.Globals.File) == 0 {
					log.Fatal(validate.Usage("The file you specified ({0}) was found but contained no commands.", opts.Globals.File).Error())
				}
			}

		} else {
			if opts.Sleep != 14 {
				return validate.Usage("The {0} option is not available{1}.", "--sleep", " in non-watch mode")
			}

			// We validate some of the simpler curd commands here and the rest in HandleCrudCommands
			if opts.Undelete {
				if opts.Delete || opts.Remove {
					return validate.Usage("The --undelete option may not be used with --delete or --remove.")
				}
			}

			if !opts.Clean && len(opts.Addrs) == 0 {
				return validate.Usage("You must provide at least one Ethereum address for this command.")
			}

			if !opts.Clean && !opts.Delete && !opts.Undelete && !opts.Remove {
				return validate.Usage("Please provide either --clean or one of the CRUD commands.")
			}

			if !opts.Globals.IsApiMode() && !opts.Clean {
				if len(opts.Globals.File) > 0 {
					// Do nothing
				} else {
					err := validate.ValidateAtLeastOneAddr(opts.Addrs)
					if err != nil {
						return err
					}
				}
			}
		}
	}

	return opts.Globals.Validate()
}
