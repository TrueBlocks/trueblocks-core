package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/provider"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleAppearancesKey() error {
	// TODO: maybe use []string in Query.Addresses
	addresses := make([]base.Address, 0, len(opts.Addrs))
	for _, addr := range opts.Addrs {
		addresses = append(addresses, base.HexToAddress(addr))
	}

	keyProvider := provider.NewKeyProvider(opts.Conn, opts.Globals.Chain)
	keyProvider.PrintProgress = !opts.Globals.TestMode && !utils.IsTerminal()
	query := &provider.Query{
		Addresses: addresses,
		Resources: opts.Types,
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		appearancesChan := keyProvider.Appearances(ctx, query, errorChan)
		for appearance := range appearancesChan {
			modelChan <- &appearance
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
