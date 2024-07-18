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

func (opts *AbisOptions) HandleMany(rCtx *output.RenderCtx) (err error) {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, addr := range opts.Addrs {
			abiCache := articulate.NewAbiCache(opts.Conn, opts.Known)
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
					rCtx.Cancel()
				}
				// } else if len(opts.ProxyFor) > 0 {
				// TODO: We need to copy the proxied-to ABI to the proxy (replacing)
			}

			abi := types.Abi{}
			abi.Address = address
			names := abiCache.AbiMap.Keys()
			sort.Strings(names)
			for _, name := range names {
				function := abiCache.AbiMap.GetValue(name)
				if function != nil {
					abi.Functions = append(abi.Functions, *function)
				}
			}
			modelChan <- &abi
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
