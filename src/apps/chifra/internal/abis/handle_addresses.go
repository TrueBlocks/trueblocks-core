package abisPkg

import (
	"context"
	"errors"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleAddresses() (err error) {
	chain := opts.Globals.Chain
	abiCache := articulate.NewAbiCache(chain, opts.Known)

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawFunction], errorChan chan error) {
		// Note here, that known ABIs are not downloaded. They are only loaded from the local cache.
		for _, addr := range opts.Addrs {
			address := base.HexToAddress(addr)
			if err = abi.LoadAbiFromAddress(chain, address, &abiCache.AbiMap); err != nil {
				if !os.IsNotExist(err) {
					// The error was not due to a missing file...
					errorChan <- err
					cancel()
				}
				// Let's try to download the file from somewhere
				if err := opts.Conn.IsContractAt(address, nil); err != nil {
					if !errors.Is(err, rpc.ErrNotAContract) {
						errorChan <- err
						cancel()
					} else {
						logger.Info("Address", address, "is not a smart contract. Skipping...")
						err = nil // not an error to not be a contract
						continue
					}
				} else {
					// It's okay to not find the ABI. We report an error, but do not stop processing
					if err = abi.DownloadAbi(chain, address, &abiCache.AbiMap); err != nil {
						errorChan <- err
					}
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
