package slurpPkg

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleShowAppearances() error {
	paginator := Paginator{
		page:    1,
		perPage: int(opts.PerPage),
	}
	if opts.Globals.TestMode {
		paginator.perPage = 100
	}

	chain := opts.Globals.Chain
	logger.Info("Processing", opts.Addrs, "--types:", opts.Types, opts.Blocks)

	// TODO: Why does this have to dirty the caller?
	settings := rpcClient.DefaultRpcOptionsSettings{
		Chain: chain,
		Opts:  opts,
	}
	opts.Conn = settings.DefaultRpcOptions()
	if !opts.Conn.Store.ReadOnly() {
		opts.Conn.LatestBlockTimestamp = opts.Conn.GetBlockTimestamp(chain, nil)
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		totalFetched := 0
		totalFiltered := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				done := false
				for !done {
					txs, nFetched, err := opts.GetTransactionsFromEtherscan(chain, addr, tt, &paginator)
					done = nFetched < paginator.perPage
					totalFetched += nFetched
					if err != nil {
						errorChan <- err
						continue
					}

					for _, tx := range txs {
						tx := tx
						if !opts.isInRange(uint(tx.BlockNumber), errorChan) {
							continue
						}
						modelChan <- &types.SimpleAppearance{
							Address:          base.HexToAddress(addr),
							BlockNumber:      uint32(tx.BlockNumber),
							TransactionIndex: uint32(tx.TransactionIndex),
							Timestamp:        tx.Timestamp,
						}
						totalFiltered++
					}

					// Without this Etherscan chokes
					sleep := opts.Sleep
					if sleep > 0 {
						ms := time.Duration(sleep*1000) * time.Millisecond
						if !opts.Globals.TestMode {
							logger.Info(fmt.Sprintf("Sleeping for %g seconds", sleep))
						}
						time.Sleep(ms)
					}
				}
			}
		}

		if totalFiltered == 0 {
			msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
			errorChan <- fmt.Errorf(msg)
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
