package names

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
	"github.com/spf13/cobra"
)

func Run(cmd *cobra.Command, args []string) {
	options := ""
	if Options.Expand {
		options += " --expand"
	}
	if Options.Match_Case {
		options += " --match_case"
	}
	if Options.All {
		options += " --all"
	}
	if Options.Custom {
		options += " --custom"
	}
	if Options.Prefund {
		options += " --prefund"
	}
	if Options.Named {
		options += " --named"
	}
	if Options.Addr {
		options += " --addr"
	}
	if Options.Collections {
		options += " --collections"
	}
	if Options.Tags {
		options += " --tags"
	}
	if Options.To_Custom {
		options += " --to_custom"
	}
	if Options.Clean {
		options += " --clean"
	}
	if Options.Create {
		options += " --create"
	}
	if Options.Delete {
		options += " --delete"
	}
	if Options.Update {
		options += " --update"
	}
	if Options.Remove {
		options += " --remove"
	}
	if Options.Undelete {
		options += " --undelete"
	}
	if len(Options.Autoname) > 0 {
		options += " --autoname " + Options.Autoname
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	root.PassItOn("ethNames", options, arguments)
}
