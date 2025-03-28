package namesPkg

import (
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleTags(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	namesArray, err := loadNamesArray(chain, opts.getType(), types.SortByTags, opts.Terms)
	if err != nil {
		return err
	}
	if len(namesArray) == 0 {
		logger.Warn("No results for", os.Args)
		return nil
	}

	tagsMap := make(map[string]bool, len(namesArray)/10)

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		for _, name := range namesArray {
			if len(name.Tags) > 0 && !tagsMap[name.Tags] {
				s := types.Name{
					Tags: name.Tags,
				}
				modelChan <- &s
			}
			tagsMap[name.Tags] = true
		}
	}

	extraOpts := map[string]any{
		"single": "tags",
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
