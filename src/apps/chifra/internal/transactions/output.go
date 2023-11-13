// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package transactionsPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTransactions handles the transactions command for the command line. Returns error only as per cobra.
func RunTransactions(cmd *cobra.Command, args []string) error {
	opts := transactionsFinishParse(args)
	outputHelpers.EnableCommand("transactions", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("transactions", &opts.Globals)
	return opts.TransactionsInternal()
}

// ServeTransactions handles the transactions command for the API. Returns an error.
func ServeTransactions(w http.ResponseWriter, r *http.Request) error {
	opts := transactionsFinishParseApi(w, r)
	outputHelpers.EnableCommand("transactions", true)
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("transactions", w, &opts.Globals)
	err := opts.TransactionsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("transactions", err, &opts.Globals)
	return err
}

// TransactionsInternal handles the internal workings of the transactions command.  Returns an error.
func (opts *TransactionsOptions) TransactionsInternal() error {
	var err error
	if err = opts.validateTransactions(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra transactions"
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache()

	} else if opts.Logs {
		err = opts.HandleLogs()

	} else if opts.Source {
		err = opts.HandleSource()

	} else if opts.Uniq {
		err = opts.HandleUniq()

	} else if len(opts.AccountFor) > 0 {
		err = opts.HandleAccounting()

	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return err
}

// GetTransactionsOptions returns the options for this tool so other tools may use it.
func GetTransactionsOptions(args []string, g *globals.GlobalOptions) *TransactionsOptions {
	ret := transactionsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

// EXISTING_CODE
// EXISTING_CODE
