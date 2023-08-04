package abisPkg

import (
	"context"
	"errors"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleAddresses() (err error) {
	chain := opts.Globals.Chain

	result := abi.NewFunctionSyncMap12()
	if opts.Known {
		if err = abi.PreloadKnownAbis(chain, result); err != nil {
			return
		}
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawFunction], errorChan chan error) {
		// Note here, that known ABIs are not downloaded. They are only loaded from the local cache.
		for _, addr := range opts.Addrs {
			address := base.HexToAddress(addr)
			if err = abi.LoadAbiFromAddress(chain, address, result); err != nil {
				if !os.IsNotExist(err) {
					// The error was not due to a missing file...
					errorChan <- err
					cancel()
				}
				// Let's try to download the file from somewhere
				if err := opts.Conn.IsContractAt(chain, address, nil); err != nil {
					if !errors.Is(err, rpcClient.ErrNotAContract) {
						errorChan <- err
						cancel()
					} else {
						logger.Info("Address", address, "is not a smart contract. Skipping...")
						err = nil // not an error to not be a contract
						continue
					}
				} else {
					// It's okay to not find the ABI. We report an error, but do not stop processing
					if err = abi.DownloadAbi(chain, address, result); err != nil {
						errorChan <- err
					}
				}
			}
		}

		names := result.Names()
		sort.Strings(names)

		for _, name := range names {
			function := result.Get(name)
			modelChan <- function
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
