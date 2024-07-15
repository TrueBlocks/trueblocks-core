package namesPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleShow(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	testMode := opts.Globals.TestMode
	namesArray, err := loadNamesArray(chain, opts.getType(), names.SortByAddress, opts.Terms)
	if err != nil {
		return err
	}

	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		if len(namesArray) == 0 {
			hasAddr := strings.Contains(strings.Join(opts.Terms, " "), "0x")
			hadEths := strings.Contains(strings.Join(opts.OrigTerms, " "), ".eth")
			if hadEths && hasAddr {
				for i, t := range opts.Terms {
					orig := opts.OrigTerms[i]
					if strings.Contains(orig, ".eth") {
						modelChan <- &types.Name{
							Tags:    "66-ENS",
							Name:    orig,
							Address: base.HexToAddress(t),
						}
					} else if strings.Contains(orig, "0x") {
						modelChan <- &types.Name{
							Tags:    "Not found",
							Address: base.HexToAddress(orig),
						}
					}
				}
			} else {
				logger.Warn("No known names found for", opts.Terms)
				if !testMode {
					logger.Warn("Original command:", opts.OrigTerms)
				}
			}
			return
		} else {
			for _, name := range namesArray {
				modelChan <- &name
			}
		}
	}

	extraOpts := map[string]any{
		"expand":  opts.Expand,
		"prefund": opts.Prefund,
	}

	if opts.Addr {
		extraOpts["single"] = "address"
		opts.Globals.NoHeader = true
	}

	return output.StreamMany(rCtx.Ctx, fetchData, opts.Globals.OutputOptsWithExtra(extraOpts))
}
