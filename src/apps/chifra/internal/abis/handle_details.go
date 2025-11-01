package abisPkg

import (
	"errors"
	"fmt"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

func (opts *AbisOptions) HandleDetails(rCtx *output.RenderCtx) (err error) {
	if len(opts.Addrs) == 0 {
		vFunc := func(fn string, vP any) (bool, error) {
			_ = vP
			_, name := filepath.Split(fn)
			if strings.HasSuffix(name, ".json") {
				if base.IsValidAddress(strings.TrimSuffix(name, ".json")) {
					// silent ignore
					opts.Addrs = append(opts.Addrs, strings.TrimSuffix(name, ".json"))
				}
			}
			return true, nil
		}
		opts.ForEveryAbi(true, vFunc, nil)
	}

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
				modelChan <- f
			}
		}
	}

	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}
