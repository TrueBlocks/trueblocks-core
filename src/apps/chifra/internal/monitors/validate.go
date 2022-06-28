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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
)

func (opts *MonitorsOptions) ValidateMonitors() error {
	opts.TestLog()

	if opts.BadFlag != nil {
		return opts.BadFlag
	}

	if opts.Watch {
		// phonied up just to make sure we have bloom for block zero
		var expOpts exportPkg.ExportOptions
		expOpts.Addrs = append(expOpts.Addrs, "0x0000000000000000000000000000000000000001")
		expOpts.Globals.Chain = opts.Globals.Chain
		err := expOpts.ValidateExport()
		if err != nil {
			return validate.Usage(err.Error())
		}

		cmdFile := opts.Globals.File
		if !file.FileExists(cmdFile) {
			dir, _ := os.Getwd()
			cmdFile = filepath.Join(dir, opts.Globals.File)
		}

		if file.FileExists(cmdFile) {
			contents := utils.AsciiFileToString(cmdFile)
			cmds := strings.Split(contents, "\n")
			if len(cmds) == 0 {
				return validate.Usage("The command file you specified ({0}) was found but contained no commands.", cmdFile)
			}
		}
	} else {
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

		if !opts.Globals.ApiMode && !opts.Clean {
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

	return opts.Globals.ValidateGlobals()
}
