// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * This file was auto generated with makeClass --gocmds. DO NOT EDIT.
 */

package cmd

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
	Version: versionText,
	RunE:    quotesPkg.RunQuotes,
}

const usageQuotes = `quotes [flags]`

const shortQuotes = "update or display Ethereum price data, this tool has been deprecated"

const longQuotes = `Purpose:
  Update or display Ethereum price data, this tool has been deprecated.`

const notesQuotes = `
Notes:
  - Due to restrictions from Poloniex, this tool retrieves only 30 days of data at a time. You must repeatedly run this command until the data is up-to-date.`

func init() {
	quotesCmd.Flags().SortFlags = false

	quotesCmd.Flags().BoolVarP(&quotesPkg.GetOptions().Update, "update", "u", false, "freshen price database")
	quotesCmd.Flags().StringVarP(&quotesPkg.GetOptions().Period, "period", "p", "", `increment of display
One of [ 5 | 15 | 30 | 60 | 120 | 240 | 1440 | 10080 | hourly | daily | weekly ]`)
	quotesCmd.Flags().StringVarP(&quotesPkg.GetOptions().Pair, "pair", "a", "", "which price pair to freshen or list (see Poloniex)")
	quotesCmd.Flags().StringVarP(&quotesPkg.GetOptions().Feed, "feed", "e", "", `the feed for the price data
One of [ poloniex | maker | tellor ]`)
	globals.InitGlobals(quotesCmd, &quotesPkg.GetOptions().Globals)

	quotesCmd.SetUsageTemplate(UsageWithNotes(notesQuotes))
	quotesCmd.SetOut(os.Stderr)

	// EXISTING_CODE
	// EXISTING_CODE

	chifraCmd.AddCommand(quotesCmd)
}
