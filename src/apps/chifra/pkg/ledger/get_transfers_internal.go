package ledger

import (
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (r *Reconciler) getTransfersInternal(txs []*types.Transaction) ([]*types.Transfer, []*types.Transfer, map[BalanceOptions]*base.Wei, error) {
	type txResult struct {
		ethTransfers   []*types.Transfer
		tokenTransfers []*types.Transfer
		err            error
	}
	txChan := make(chan txResult, len(txs))
	var wg sync.WaitGroup

	for _, tx := range txs {
		wg.Add(1)
		go func(tx *types.Transaction) {
			defer wg.Done()
			ethXfers, tokenXfers, err := r.getUnreconciledTransfers(tx)
			txChan <- txResult{ethXfers, tokenXfers, err}
		}(tx)
	}

	go func() {
		wg.Wait()
		close(txChan)
	}()

	// Collect transfers and check for errors
	var allEthTransfers []*types.Transfer
	var allTokenTransfers []*types.Transfer
	for result := range txChan {
		if result.err != nil {
			return nil, nil, nil, result.err // Return on first error
		}
		allEthTransfers = append(allEthTransfers, result.ethTransfers...)
		allTokenTransfers = append(allTokenTransfers, result.tokenTransfers...)
	}

	// Phase 2: Identify unique combinations from all transfers, including bn-1 if bn > 0
	uniqueKeys := make(map[base.Blknum]map[AssetHolder]struct{})
	for _, xfer := range allEthTransfers {
		bn := xfer.BlockNumber
		key := AssetHolder{Asset: xfer.Asset, Holder: xfer.Holder}
		// Add bn entry
		if _, ok := uniqueKeys[bn]; !ok {
			uniqueKeys[bn] = make(map[AssetHolder]struct{})
		}
		uniqueKeys[bn][key] = struct{}{}
		// Add bn-1 entry if bn > 0
		if bn > 0 {
			prevBn := bn - 1
			if _, ok := uniqueKeys[prevBn]; !ok {
				uniqueKeys[prevBn] = make(map[AssetHolder]struct{})
			}
			uniqueKeys[prevBn][key] = struct{}{}
		}
	}
	for _, xfer := range allTokenTransfers {
		bn := xfer.BlockNumber
		key := AssetHolder{Asset: xfer.Asset, Holder: xfer.Holder}
		// Add bn entry
		if _, ok := uniqueKeys[bn]; !ok {
			uniqueKeys[bn] = make(map[AssetHolder]struct{})
		}
		uniqueKeys[bn][key] = struct{}{}
		// Add bn-1 entry if bn > 0
		if bn > 0 {
			prevBn := bn - 1
			if _, ok := uniqueKeys[prevBn]; !ok {
				uniqueKeys[prevBn] = make(map[AssetHolder]struct{})
			}
			uniqueKeys[prevBn][key] = struct{}{}
		}
	}

	// Phase 3: Concurrent balance queries
	type balanceResult struct {
		key     BalanceOptions
		balance *base.Wei
		err     error
	}
	balanceChan := make(chan balanceResult)
	var balanceWG sync.WaitGroup

	// Launch goroutines for each unique combination
	for bn, keys := range uniqueKeys {
		for key := range keys {
			balanceWG.Add(1)
			go func(bn base.Blknum, key AssetHolder) {
				defer balanceWG.Done()
				balance, err := r.Connection.GetBalanceAtToken(key.Asset, key.Holder, bn)
				ahbKey := BalanceOptions{Asset: key.Asset, Holder: key.Holder, BlockNumber: bn}
				balanceChan <- balanceResult{ahbKey, balance, err}
			}(bn, key)
		}
	}

	// Close the balance channel once all balance queries are done
	go func() {
		balanceWG.Wait()
		close(balanceChan)
	}()

	// Collect balance results
	balances := make(map[BalanceOptions]*base.Wei)
	for result := range balanceChan {
		if result.err != nil {
			return nil, nil, nil, result.err // Return on first error
		}

		key := result.key
		if _, ok := balances[key]; !ok {
			balances[key] = new(base.Wei)
		}
		balances[key] = result.balance
	}

	return allEthTransfers, allTokenTransfers, balances, nil
}
