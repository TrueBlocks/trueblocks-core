package namesPkg

import (
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/types"
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
			tags := strings.Split(name.Tags, ",")
			for _, tag := range tags {
				if len(tag) > 0 && !tagsMap[tag] {
					s := types.Name{
						Tags: tag,
					}
					modelChan <- &s
				}
				tagsMap[tag] = true
			}
		}
	}

	extraOpts := map[string]any{
		"single": "tags",
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
