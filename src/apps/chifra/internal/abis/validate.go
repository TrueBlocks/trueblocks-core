package abisPkg

/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

func Validate(cmd *cobra.Command, args []string) error {
	output.Format = Options.Globals.Format
	Options.Addrs = args
	return Options.ValidateOptionsAbis()
}

func (opts *AbisOptionsType) ValidateOptionsAbis() error {
	if opts.Classes {
		return validate.Usage("The {0} option is not available{1}.", "--classes", " (not implemented)")
	}

	if len(opts.Find) == 0 && !opts.Known {
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	if len(opts.Find) == 0 && !opts.Known {
		err := validate.ValidateAtLeastOneAddr(opts.Addrs)
		if err != nil {
			return err
		}
	}

	if opts.Sol && len(opts.Find) > 0 {
		return validate.Usage("Please choose only one of {0}.", "--sol or --find")
	}

	if opts.Sol {
		for _, sol := range opts.Addrs {
			if sol == "" {
				continue
			}
			cleaned := "./" + strings.Replace(sol, ".sol", "", 1) + ".sol"
			if !utils.FileExists(cleaned) {
				return validate.Usage("The {0} option ({1}) must {2}", "file", cleaned, "exist")
			}
		}
	} else {
		for _, term := range opts.Find {
			ok1, err1 := validate.IsValidFourByte(term)
			if !ok1 && len(term) < 10 {
				return err1
			}
			ok2, err2 := validate.IsValidTopic(term)
			if !ok2 && len(term) > 66 {
				return err2
			}
			if !ok1 && !ok2 {
				if len(term) > 43 {
					// more than halfway reports topic
					return err2
				}
				return err1
			} else {
				validate.Errors = nil
			}
		}

	}

	opts.TestLog()

	return globals.ValidateGlobals(&opts.Globals, opts.Addrs)
}
