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
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTransactions handles the transactions command for the command line. Returns error only as per cobra.
func RunTransactions(cmd *cobra.Command, args []string) (err error) {
	opts := transactionsFinishParse(args)
	outputHelpers.SetEnabledForCmds("transactions", opts.IsPorted())
	outputHelpers.SetWriterForCommand("transactions", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.TransactionsInternal()
	return
}

// ServeTransactions handles the transactions command for the API. Returns error and a bool if handled
func ServeTransactions(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := transactionsFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("transactions", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("transactions", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.TransactionsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("transactions", err, &opts.Globals)
	return
}

// TransactionsInternal handles the internal workings of the transactions command.  Returns error and a bool if handled
func (opts *TransactionsOptions) TransactionsInternal() (err error, handled bool) {
	err = opts.validateTransactions()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.IsPorted() {
		if opts.Decache {
			return opts.HandleDecache(), true
		} else if opts.Source {
			return opts.HandleSource(), true
		} else if opts.Uniq {
			return opts.HandleUniq()
		} else if len(opts.AccountFor) > 0 {
			return opts.HandleAccountFor()
		} else {
			return opts.HandleShowTxs(), true
		}
	}

	if opts.Globals.IsApiMode() {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getTrans", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
	// EXISTING_CODE

	return
}

// GetTransactionsOptions returns the options for this tool so other tools may use it.
func GetTransactionsOptions(args []string, g *globals.GlobalOptions) *TransactionsOptions {
	ret := transactionsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *TransactionsOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	if opts.Decache || opts.Uniq {
		return true
	}

	ported = !opts.Cache && len(opts.AccountFor) == 0
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
