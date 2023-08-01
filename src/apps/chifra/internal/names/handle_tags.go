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
	chain := opts.Globals.Chain
	namesArray, err := names.LoadNamesArray(chain, opts.getType(), names.SortByTags, opts.Terms)
	if err != nil {
		return err
	}
	if len(namesArray) == 0 {
		logger.Warn("No results for", os.Args)
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

	extra := map[string]interface{}{
		"single": "tags",
	}
	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOptsWithExtra(extra))
}
