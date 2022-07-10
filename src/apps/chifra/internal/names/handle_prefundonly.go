package namesPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandlePrefundOnly() error {
	err := opts.Globals.RenderHeader(types.SimpleName{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.ApiMode, opts.Globals.NoHeader, true)
	defer opts.Globals.RenderFooter()
	if err != nil {
		return err
	}

	allocs, err := names.LoadPrefunds(opts.Globals.Chain)
	if err != nil {
		return err
	}

	for i, alloc := range allocs {
		a := types.SimpleName{
			Tags:    "80-Prefund",
			Address: strings.ToLower(alloc.Address.Hex()),
			Name:    "Prefund_" + fmt.Sprintf("%04d", i),
		}
		err = opts.Globals.RenderObject(a, i == 0)
		if err != nil {
			return err
		}
	}
	return nil
}
