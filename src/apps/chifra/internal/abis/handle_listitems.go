package abisPkg

import (
	"errors"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleListFuncs(rCtx *output.RenderCtx) (err error) {
	return opts.HandleListItems(rCtx, "function")
}

func (opts *AbisOptions) HandleListEvents(rCtx *output.RenderCtx) (err error) {
	return opts.HandleListItems(rCtx, "event")
}

func (opts *AbisOptions) HandleListItems(rCtx *output.RenderCtx, filter string) (err error) {
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		for _, addr := range opts.Addrs {
			functions, which, err := opts.LoadAbis([]string{addr}, false /* known */)
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
				if filter == "" || filter == f.FunctionType {
					modelChan <- f
				}
			}
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
