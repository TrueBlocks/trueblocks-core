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
	whenCmd.SetHelpTemplate(getHelpTextWhen())
	rootCmd.AddCommand(whenCmd)

	// Here you will define your flags and configuration settings.

	// Cobra supports Persistent Flags which will work for this command
	// and all subcommands, e.g.:
	// whenCmd.PersistentFlags().String("foo", "", "A help for foo")

	// Cobra supports local flags which will only run when this command
	// is called directly, e.g.:
	// whenCmd.Flags().BoolP("toggle", "t", false, "Help message for toggle")
}

func getHelpTextWhen() string {
	return `
  Usage:    chifra when [-l|-t|-v|-h] < block | date > [ block... | date... ]
  Purpose:  Find block(s) based on date, blockNum, timestamp, or 'special'.

  Where:
	block_list            one or more dates, block numbers, hashes, or special named blocks (see notes)
	-l  (--list)          export a list of the 'special' blocks
	-t  (--timestamps)    ignore other options and generate timestamps only
	-x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
	-v  (--verbose)       set verbose level (optional level defaults to 1)
	-h  (--help)          display this help screen

  Powered by TrueBlocks (GHC-TrueBlocks//0.12.1-alpha-7a8fdbb7f-20210922)`
}
