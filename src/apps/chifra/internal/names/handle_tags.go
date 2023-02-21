package namesPkg

import (
	"context"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleTags() error {
	namesArray, err := names.LoadNamesArray(opts.Globals.Chain, opts.getType(), names.SortByTags, opts.Terms)
	if err != nil {
		return err
	}
	if len(namesArray) == 0 {
		logger.Log(logger.Warning, "No results for ", os.Args)
		return nil
	}

	tagsMap := make(map[string]bool, len(namesArray)/10)
	ctx := context.Background()

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawName], errorChan chan error) {
		for _, name := range namesArray {
			if !tagsMap[name.Tags] {
				s := types.SimpleName{
					Tags: name.Tags,
				}
				modelChan <- &s
			}
			tagsMap[name.Tags] = true
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
			"single": "tags",
		},
	})
}
