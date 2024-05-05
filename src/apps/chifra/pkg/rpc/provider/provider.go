package provider

import (
	"context"
	"fmt"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// When providers create a new buffered channel, they set this as buffer size.
const providerChannelBufferSize = 50

// SlurpedPageItem carries Transaction (if available) and Appearance (to give more info
// about Transaction, like Address). Either Transaction or Appearance comes from 3rd party
// RPC provider. For example, from Etherscan we get Transaction, but from Key - Appearance.
type SlurpedPageItem struct {
	Appearance  *types.Appearance
	Transaction *types.Slurp
}

// Provider is an abstraction over different RPC services, like Etherscan or Key
type Provider interface {
	// TransactionsByAddress returns a channel that will be populated with transactions
	TransactionsByAddress(context.Context, *Query, chan error) chan types.Slurp

	// Appearances returns a channel that will be populated with appearances
	Appearances(context.Context, *Query, chan error) chan types.Appearance

	// Count returns a channel that will be populated with monitors, one for each address.
	// These monitors will only have NRecords set.
	Count(context.Context, *Query, chan error) chan types.Monitor

	// NewPaginator creates and returns Paginator that should be used for given service
	NewPaginator(startPage any, perPage int) Paginator

	// PrintProgress returns true if Provider should print progress bar
	PrintProgress() bool
	SetPrintProgress(bool)
}

// fetchPageFunc is a function that gets a page of data from 3rd party provider
type fetchPageFunc func(ctx context.Context, address base.Address, paginator Paginator, resource string) ([]SlurpedPageItem, int, error)

// fetchAndFilterData gets all pages available for every query.Address and every query.Resource and push individual page items to a channel.
// It is a helper that encapsulates logic shared by multiple providers
func fetchAndFilterData(ctx context.Context, provider Provider, query *Query, errorChan chan error, fetchPage fetchPageFunc) (resultChan chan SlurpedPageItem) {
	resultChan = make(chan SlurpedPageItem, providerChannelBufferSize)

	totalFetched := 0
	totalFiltered := 0

	go func() {
		defer close(resultChan)

		for _, address := range query.Addresses {
			for _, resource := range query.Resources {
				bar := logger.NewBar(logger.BarOptions{
					Type:    logger.Expanding,
					Enabled: provider.PrintProgress(),
					Prefix:  fmt.Sprintf("%s %s", utils.FormattedHash(false, address.String()), resource),
				})

				// Get a paginator so we can iterate
				paginator := provider.NewPaginator(query.StartPage, int(query.PerPage))

				for !paginator.Done() {
					select {
					case <-ctx.Done():
						return
					default:
						if err := paginator.NextPage(); err != nil {
							errorChan <- err
							return
						}
						page, fetched, err := fetchPage(ctx, address, paginator, resource)
						totalFetched += fetched
						if err != nil {
							errorChan <- err
							continue
						}

						for _, slurpedItem := range page {
							// Make sure we want this block
							if ok, err := query.InRange(base.Blknum(slurpedItem.Appearance.BlockNumber)); !ok {
								if err != nil {
									errorChan <- err
								}
								continue
							}
							totalFiltered++
							bar.Tick()

							resultChan <- slurpedItem
						}
					}
				}
				bar.Finish(true /* newLine */)
			}
			if totalFiltered == 0 {
				msg := fmt.Sprintf("zero transactions reported, %d fetched", totalFetched)
				errorChan <- fmt.Errorf(msg)
			}
		}
	}()

	return
}

// countSlurped turns a channel of SlurpedPageItem into a channel of Monitors.
// It is a helper that encapsulates logic shared by multiple providers
func countSlurped(ctx context.Context, query *Query, slurpedChan chan SlurpedPageItem) (monitorChan chan types.Monitor) {
	monitorChan = make(chan types.Monitor)

	recordCount := make(map[base.Address]types.Monitor, len(query.Addresses))
	var mu sync.Mutex

	go func() {
		defer close(monitorChan)
		for {
			select {
			case <-ctx.Done():
				return
			case slurpedTx, ok := <-slurpedChan:
				if !ok {
					for _, monitor := range recordCount {
						monitorChan <- monitor
					}
					return
				}
				mu.Lock()
				address := slurpedTx.Appearance.Address
				monitor := recordCount[address]
				recordCount[address] = types.Monitor{
					Address:  address,
					NRecords: monitor.NRecords + 1,
				}
				mu.Unlock()
			}
		}
	}()

	return
}
