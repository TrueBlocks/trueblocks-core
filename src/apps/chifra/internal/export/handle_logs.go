package exportPkg

import (
	"context"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/monitor"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ExportOptions) HandleLogs(monitorArray []monitor.Monitor) error {
	// ctx, cancel := context.WithCancel(context.Background())
	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawLog], errorChan chan error) {
		for _, mon := range monitorArray {
			l := &types.SimpleLog{
				Address:          mon.Address,
				BlockHash:        base.HexToHash("0x123456789abcdef"),
				BlockNumber:      123456789,
				Data:             "0xThis",
				LogIndex:         1,
				TransactionHash:  base.HexToHash("0x123456789abcdef"),
				TransactionIndex: 12,
			}
			modelChan <- l
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
