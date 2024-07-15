package abisPkg

import (
	"context"
	"errors"
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleShow(rCtx *output.RenderCtx) (err error) {
	if len(opts.Addrs) > 1 && opts.Globals.Format == "json" {
		return opts.HandleMany(rCtx)
	}

	abiCache := articulate.NewAbiCache(opts.Conn, opts.Known)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		// Note here, that known ABIs are not downloaded. They are only loaded from the local cache.
		for _, addr := range opts.Addrs {
			address := base.HexToAddress(addr)
			proxy := base.HexToAddress(opts.ProxyFor)
			if !proxy.IsZero() {
				address = proxy
			}
			err = abi.LoadAbi(opts.Conn, address, &abiCache.AbiMap)
			if err != nil {
				if errors.Is(err, rpc.ErrNotAContract) {
					msg := fmt.Errorf("address %s is not a smart contract", address.Hex())
					errorChan <- msg
					// Report but don't quit processing
				} else {
					// Cancel on all other errors
					errorChan <- err
					cancel()
				}
				// } else if len(opts.ProxyFor) > 0 {
				// TODO: We need to copy the proxied-to ABI to the proxy (replacing)
			}
		}

		names := abiCache.AbiMap.Keys()
		sort.Strings(names)

		for _, name := range names {
			function := abiCache.AbiMap.GetValue(name)
			modelChan <- function
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
