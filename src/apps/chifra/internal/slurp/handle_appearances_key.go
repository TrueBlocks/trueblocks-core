package slurpPkg

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleAppearancesKey() error {
	testMode := opts.Globals.TestMode

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		totalFetched := 0
		totalFiltered := 0
		for _, addr := range opts.Addrs {
			paginator := rpc.NewPageIdPaginator("", opts.PerPageValue())

			bar := logger.NewBar(logger.BarOptions{
				Type:    logger.Expanding,
				Enabled: !testMode && !utils.IsTerminal(),
				Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, addr), "all"),
			})

			done := false
			for !done {
				txs, nFetched, err := opts.Conn.SlurpTxsByAddress(opts.Globals.Chain, opts.Source, addr, "not-used", paginator)
				nextPageErr := paginator.PreviousPage() // order matters
				if nextPageErr != nil {
					// we have reached the last page
					if nextPageErr == rpc.ErrPaginatorEmptyPreviousPage {
						done = true
					} else {
						// some other error happened
						errorChan <- nextPageErr
						return
					}
				}
				totalFetched += nFetched
				if err != nil {
					errorChan <- err
					continue
				}

				for _, tx := range txs {
					if ok, err := opts.isInRange(tx.BlockNumber); !ok {
						if err != nil {
							errorChan <- err
						}

						continue
					}
					bar.Tick()
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
					logger.Progress(true, fmt.Sprintf("Sleeping for %g seconds", sleep))
					time.Sleep(ms)
				}
			}

			bar.Finish(true /* newLine */)
		}

		if totalFiltered == 0 {
			msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
			errorChan <- fmt.Errorf(msg)
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
