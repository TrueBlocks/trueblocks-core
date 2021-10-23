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
/*
 * Parts of this file were generated with makeClass --gocmds. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package cmd

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/utils"
	"github.com/spf13/cobra"
)

// quotesCmd represents the quotes command
var quotesCmd = &cobra.Command{
	Use:   usageQuotes,
	Short: shortQuotes,
	Long:  longQuotes,
	Run:   runQuotes,
	Args:  validateQuotesArgs,
}

var usageQuotes = `quotes [flags]`

var shortQuotes = "freshen or display Ethereum price data. This tool has been deprecated"

var longQuotes = `Purpose:
  Freshen or display Ethereum price data. This tool has been deprecated.`

var notesQuotes = `
Notes:
  - Due to restrictions from Poloniex, this tool retrieves only 30 days of data at a time. You must repeatedly run this command until the data is up-to-date.`

type quotesOptionsType struct {
	freshen bool
	period  string
	pair    string
	feed    string
}

var QuotesOpts quotesOptionsType

func init() {
	quotesCmd.SetOut(os.Stderr)

	quotesCmd.Flags().SortFlags = false
	quotesCmd.PersistentFlags().SortFlags = false
	quotesCmd.Flags().BoolVarP(&QuotesOpts.freshen, "freshen", "f", false, "Freshen price database")
	quotesCmd.Flags().StringVarP(&QuotesOpts.period, "period", "p", "", `increment of display
One of [ 5 | 15 | 30 | 60 | 120 | 240 | 1440 | 10080 | hourly | daily | weekly ]`)
	quotesCmd.Flags().StringVarP(&QuotesOpts.pair, "pair", "a", "", "which price pair to freshen or list (see Poloniex)")
	quotesCmd.Flags().StringVarP(&QuotesOpts.feed, "feed", "e", "", `the feed for the price data
One of [ poloniex | maker | tellor ]`)
	quotesCmd.Flags().SortFlags = false
	quotesCmd.PersistentFlags().SortFlags = false

	quotesCmd.SetUsageTemplate(UsageWithNotes(notesQuotes))
	rootCmd.AddCommand(quotesCmd)
}

func TestLogQuotes(args []string) {
	if !utils.IsTestMode() {
		return
	}
}

// EXISTING_CODE
// EXISTING_CODE
