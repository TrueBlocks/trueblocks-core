// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package receiptsPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunReceipts handles the receipts command for the command line. Returns error only as per cobra.
func RunReceipts(cmd *cobra.Command, args []string) (err error) {
	opts := receiptsFinishParse(args)
	outputHelpers.SetEnabledForCmds("receipts", opts.IsPorted())
	outputHelpers.SetWriterForCommand("receipts", &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ReceiptsInternal()
	return
}

// ServeReceipts handles the receipts command for the API. Returns error and a bool if handled
func ServeReceipts(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := receiptsFinishParseApi(w, r)
	outputHelpers.SetEnabledForCmds("receipts", opts.IsPorted())
	outputHelpers.InitJsonWriterApi("receipts", w, &opts.Globals)
	// EXISTING_CODE
	// EXISTING_CODE
	err, handled = opts.ReceiptsInternal()
	outputHelpers.CloseJsonWriterIfNeededApi("receipts", err, &opts.Globals)
	return
}

// ReceiptsInternal handles the internal workings of the receipts command.  Returns error and a bool if handled
func (opts *ReceiptsOptions) ReceiptsInternal() (err error, handled bool) {
	err = opts.validateReceipts()
	if err != nil {
		return err, true
	}

	timer := logger.NewTimer()
	msg := "chifra receipts"
	// EXISTING_CODE
	handled = true
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else {
		err = opts.HandleShow()
	}
	// EXISTING_CODE
	timer.Report(msg)

	return
}

// GetReceiptsOptions returns the options for this tool so other tools may use it.
func GetReceiptsOptions(args []string, g *globals.GlobalOptions) *ReceiptsOptions {
	ret := receiptsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}

func (opts *ReceiptsOptions) IsPorted() (ported bool) {
	// EXISTING_CODE
	ported = true
	// EXISTING_CODE
	return
}

// EXISTING_CODE
// EXISTING_CODE
