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
	"context"
	"fmt"
	"net/http"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunReceipts handles the receipts command for the command line. Returns error only as per cobra.
func RunReceipts(cmd *cobra.Command, args []string) (err error) {
	opts := receiptsFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ReceiptsInternal()
	return
}

// ServeReceipts handles the receipts command for the API. Returns error and a bool if handled
func ServeReceipts(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := receiptsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.ReceiptsInternal()
}

// ReceiptsInternal handles the internal workings of the receipts command.  Returns error and a bool if handled
func (opts *ReceiptsOptions) ReceiptsInternal() (err error, handled bool) {
	err = opts.validateReceipts()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	if opts.Articulate || opts.Globals.ToFile {
		err = opts.Globals.PassItOn("getReceipts", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
		return err, true
	}

	clientVersion, err := rpcClient.GetVersion(opts.Globals.Chain)
	if err != nil {
		return err, true
	}
	erigonUsed := utils.IsClientErigon(clientVersion)

	ctx, cancel := context.WithCancel(context.Background())

	fetchTransactionData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		// TODO: stream transaction identifiers
		for idIndex, rng := range opts.TransactionIds {
			txList, err := rng.ResolveTxs(opts.Globals.Chain)
			// TODO: rpcClient should return a custom type of error in this case
			if err != nil && strings.Contains(err.Error(), "not found") {
				errorChan <- err
				continue
			}
			if err != nil {
				errorChan <- err
				cancel()
				return
			}
			for _, tx := range txList {
				if tx.BlockNumber < uint32(byzantiumBlockNumber) && !erigonUsed {
					err = opts.Globals.PassItOn("getReceipts", opts.Globals.Chain, getReceiptsCmdLine(opts, []string{rng.Orig}), opts.getEnvStr())
					if err != nil {
						errorChan <- err
						cancel()
						return
					}
					continue
				}

				receipt, err := rpcClient.GetTransactionReceipt(opts.Globals.Chain, uint64(tx.BlockNumber), uint64(tx.TransactionIndex))
				if err != nil && err.Error() == "not found" {
					errorChan <- fmt.Errorf("transaction %s not found", opts.Transactions[idIndex])
					continue
				}
				if err != nil {
					errorChan <- err
					cancel()
					return
				}

				modelChan <- &receipt
			}
		}
	}

	err = output.StreamMany(ctx, fetchTransactionData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
	})

	handled = true
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// TODO: create EXISTING CODE block at the beginning of this file to keep constants ther
var byzantiumBlockNumber = 4370000

// TODO: remove this function when rewrite to Go is completed. It is only used to send
// pre-Byzantium transactions to C++ version
func getReceiptsCmdLine(opts *ReceiptsOptions, txs []string) string {
	options := ""
	if opts.Articulate {
		options += " --articulate"
	}

	options += " " + strings.Join(txs, " ")
	return options
}

// EXISTING_CODE
