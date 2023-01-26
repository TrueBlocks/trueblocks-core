package abisPkg

import (
	"context"
	"os"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/account"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleAddresses() (err error) {
	testMode := opts.Globals.TestMode
	result := make(abi.AbiInterfaceMap)
	if opts.Known {
		if err = abi.PreloadKnownAbis(opts.Globals.Chain, result, false); err != nil {
			return
		}
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawFunction], errorChan chan error) {
		// result := make(abi.AbiInterfaceMap)
		for _, addr := range opts.Addrs {
			address := types.HexToAddress(addr)
			err = abi.LoadAbiFromAddress(opts.Globals.Chain, address, result)
			if err == nil {
				if testMode {
					// Tests expect sorted map, because maps in C++ are sorted.
					keys := make([]string, 0, len(result))

					for k := range result {
						keys = append(keys, k)
					}
					sort.Strings(keys)

					for _, k := range keys {
						modelChan <- result[k]
					}
					continue
				}
				for _, function := range result {
					modelChan <- function
				}
				continue
			}
			if !os.IsNotExist(err) {
				errorChan <- err
				cancel()
			}

			// We didn't find the file
			// Check if the address is a contract
			contract, err := account.IsContractAt(
				opts.Globals.Chain,
				address,
				nil, // use latest block number
			)
			if err != nil {
				errorChan <- err
				cancel()
			}
			if !contract {
				logger.Log(logger.Info, "Address", address, "is not a smart contract. Skipping...")
				continue
			}
			// Fetch ABI from a provider
			if err = abi.DownloadAbi(opts.Globals.Chain, address, result); err != nil {
				errorChan <- err
			}
			// TODO: This code is duplicated
			if testMode {
				// Tests expect sorted map, because maps in C++ are sorted.
				keys := make([]string, 0, len(result))

				for k := range result {
					keys = append(keys, k)
				}
				sort.Strings(keys)

				for _, k := range keys {
					modelChan <- result[k]
				}
				continue
			}
			for _, function := range result {
				modelChan <- function
			}
			// end of duplicated code
		}
	}

	return output.StreamMany(ctx, fetchData, output.OutputOptions{
		Writer:     opts.Globals.Writer,
		Chain:      opts.Globals.Chain,
		TestMode:   opts.Globals.TestMode,
		NoHeader:   opts.Globals.NoHeader,
		ShowRaw:    opts.Globals.ShowRaw,
		Verbose:    opts.Globals.Verbose,
		LogLevel:   opts.Globals.LogLevel,
		Format:     opts.Globals.Format,
		OutputFn:   opts.Globals.OutputFn,
		Append:     opts.Globals.Append,
		JsonIndent: "  ",
		Extra: map[string]interface{}{
			"verbose": opts.Globals.Verbose,
		},
	})
}
