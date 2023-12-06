package slurpPkg

import (
	"context"
	"fmt"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleShow() error {
	testMode := opts.Globals.TestMode
	paginator := rpc.Paginator{
		Page:    1,
		PerPage: int(opts.PerPage),
	}
	if opts.Globals.TestMode {
		paginator.PerPage = 100
	}

	// TODO: Turn this back on
	// TODO: ARTICULATE SLURP
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawSlurp], errorChan chan error) {
		totalFetched := 0
		totalFiltered := 0
		for _, addr := range opts.Addrs {
			for _, tt := range opts.Types {
				paginator.Page = 1
				done := false
				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: !testMode,
					Total:   250, // estimate since we have no idea how many there are
				})
				for !done {
					txs, nFetched, err := opts.Conn.GetESTransactionByAddress(opts.Globals.Chain, addr, tt, &paginator)
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
						// TODO: ARTICULATE SLURP
						if opts.Articulate {
							if err = abiCache.ArticulateSlurp(&tx); err != nil {
								errorChan <- err // continue even with an error
							}
						}
						modelChan <- &tx
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
		}

		if totalFiltered == 0 {
			msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
			errorChan <- fmt.Errorf(msg)
		}
	}

	extra := map[string]interface{}{
		// TODO: ARTICULATE SLURP
		"articulate": opts.Articulate,
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}

func (opts *SlurpOptions) isInRange(bn uint, errorChan chan error) bool {
	chain := opts.Globals.Chain

	// Note that validation ensures that there is only a single range
	if len(opts.BlockIds) == 0 {
		return true
	}

	br := opts.BlockIds[0]
	if strings.Contains(br.Orig, "-") && !strings.Contains(br.Orig, ":") {
		// a plain block range
		return br.Start.Number <= bn && bn <= br.End.Number
	}

	blockNums, err := br.ResolveBlocks(chain)
	if err != nil {
		errorChan <- err
		return false
	}
	for _, num := range blockNums {
		if uint(num) == bn {
			return true
		}
	}

	return false
}
