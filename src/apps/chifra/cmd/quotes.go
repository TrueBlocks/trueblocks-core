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
 * Parts of this file were generated with makeClass --gocmds.
 */

import (
	"errors"
	"os"

	"github.com/spf13/cobra"
)

// quotesCmd represents the quotes command
var quotesCmd = &cobra.Command{
	Use:   usageQuotes,
	Short: shortQuotes,
	Long:  longQuotes,
	Run:   runQuotes,
	Args:  ValidatePositionals(validateQuotesArgs, cobra.MinimumNArgs(1)),
}

var usageQuotes = `quotes [flags]`

var shortQuotes = "freshen and/or display Ethereum price data"

var longQuotes = `Purpose:
  Freshen and/or display Ethereum price data.`

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
	quotesCmd.Flags().StringVarP(&QuotesOpts.period, "period", "p", "", "increment of display")
	quotesCmd.Flags().StringVarP(&QuotesOpts.pair, "pair", "a", "", "which price pair to freshen or list (see Poloniex)")
	quotesCmd.Flags().StringVarP(&QuotesOpts.feed, "feed", "e", "", "the feed for the price data")
	quotesCmd.Flags().SortFlags = false
	quotesCmd.PersistentFlags().SortFlags = false

	PostNotes = "[{POSTNOTES}]"
	rootCmd.AddCommand(quotesCmd)
}

func runQuotes(cmd *cobra.Command, args []string) {
	options := ""
	if QuotesOpts.freshen {
		options += " --freshen"
	}
	if len(QuotesOpts.period) > 0 {
		options += " --period " + QuotesOpts.period
	}
	if len(QuotesOpts.pair) > 0 {
		options += " --pair " + QuotesOpts.pair
	}
	if len(QuotesOpts.feed) > 0 {
		options += " --feed " + QuotesOpts.feed
	}
	arguments := ""
	for _, arg := range args {
		arguments += " " + arg
	}
	PassItOn("/Users/jrush/.local/bin/chifra/getQuotes", options, arguments)
}

func validateQuotesArgs(cmd *cobra.Command, args []string) error {
	if len(args) > 0 && args[0] == "12" {
		return ErrFunc(cmd, errors.New("Invalid argument "+args[0]))
	}
	return nil
}
