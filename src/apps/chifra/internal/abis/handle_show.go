package abisPkg

import (
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

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		functions, which, err := opts.LoadAbis(opts.Addrs, false /* known */)
		if err != nil {
			if errors.Is(err, rpc.ErrNotAContract) {
				msg := fmt.Errorf("address %s is not a smart contract", which)
				errorChan <- msg
				// Report but don't quit processing
			} else {
				// Cancel on all other errors
				errorChan <- err
				rCtx.Cancel()
				// } else if len(opts.ProxyFor) > 0 {
				// TODO: We need to copy the proxied-to ABI to the proxy (replacing)
			}
		}

		for _, f := range functions {
			modelChan <- f
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

func (opts *AbisOptions) LoadAbis(addrs []string, loadKnown bool) ([]*types.Function, string, error) {
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Known)
	for _, addr := range addrs {
		address := base.HexToAddress(addr)
		proxy := base.HexToAddress(opts.ProxyFor)
		if !proxy.IsZero() {
			address = proxy
		}
		err := abi.LoadAbi(opts.Conn, address, &abiCache.AbiMap)
		if err != nil {
			return []*types.Function{}, address.Hex(), err
		}
	}

	names := abiCache.AbiMap.Keys()
	sort.Strings(names)

	var funcs = make([]*types.Function, 0, len(names))
	for _, name := range names {
		f := abiCache.AbiMap.GetValue(name)
		if f != nil {
			funcs = append(funcs, f)
		}
	}
	return funcs, "", nil
}
