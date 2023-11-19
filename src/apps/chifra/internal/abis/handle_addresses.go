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

func (opts *AbisOptions) HandleAddresses() (err error) {
	abiCache := articulate.NewAbiCache(opts.Conn, opts.Known)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawFunction], errorChan chan error) {
		// Note here, that known ABIs are not downloaded. They are only loaded from the local cache.
		for _, addr := range opts.Addrs {
			address := base.HexToAddress(addr)
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
