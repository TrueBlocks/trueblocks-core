package slurpPkg

import (
	"context"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *SlurpOptions) HandleCountKey() error {
	chain := opts.Globals.Chain
	paginator := rpc.Paginator{
		Page:    opts.FirstPage(),
		PerPage: int(opts.PerPage),
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawMonitor], errorChan chan error) {
		for _, addr := range opts.Addrs {
			// TODO: This does not filter by block range because tb_getAppearanceCount does not take filter
			if cnt, err := opts.Conn.SlurpTxCountByAddress(chain, opts.Source, addr, "not-used", &paginator); err != nil {
				errorChan <- err
			} else {
				if cnt == 0 {
					msg := fmt.Sprintf("zero transactions reported, %d fetched", cnt)
					errorChan <- fmt.Errorf(msg)
				} else {
					s := types.SimpleMonitor{
						Address:  base.HexToAddress(addr),
						NRecords: int64(cnt),
					}
					modelChan <- &s
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
