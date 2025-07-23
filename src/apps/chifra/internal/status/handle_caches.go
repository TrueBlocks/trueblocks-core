package statusPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
)

func (opts *StatusOptions) HandleCaches(rCtx *output.RenderCtx) error {
	return opts.HandleChainsAndCaches(rCtx, false)
}
