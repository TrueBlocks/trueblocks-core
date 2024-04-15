package slurpPkg

import (
	"context"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/provider"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleShow() error {
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Articulate)

	// TODO: maybe use []string in Query.Addresses
	addresses := make([]base.Address, 0, len(opts.Addrs))
	for _, addr := range opts.Addrs {
		addresses = append(addresses, base.HexToAddress(addr))
	}

	esProvider := provider.NewEtherscanProvider(opts.Conn)
	esProvider.PrintProgress = !opts.Globals.TestMode && !utils.IsTerminal()
	query := &provider.Query{
		Addresses: addresses,
		Resources: opts.Types,
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawSlurp], errorChan chan error) {
		txChan := esProvider.TransactionsByAddress(ctx, query, errorChan)
		for tx := range txChan {
			if opts.Articulate {
				if err := abiCache.ArticulateSlurp(tx.Transaction); err != nil {
					errorChan <- err // continue even with an error
				}
			}
			modelChan <- tx.Transaction
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
