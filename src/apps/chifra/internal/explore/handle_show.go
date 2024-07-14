package explorePkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ExploreOptions) HandleShow(rCtx output.RenderCtx) error {
	_ = context.TODO()
	for _, url := range urls {
		ret := url.getUrl(opts)
		if !opts.Globals.TestMode {
			logger.Info("Opening", ret)
			utils.OpenBrowser(ret)
		} else {
			logger.Info("Not opening", ret, "in test mode")
		}
	}

	return nil
}
