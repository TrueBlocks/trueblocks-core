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
package cmd

import (
	"os"

	"github.com/spf13/cobra"
)

type gendocOptionsType struct {
	readmes     bool
	completions bool
	manpages    bool
}

var GendocOpts gendocOptionsType

var gendocCmd = &cobra.Command{
	Use:   "gendoc",
	Short: "Generate docs",
	Run:   runGendoc,
	Args:  validateGendocArgs,
}

func init() {
	listCmd.SetOut(os.Stderr)
	gendocCmd.Flags().BoolVarP(&GendocOpts.readmes, "reamdes", "r", true, "generate readme docs")
	gendocCmd.Flags().BoolVarP(&GendocOpts.completions, "completions", "c", true, "generate completions")
	gendocCmd.Flags().BoolVarP(&GendocOpts.manpages, "manpages", "m", true, "generate man pages")
	gendocCmd.SetUsageTemplate(UsageWithNotes(""))
	rootCmd.AddCommand(gendocCmd)
}
