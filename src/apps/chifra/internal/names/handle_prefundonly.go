package namesPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// HandlePrefundOnly handles chifra names --prefund
func (opts *NamesOptions) HandlePrefundOnly() error {
	err := opts.Globals.RenderHeader(types.SimpleName{}, &opts.Globals.Writer, opts.Globals.Format, opts.Globals.NoHeader, true)
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
			Address: hexutil.Encode(alloc.Address.Bytes()),
			Name:    "Prefund_" + fmt.Sprintf("%04d", i),
			Source:  "Genesis",
		}
		err = opts.Globals.RenderObject(a, i == 0)
		if err != nil {
			return err
		}
	}
	return nil
}

// TODO: PrefundOnly is a temporary function used to distinguish chifra names calls with only the --prefund option.
// Once we are fully ported to GoLang, this will go away.
func (opts *NamesOptions) PrefundOnly() bool {
	if !opts.Prefund {
		return false
	}
	if len(opts.Terms) > 0 || opts.Expand || opts.MatchCase || opts.All || opts.Custom || opts.Named ||
		opts.Addr || opts.Tags || opts.ToCustom ||
		opts.Clean || len(opts.Autoname) > 0 || opts.Create || opts.Update ||
		opts.Delete || opts.Undelete || opts.Remove {
		return false
	}
	return true
}
