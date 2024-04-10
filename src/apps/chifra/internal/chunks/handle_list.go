package chunksPkg

import (
	"context"
	"strings"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/pinning"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (opts *ChunksOptions) HandleList(unused []uint64) error {
	testMode := opts.Globals.TestMode
	if testMode {
		logger.Info("Test mode: list pins not tested")
		return nil
	}

	ctx := context.Background()
	fetchData := func(modelChan chan types.Modeler[types.RawIpfsPin], errorChan chan error) {
		var perPage = 1000
		if testMode {
			perPage = -100
		}
		if array, err := pinning.ListPins(opts.Globals.Chain, "pinned", opts.Count, perPage, time.Millisecond*500); err != nil {
			errorChan <- err
		} else {
			for _, line := range array {
				parts := strings.Split(line, "\t")
				if len(parts) == 5 {
					s := types.SimpleIpfsPin{
						Cid:        base.IpfsHash(parts[0]),
						DatePinned: parts[1],
						FileName:   parts[2],
						Size:       utils.MustParseInt(parts[3]),
						Status:     parts[4],
					}
					modelChan <- &s
				}
			}
		}
	}

	return output.StreamMany(ctx, fetchData, opts.Globals.OutputOpts())
}
