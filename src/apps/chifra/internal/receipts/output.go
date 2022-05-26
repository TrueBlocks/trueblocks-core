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
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/spf13/cobra"
)

// EXISTING_CODE

// RunReceipts handles the receipts command for the command line. Returns error only as per cobra.
func RunReceipts(cmd *cobra.Command, args []string) (err error) {
	opts := ReceiptsFinishParse(args)
	// EXISTING_CODE
	// EXISTING_CODE
	err, _ = opts.ReceiptsInternal()
	return
}

// ServeReceipts handles the receipts command for the API. Returns error and a bool if handled
func ServeReceipts(w http.ResponseWriter, r *http.Request) (err error, handled bool) {
	opts := ReceiptsFinishParseApi(w, r)
	// EXISTING_CODE
	// EXISTING_CODE
	return opts.ReceiptsInternal()
}

// ReceiptsInternal handles the internal workings of the receipts command.  Returns error and a bool if handled
func (opts *ReceiptsOptions) ReceiptsInternal() (err error, handled bool) {
	err = opts.ValidateReceipts()
	if err != nil {
		return err, true
	}

	// EXISTING_CODE
	newnew := os.Getenv("NEW") == "true"
	if !opts.Articulate && newnew {
		for _, rng := range opts.TransactionIds {
			txList, err := rng.ResolveTxs(opts.Globals.Chain)
			if err != nil {
				return err, true
			}
			var header types.SimpleReceipt
			err = opts.Globals.RenderHeader(header, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
			defer opts.Globals.RenderFooter(opts.Globals.ApiMode || opts.Globals.Format == "api")

			if err != nil {
				return err, true
			}

			for i, tx := range txList {
				provider := config.GetRpcProvider(opts.Globals.Chain)
				obj, err := rpcClient.GetTransactionReceipt(provider, uint64(tx.BlockNumber), uint64(tx.TransactionIndex))
				if err != nil {
					return err, true
				}
				err = opts.Globals.RenderObject(obj, false, i == 0)
				if err != nil {
					return err, true
				}
			}
			return nil, true
		}
		// var receipt rpcClient.Receipt
		// var txReceiptPl = rpcClient.RPCPayload{
		// 	Jsonrpc:   "2.0",
		// 	Method:    "eth_getTransactionReceipt",
		// 	RPCParams: rpcClient.RPCParams{traces.Result[i].TransactionHash},
		// 	ID:        1005,
		// }
		// err := rpcClient.FromRpc(rpcProvider, &txReceiptPl, &receipt)
		// if err != nil {
		// 	fmt.Println("FromRpc(transReceipt) returned error")
		// 	log.Fatal(err)
		// }
		// addr := receipt.Result.ContractAddress
		// if goodAddr(addr) {
		// 	addressMap[addr+blockAndIdx] = true
		// }
	}

	if opts.Globals.ApiMode {
		return nil, false
	}

	handled = true
	err = opts.Globals.PassItOn("getReceipts", opts.Globals.Chain, opts.ToCmdLine(), opts.Globals.ToCmdLine())
	// EXISTING_CODE

	return
}

// EXISTING_CODE
// EXISTING_CODE
