package slurpPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/provider"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *SlurpOptions) HandleAppearances() error {
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
	fetchData := func(modelChan chan types.Modeler[types.RawAppearance], errorChan chan error) {
		appearancesChan := esProvider.Appearances(ctx, query, errorChan)
		for appearance := range appearancesChan {
			modelChan <- &appearance
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}

func (opts *SlurpOptions) FirstPage() int {
	switch opts.Source {
	case "etherscan":
		return 1
	case "key":
		fallthrough
	default:
		return 0
	}
}
