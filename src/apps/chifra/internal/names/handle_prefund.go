package namesPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandlePrefund handles chifra names --prefund
func (opts *NamesOptions) HandlePrefund() error {
	allocs, err := names.LoadPrefunds(opts.Globals.Chain)
	if err != nil {
		return err
	}

	ctx, _ := context.WithCancel(context.Background())
	fetchData := func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
		for i, alloc := range allocs {
			a := types.SimpleName{
				Tags:      "80-Prefund",
				Address:   alloc.Address,
				Name:      "Prefund_" + fmt.Sprintf("%04d", i),
				Source:    "Genesis",
				IsPrefund: true,
				Petname:   alloc.Petname,
			}
			modelChan <- &a
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
