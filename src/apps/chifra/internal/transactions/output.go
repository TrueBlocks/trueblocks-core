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

	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTransactions handles the transactions command for the command line. Returns error only as per cobra.
func RunTransactions(cmd *cobra.Command, args []string) (err error) {
	opts := TransactionsFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.TransactionsInternal()
	return
}

// ServeTransactions handles the transactions command for the API. Returns error and a bool if handled
func ServeTransactions(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := TransactionsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.TransactionsInternal()
}

// TransactionsInternal handles the internal workings of the transactions command.  Returns error and a bool if handled
func (opts *TransactionsOptions) TransactionsInternal() (err error, handled bool) {
	err = opts.ValidateTransactions()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getTrans", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
