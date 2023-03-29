package namesPkg

import (
	"context"
	"net/rpc"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleTerms() error {
	ctx := context.Background()
	var fetchData func(modelChan chan types.Modeler[types.RawName], errorChan chan error)

	// Try RPC
	client, err := rpc.Dial("unix", "/tmp/trueblocks.sock")
	if err == nil {
		// RPC server is running and available
		fetchData = func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
			var results []types.SimpleName
			args := struct {
				Parts names.Parts
				Terms []string
			}{
				Parts: opts.getType(),
				Terms: opts.Terms,
			}
			if err = client.Call("Rpc.SearchNames", &args, &results); err != nil {
				errorChan <- err
			}
			for _, result := range results {
				result := result
				modelChan <- &result
			}
			// }
		}
	} else {
		// TODO(rpc): we don't have to print here
		logger.Info("Not using RPC server because of error", err)

		namesArray, err := names.LoadNamesArray(opts.Globals.Chain, opts.getType(), names.SortByAddress, opts.Terms)
		if err != nil {
			return err
		}
		if len(namesArray) == 0 {
			logger.Warn("No results for", os.Args)
			return nil
		}

		// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
		fetchData = func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
			for _, name := range namesArray {
				name := name
				modelChan <- &name
			}
		}
	}
	extra := map[string]interface{}{
		"expand":  opts.Expand,
		"prefund": opts.Prefund,
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
		Extra:      extra,
	})
}
