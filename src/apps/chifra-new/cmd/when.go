package cmd

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
	"fmt"
	"os"

	"github.com/spf13/cobra"
)

// whenCmd represents the when command
var whenCmd = &cobra.Command{
	Use:   "when",
	Short: "Find block(s) based on date, blockNum, timestamp, or 'special'",
	Long:  ``,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("when called")
	},
}

func init() {
	rootCmd.AddCommand(whenCmd)

	whenCmd.SetHelpTemplate(getHelpTextWhen())
	whenCmd.Flags().BoolP("list", "l", false, "export a list of the 'special' blocks")
	whenCmd.Flags().BoolP("timestamps", "t", false, "ignore other options and generate timestamps only")
}

func getHelpTextWhen() string {
	debugText := ""
	if os.Getenv("TEST_MODE") == "true" {
		debugText = `chifra argc: 5 [1:when] [2:--help] [3:--verbose] [4:2] 
chifra when --help --verbose 2 
chifra when argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra when --help --verbose 2 
PROG_NAME = [chifra when]
`
	}
	helpText := `
  Usage:    chifra when [-l|-t|-v|-h] < block | date > [ block... | date... ]  
  Purpose:  Find block(s) based on date, blockNum, timestamp, or 'special'.

  Where:
    blocks                one or more dates, block numbers, hashes, or special named blocks (see notes)
    -l  (--list)          export a list of the 'special' blocks
    -t  (--timestamps)    ignore other options and generate timestamps only

    #### Hidden options
    -c  (--check)         available only with --timestamps, checks the validity of the timestamp data
    -f  (--fix)           available only with --timestamps, fixes incorrect timestamps if any
    -u  (--count)         available only with --timestamps, returns the number of timestamps in the cache
    -s  (--skip <num>)    only applicable if --timestamps is on, the step between block numbers in the export
    #### Hidden options

    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - The block list may contain any combination of number, hash, date, special named blocks.
    - Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].

  Powered by TrueBlocks
`
	return debugText + helpText
}
