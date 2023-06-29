package receiptsPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *ReceiptsOptions) HandleShowReceipts() error {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
		// TODO: stream transaction identifiers
		for idIndex, rng := range opts.TransactionIds {
			txList, err := rng.ResolveTxs(opts.Globals.Chain)
			if err != nil {
				errorChan <- err
				if errors.Is(err, ethereum.NotFound) {
					continue
				}
				cancel()
				return
			}

			for _, tx := range txList {
				// TODO(cache): Can this be hidden behind the GetTransactionReceipt interface. No reason
				// TODO(cache): for this calling code to know the data is in the cache.
				// Try to load receipt from cache
				// TODO(cache): We should not be sending chain here. We have enough information to fully resolve the path at this level. Send only path.
				transaction, _ := cache.GetTransaction(
					opts.Globals.Chain,
					uint64(tx.BlockNumber),
					uint64(tx.TransactionIndex),
				)

				if transaction != nil && transaction.Receipt != nil {
					// Some values are not cached
					transaction.Receipt.BlockNumber = uint64(tx.BlockNumber)
					transaction.Receipt.TransactionHash = transaction.Hash
					transaction.Receipt.TransactionIndex = uint64(tx.TransactionIndex)
					modelChan <- transaction.Receipt
					continue
				}

				// TODO: Why does this interface always accept nil and zero at the end?
				receipt, err := rpcClient.GetTransactionReceipt(opts.Globals.Chain, rpcClient.ReceiptQuery{
					Bn:      uint64(tx.BlockNumber),
					Txid:    uint64(tx.TransactionIndex),
					NeedsTs: true,
				})

				if err != nil {
					if errors.Is(err, ethereum.NotFound) {
						errorChan <- fmt.Errorf("transaction at %s returned an error: %s", opts.Transactions[idIndex], ethereum.NotFound)
						continue
					}
					errorChan <- err
					cancel()
					return
				}

				if opts.Articulate {
					if err = abiCache.ArticulateReceipt(chain, &receipt); err != nil {
						errorChan <- err // continue even with an error
					}
				}

				modelChan <- &receipt
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
