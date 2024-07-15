// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package receiptsPkg

// EXISTING_CODE
import (
	"net/http"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/internal/globals"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	outputHelpers "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output/helpers"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunReceipts handles the receipts command for the command line. Returns error only as per cobra.
func RunReceipts(cmd *cobra.Command, args []string) error {
	opts := receiptsFinishParse(args)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.SetWriterForCommand("receipts", &opts.Globals)
	return opts.ReceiptsInternal(rCtx)
}

// ServeReceipts handles the receipts command for the API. Returns an error.
func ServeReceipts(w http.ResponseWriter, r *http.Request) error {
	opts := receiptsFinishParseApi(w, r)
	rCtx := output.NewRenderContext()
	// EXISTING_CODE
	// EXISTING_CODE
	outputHelpers.InitJsonWriterApi("receipts", w, &opts.Globals)
	err := opts.ReceiptsInternal(rCtx)
	outputHelpers.CloseJsonWriterIfNeededApi("receipts", err, &opts.Globals)
	return err
}

// ReceiptsInternal handles the internal workings of the receipts command. Returns an error.
func (opts *ReceiptsOptions) ReceiptsInternal(rCtx *output.RenderCtx) error {
	var err error
	if err = opts.validateReceipts(); err != nil {
		return err
	}

	timer := logger.NewTimer()
	msg := "chifra receipts"
	// EXISTING_CODE
	// EXISTING_CODE
	if opts.Globals.Decache {
		err = opts.HandleDecache()
	} else {
		err = opts.HandleShow()
	}
	timer.Report(msg)

	return err
}

// GetReceiptsOptions returns the options for this tool so other tools may use it.
func GetReceiptsOptions(args []string, g *globals.GlobalOptions) *ReceiptsOptions {
	ret := receiptsFinishParse(args)
	if g != nil {
		ret.Globals = *g
	}
	return ret
}
