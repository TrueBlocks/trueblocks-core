// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package monitorsPkg

import (
	"os"
	"path/filepath"
	"strings"

	exportPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/export"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) validateMonitors() error {
	opts.testLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.List {
		// All other options are ignored

	} else {
		if opts.Watch {
			// phonied up just to make sure we have bloom for block zero
			var expOpts exportPkg.ExportOptions
			expOpts.Addrs = append(expOpts.Addrs, "0x0000000000000000000000000000000000000001")
			expOpts.Globals.Chain = opts.Globals.Chain
			err := expOpts.Validate()
			if err != nil {
				return validate.Usage(err.Error())
			}

			cmdFile := opts.Globals.File
			if !file.FileExists(cmdFile) {
				dir, _ := os.Getwd()
				cmdFile = filepath.Join(dir, opts.Globals.File)
			}

			if file.FileExists(cmdFile) {
				contents := file.AsciiFileToString(cmdFile)
				cmds := strings.Split(contents, "\n")
				if len(cmds) == 0 {
					return validate.Usage("The command file you specified ({0}) was found but contained no commands.", cmdFile)
				}
			}

			if opts.Globals.IsApiMode() {
				return validate.Usage("The {0} options is not available from the API", "--watch")
			}

		} else {
			if opts.Sleep != 14 {
				return validate.Usage("The {0} option is not available in non-watch mode.", "--sleep")
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

			if !opts.Clean && !opts.Delete && !opts.Undelete && !opts.Remove && !opts.Decache {
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

			if opts.Globals.IsApiMode() && opts.Decache {
				return validate.Usage("The {0} option is not available in API mode.", "--decache")
			}
		}
	}

	return opts.Globals.Validate()
}
