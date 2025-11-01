// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package transactionsPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunTransactions handles the transactions command for the command line. Returns error only as per cobra.
func RunTransactions(cmd *cobra.Command, args []string) error {
	_ = cmd
	opts := transactionsFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("transactions", &opts.Globals)
	return opts.TransactionsInternal(rCtx)
}

// ServeTransactions handles the transactions command for the API. Returns an error.
func ServeTransactions(w http.ResponseWriter, r *http.Request) error {
	opts := transactionsFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("transactions", w, &opts.Globals)
	err := opts.TransactionsInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("transactions", err, &opts.Globals)
	return err
}

// TransactionsInternal handles the internal workings of the transactions command. Returns an error.
func (opts *TransactionsOptions) TransactionsInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateTransactions(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra transactions"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache(rCtx)
	} else if opts.Logs {
		err = opts.HandleLogs(rCtx)
	} else if opts.Uniq {
		err = opts.HandleUniq(rCtx)
	} else {
		err = opts.HandleShow(rCtx)
	}
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
