package logsPkg

import (
	"context"
	"errors"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum"
)

func (opts *LogsOptions) HandleShowLogs() (err error) {
	abiCache := articulate.NewAbiCache()
	chain := opts.Globals.Chain

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, rng := range opts.TransactionIds {
			txIds, err := rng.ResolveTxs(chain)
			if err != nil && !errors.Is(err, ethereum.NotFound) {
				errorChan <- err
				cancel()
			}

			for _, appearance := range txIds {
				tx, err := rpcClient.GetTransactionByAppearance(chain, &appearance, false /* no traces for logs */)
				if err != nil {
					errorChan <- fmt.Errorf("transaction at %s returned an error: %w", strings.Replace(rng.Orig, "-", ".", -1), err)
					continue
				}
				if tx == nil {
					errorChan <- fmt.Errorf("transaction at %s not found", strings.Replace(rng.Orig, "-", ".", -1))
					continue
				}

				for index := range tx.Receipt.Logs {
					tx.Receipt.Logs[index].Timestamp = tx.Timestamp
					if opts.Articulate {
						if err = abiCache.ArticulateLog(chain, &tx.Receipt.Logs[index]); err != nil {
							errorChan <- err // continue even with an error
						}
					}
					modelChan <- &tx.Receipt.Logs[index]
				}
			}
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
