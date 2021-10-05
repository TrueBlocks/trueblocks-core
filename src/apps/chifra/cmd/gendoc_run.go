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
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/validate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
	"github.com/spf13/cobra/doc"
)

func validateGendocArgs(cmd *cobra.Command, args []string) error {
	if !utils.FolderExists("../build/docs/") {
		cwd, _ := os.Getwd()
		return validate.Usage("Cannot find local ./docs folder in {0}", cwd)
	}

	err := validate.ValidateGlobalFlags(RootOpts.file, RootOpts.fmt, cmd, args)
	if err != nil {
		return err
	}

	return nil
}

func runGendoc(cmd *cobra.Command, args []string) {
	err := doc.GenMarkdownTree(rootCmd, "../build/docs/")
	if err != nil {
		log.Fatal(err)
	}
	for _, arg := range args {
		fn := "../build/docs/chifra_" + arg + ".md"
		if utils.FileExists(fn) {
			contents := utils.AsciiFileToString(fn)
			contents = strings.Replace(contents, "\n### SEE ALSO\n", "[{BB}]", -1)
			contents = strings.Replace(contents, "### Synopsis", "[{BB}]##### Synopsis", -1)
			contents = strings.Replace(contents, "### Options inherited from parent commands", "##### Global Flags", -1)
			contents = strings.Replace(contents, "### Options", "##### Flags", -1)
			contents = strings.Replace(contents, "Purpose:\n", "", -1)
			contents = strings.Replace(contents, "\nArguments:\n", "```\n\n##### Arguments:\n\n```\n", -1)
			contents = strings.Replace(contents, "```\nchifra", "```\n  chifra", -1)
			fmt.Printf("%s", strings.Split(contents, "[{BB}]")[1])
		}
	}
}
