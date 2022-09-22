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
	"fmt"
	"net/http"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

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
	if opts.Globals.ApiMode {
		return nil, false
	}
	if opts.Articulate {
		err = opts.Globals.PassItOn("getReceipts", opts.Globals.Chain, opts.toCmdLine(), opts.getEnvStr())
		return err, true
	}

	notFound := make([]error, 0)
	defer func() {
		for _, err := range notFound {
			fmt.Fprintln(os.Stderr, err)
		}
	}()
	clientVersion, err := rpcClient.GetVersion(opts.Globals.Chain)
	if err != nil {
		return err, true
	}
	erigonUsed := utils.IsClientErigon(clientVersion)

	getTransaction := func(models chan types.Modeler[types.RawReceipt], errors chan error) {
		// TODO: stream transaction identifiers
		for idIndex, rng := range opts.TransactionIds {
			txList, err := rng.ResolveTxs(opts.Globals.Chain)
			// TODO: rpcClient should return a custom type of error in this case
			if err != nil && strings.Contains(err.Error(), "not found") {
				notFound = append(notFound, err)
				continue
			}
			if err != nil {
				errors <- err
				return
			}
			for _, tx := range txList {
				if tx.BlockNumber < uint32(byzantiumBlockNumber) && !erigonUsed {
					err = opts.Globals.PassItOn("getReceipts", opts.Globals.Chain, getReceiptsCmdLine(opts, []string{rng.Orig}), opts.getEnvStr())
					if err != nil {
						errors <- err
						return
					}
					continue
				}

				receipt, err := rpcClient.GetTransactionReceipt(opts.Globals.Chain, uint64(tx.BlockNumber), uint64(tx.TransactionIndex))
				if err != nil && err.Error() == "not found" {
					notFound = append(notFound, fmt.Errorf("transaction %s not found", opts.Transactions[idIndex]))
					continue
				}
				if err != nil {
					errors <- err
					return
				}

				models <- &receipt
			}
		}
	}
	var meta *rpcClient.MetaData
	if opts.Globals.Format == "api" {
		meta, err = rpcClient.GetMetaData(opts.Globals.Chain, opts.Globals.TestMode)
		if err != nil {
			return err, true
		}
	}
	err = output.StreamMany(opts.Globals.Writer, getTransaction, output.OutputOptions{
		ShowKeys:   !opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.Raw,
		ShowHidden: opts.Globals.Verbose,
		Format:     opts.Globals.Format,
		Meta:       meta,
	})
	handled = true
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
