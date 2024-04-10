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

func (opts *SlurpOptions) HandleCount() error {
	testMode := opts.Globals.TestMode
	paginator := rpc.Paginator{
		Page:    opts.FirstPage(),
		PerPage: int(opts.PerPage),
	}
	if opts.Globals.TestMode {
		paginator.PerPage = 100
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		totalFetched := 0
		totalFiltered := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				paginator.Page = opts.FirstPage()

				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: !testMode, // && !utils.IsTerminal(),
					Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, addr), tt),
				})

				done := false
				for !done {
					txs, nFetched, err := opts.Conn.SlurpTxsByAddress(opts.Globals.Chain, opts.Source, addr, tt, &paginator)
					totalFetched += nFetched
					paginator.Page++                    // order matters
					done = nFetched < paginator.PerPage // order matters
					if err != nil {                     // order matters
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
						totalFiltered++
					}

					// Without this Etherscan chokes
					if !done {
						sleep := opts.Sleep
						if sleep > 0 {
							ms := time.Duration(sleep*1000) * time.Millisecond
							logger.Progress(true, fmt.Sprintf("Sleeping for %g seconds", sleep))
							time.Sleep(ms)
						}
					}
				}
				bar.Finish(true /* newLine */)
			}

			if totalFiltered == 0 {
				msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
				errorChan <- fmt.Errorf(msg)
			} else {
				s := types.SimpleMonitor{
					Address:  base.HexToAddress(addr),
					NRecords: int64(totalFiltered),
				}
				modelChan <- &s
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// const maxTestingBlock = 17000000
