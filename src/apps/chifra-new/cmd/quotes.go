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

// quotesCmd represents the quotes command
var quotesCmd = &cobra.Command{
	Use:   "quotes",
	Short: "A brief description of your command",
	Long: `A longer description that spans multiple lines and likely contains examples
and usage of using your command. For example:

Cobra is a CLI library for Go that empowers applications.
This application is a tool to generate the needed files
to quickly create a Cobra application.`,
	Run: func(cmd *cobra.Command, args []string) {
		fmt.Println("quotes called")
	},
}

func init() {
	rootCmd.AddCommand(quotesCmd)
	quotesCmd.SetHelpTemplate(getHelpTextQuotes())
}

func getHelpTextQuotes() string {
	return `chifra argc: 5 [1:quotes] [2:--help] [3:--verbose] [4:2] 
chifra quotes --help --verbose 2 
chifra quotes argc: 4 [1:--help] [2:--verbose] [3:2] 
chifra quotes --help --verbose 2 
PROG_NAME = [chifra quotes]

  Usage:    chifra quotes [-f|-p|-a|-e|-v|-h]  
  Purpose:  Freshen and/or display Ethereum price data. This tool has been deprecated.

  Where:
    -f  (--freshen)       Freshen price database
    -p  (--period <val>)  increment of display, one of [5|15|30|60|120*|240|1440|10080|hourly|daily|weekly]
    -a  (--pair <str>)    which price pair to freshen or list (see Poloniex)
    -e  (--feed <val>)    the feed for the price data, one of [poloniex*|maker|tellor]
    -x  (--fmt <val>)     export format, one of [none|json*|txt|csv|api]
    -v  (--verbose)       set verbose level (optional level defaults to 1)
    -h  (--help)          display this help screen

  Notes:
    - Valid pairs include any pair from the public Poloniex's API here:
      https://poloniex.com/public?command=returnCurrencies.
    - Due to restrictions from Poloniex, this tool retrieves only 30 days of data
      at a time. You must repeatedly run this command until the data is up-to-date.

  Powered by TrueBlocks
`
}
