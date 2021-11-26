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
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

// EXISTING_CODE
import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	quotesPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/quotes"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// quotesCmd represents the quotes command
var quotesCmd = &cobra.Command{
	Use:     usageQuotes,
	Short:   shortQuotes,
	Long:    longQuotes,
	Version: "GHC-TrueBlocks//0.16.1-alpha",
	RunE:    quotesPkg.RunQuotes,
}

var usageQuotes = `quotes [flags]`

var shortQuotes = "freshen or display Ethereum price data. This tool has been deprecated"

var longQuotes = `Purpose:
  Freshen or display Ethereum price data. This tool has been deprecated.`

var notesQuotes = `
Notes:
  - Due to restrictions from Poloniex, this tool retrieves only 30 days of data at a time. You must repeatedly run this command until the data is up-to-date.`

func init() {
	quotesCmd.Flags().SortFlags = false

	quotesCmd.Flags().BoolVarP(&quotesPkg.Options.Update, "update", "u", false, "freshen price database")
	quotesCmd.Flags().StringVarP(&quotesPkg.Options.Period, "period", "p", "", `increment of display
One of [ 5 | 15 | 30 | 60 | 120 | 240 | 1440 | 10080 | hourly | daily | weekly ]`)
	quotesCmd.Flags().StringVarP(&quotesPkg.Options.Pair, "pair", "a", "", "which price pair to freshen or list (see Poloniex)")
	quotesCmd.Flags().StringVarP(&quotesPkg.Options.Feed, "feed", "e", "", `the feed for the price data
One of [ poloniex | maker | tellor ]`)
	globals.InitGlobals(quotesCmd, &quotesPkg.Options.Globals)

	quotesCmd.SetUsageTemplate(UsageWithNotes(notesQuotes))
	quotesCmd.SetOut(os.Stderr)

	chifraCmd.AddCommand(quotesCmd)
}
