package slurpPkg

import (
	"context"
	"fmt"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleCount() error {
	testMode := opts.Globals.TestMode
	paginator := rpc.Paginator{
		Page:    1,
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
				paginator.Page = 1
				done := false

				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: !testMode, // && !utils.IsTerminal(),
					Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, addr), tt),
				})

				for !done {
					source := rpc.Etherscan
					if opts.Source == "key" {
						source = rpc.Key
					}
					txs, nFetched, err := opts.Conn.SlurpTxsByAddress(opts.Globals.Chain, source, addr, tt, &paginator)
					done = nFetched < paginator.PerPage
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
						bar.Tick()
						totalFiltered++
					}

					// Without this Etherscan chokes
					sleep := opts.Sleep
					if sleep > 0 {
						ms := time.Duration(sleep*1000) * time.Millisecond
						logger.Progress(!opts.Globals.TestMode, fmt.Sprintf("Sleeping for %g seconds", sleep))
						time.Sleep(ms)
					}
				}
				bar.Finish(true /* newLine */)
			}

			if totalFiltered == 0 {
				msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
				errorChan <- fmt.Errorf(msg)
			} else {
				s := types.SimpleMonitor{
					Address:  addr,
					NRecords: totalFiltered,
					FileSize: int64(totalFetched),
				}
				if testMode {
					s.FileSize = 0xdead
				}
				modelChan <- &s
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

// const maxTestingBlock = 17000000
