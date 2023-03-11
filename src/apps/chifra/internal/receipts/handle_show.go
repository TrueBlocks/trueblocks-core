package receiptsPkg

import (
	"context"
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum"
)

func (opts *ReceiptsOptions) HandleShow() error {

	abiMap := make(abi.AbiInterfaceMap)
	loadedMap := make(map[types.Address]bool)
	chain := opts.Globals.Chain

	clientVersion, err := rpcClient.GetVersion(opts.Globals.Chain)
	if err != nil {
		return err
	}
	erigonUsed := utils.IsClientErigon(clientVersion)

	ctx, cancel := context.WithCancel(context.Background())

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchTransactionData := func(modelChan chan types.Modeler[types.RawReceipt], errorChan chan error) {
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
				if tx.BlockNumber < uint32(byzantiumBlockNumber) && !erigonUsed {
					err = opts.Globals.PassItOn("getReceipts", opts.Globals.Chain, getReceiptsCmdLine(opts, []string{rng.Orig}), opts.getEnvStr())
					if err != nil {
						errorChan <- err
						cancel()
						return
					}
					continue
				}

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
				receipt, err := rpcClient.GetTransactionReceipt(opts.Globals.Chain, uint64(tx.BlockNumber), uint64(tx.TransactionIndex), nil, 0)
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
					for index, log := range receipt.Logs {
						var err error
						if !loadedMap[log.Address] {
							if err = abi.LoadAbi(chain, log.Address, abiMap); err != nil {
								// continue processing even with an error
								errorChan <- err
							}
						}
						if err == nil {
							receipt.Logs[index].ArticulatedLog, err = articulate.ArticulateLog(&log, abiMap)
							if err != nil {
								// continue processing even with an error
								errorChan <- err
							}
						}
					}
				}

				modelChan <- &receipt
			}
		}
	}

	return output.StreamMany(ctx, fetchTransactionData, output.OutputOptions{
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
		Extra: map[string]interface{}{
			"articulate": opts.Articulate,
		},
	})
}
