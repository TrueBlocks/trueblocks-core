package scrape

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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/cmd/root"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/spf13/cobra"
)

// TODO: this is a much more elegant way to do error strings:
// TODO: https://github.com/storj/uplink/blob/v1.7.0/bucket.go#L19

func Validate(cmd *cobra.Command, args []string) error {
	if len(args) == 0 {
		return validate.Usage("Please choose one of [indexer|monitors|both]")

	} else {
		for _, arg := range args {
			err := validate.ValidateEnum("mode", arg, "[indexer|monitors|both]")
			if err != nil {
				return err
			}
		}
	}

	if Options.Action == "" {
		Options.Action = "run"
	}
	err := validate.ValidateEnum("action", Options.Action, "[toggle|run|restart|pause|quit]")
	if err != nil {
		return err
	}

	if Options.Sleep < .5 {
		return validate.Usage("Values less that .5 seconds for --sleep are not allowed.")
	}

	if Options.Pin && !hasIndexerFlag(args[0]) {
		return validate.Usage("The --pin option is only available with the indexer.")
	}

	if Options.Publish && !hasIndexerFlag(args[0]) {
		return validate.Usage("The --publish option only works with the indexer.")
	}

	err = root.ValidateGlobals(cmd, args)
	if err != nil {
		return err
	}

	return nil
}
