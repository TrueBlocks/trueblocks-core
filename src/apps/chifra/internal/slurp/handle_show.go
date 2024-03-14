package slurpPkg

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleShow() error {
	testMode := opts.Globals.TestMode
	paginator := rpc.Paginator{
		Page:    opts.FirstPage(),
		PerPage: int(opts.PerPage),
	}
	if opts.Globals.TestMode {
		paginator.PerPage = 100
	}

	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawSlurp], errorChan chan error) {
		totalFetched := 0
		totalFiltered := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				paginator.Page = opts.FirstPage()
				done := false
				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: !testMode && !utils.IsTerminal(),
					Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, addr), tt),
				})

				for !done {
					txs, nFetched, err := opts.Conn.SlurpTxsByAddress(opts.Globals.Chain, opts.Source, addr, tt, &paginator)
					paginator.Page++ // order matters
					done = nFetched < paginator.PerPage
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
						if opts.Articulate {
							if err = abiCache.ArticulateSlurp(&tx); err != nil {
								errorChan <- err // continue even with an error
							}
						}
						modelChan <- &tx
						bar.Tick()
						totalFiltered++
					}

					if !done {
						// Without this Etherscan chokes
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
		}

		if totalFiltered == 0 {
			msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
			errorChan <- fmt.Errorf(msg)
		}
	}

	extra := map[string]interface{}{
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *SlurpOptions) isInRange(bn base.Blknum) (bool, error) {
	if len(opts.BlockIds) == 0 {
		return true, nil
	}

	br := opts.BlockIds[0]
	if strings.Contains(br.Orig, "-") && !strings.Contains(br.Orig, ":") {
		return br.Start.Number <= uint(bn) && uint(bn) <= br.End.Number, nil
	}

	chain := opts.Globals.Chain
	if blockNums, err := br.ResolveBlocks(chain); err != nil {
		return false, err
	} else {
		for _, num := range blockNums {
			if num == bn {
				return true, nil
			}
		}
		return false, nil
	}
}
