package abisPkg

import (
	"context"
	"os"
	"sort"
    "fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/contract"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *AbisOptions) HandleAddresses() (err error) {
	result := make(abi.AbiInterfaceMap)
	if opts.Known {
		if err = abi.PreloadKnownAbis(opts.Globals.Chain, result, false); err != nil {
			return
		}
	}

	ctx, cancel := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawFunction], errorChan chan error) {
		for _, addr := range opts.Addrs {
			address := types.HexToAddress(addr)
			err := abi.LoadAbiFromAddress(opts.Globals.Chain, address, result)
			// If there is no error the LoadAbiFromAddress was successful so the ABI is in the result
			if err == nil {
				continue
			}

			// There is an error if the abi wasn't already in cache and we need to download it
			// The error should be an NotExist error because the LoadAbiFromAddress tries to read the cache file for the address
			if !os.IsNotExist(err) {
				// The error was not due to a missing file which means it is some other unexpected error
				errorChan <- err
				cancel()
			}

			// Let's try to download the file from somewhere
			if contract, err := contract.IsContractAt(opts.Globals.Chain, address, nil); err != nil {
				errorChan <- err
				cancel()
			} else if !contract {
				logger.Log(logger.Info, "Address", address, "is not a smart contract. Skipping...")
				continue
			}

            // The address is a contract so let's try to download the ABI
			// It's okay to not find the ABI. We report an error, but do not stop processing
			if err = abi.DownloadAbi(opts.Globals.Chain, address, result); err != nil {
				errorChan <- err
			}
		}

		keys := make([]string, 0, len(result))
		for k := range result {
			keys = append(keys, k)
		}
		// We sort by the four-byte and/or event signature
		sort.Strings(keys)

		for _, k := range keys {
			modelChan <- result[k]
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
