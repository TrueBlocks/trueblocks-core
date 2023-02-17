package namesPkg

import (
	"context"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleTags() error {
	namesArray, err := names.LoadNamesArray(opts.Globals.Chain, opts.getType())
	if err != nil {
		return err
	}
	if opts.Globals.TestMode {
		if opts.Custom {
			namesArray = []names.Name{{Tags: "81-Custom"}}
		}
	}

	sort.Slice(namesArray, func(i, j int) bool {
		return namesArray[i].Tags < namesArray[j].Tags
	})
	tagsMap := make(map[string]bool, len(namesArray)/10)

	ctx := context.Background()

	// Note: Make sure to add an entry to enabledForCmd in src/apps/chifra/pkg/output/helpers.go
	fetchData := func(modelChan chan types.Modeler[types.RawTag], errorChan chan error) {
		for _, name := range namesArray {
			if !tagsMap[name.Tags] {
				s := types.SimpleTag{
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
			"list": true,
		},
	})
}

func (opts *NamesOptions) getType() names.Component {
	if opts.Custom {
		if opts.All {
			return names.Regular | names.Custom
		} else {
			return names.Custom
		}
	}
	return names.Regular
}
