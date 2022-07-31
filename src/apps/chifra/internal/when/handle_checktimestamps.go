package whenPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleWhenTimestampsCheck handles chifra when --timestamps --check
func (opts *WhenOptions) HandleWhenTimestampsCheck() error {
	cnt, err := tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	// For display only
	skip := uint64(500)
	if opts.Deep {
		skip = 10000
	}
	scanBar := progress.NewScanBar(cnt /* wanted */, (cnt / skip) /* freq */, cnt /* max */, (2. / 3.))

	blockNums, err := identifiers.GetBlockNumbers(opts.Globals.Chain, opts.BlockIds)
	if err != nil {
		return err
	}

	start := uint64(0)
	end := uint64(cnt)
	if len(blockNums) > 0 {
		start = blockNums[0]
		if len(blockNums) > 1 {
			end = blockNums[len(blockNums)-1]
		}
	}

	prev := types.SimpleTimestamp{
		BlockNumber: utils.NOPOS,
		TimeStamp:   utils.NOPOS,
	}

	for bn := start; bn < end; bn++ {
		itemOnDisc, err := tslib.FromBn(opts.Globals.Chain, bn)
		if err != nil {
			return err
		}
		// Remove type conversions, eases clarity of following code...
		onDisc := types.SimpleTimestamp{
			BlockNumber: uint64(itemOnDisc.Bn),
			TimeStamp:   uint64(itemOnDisc.Ts),
		}

		block := types.SimpleNamedBlock{BlockNumber: bn, TimeStamp: onDisc.TimeStamp}
		if opts.Deep {
			// If we're going deep, we're going to need to query the node.
			block, _ = rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
		}

		if prev.TimeStamp != utils.NOPOS {
			var check1, check2, check3, check4 bool
			if opts.Deep {
				check1 = prev.TimeStamp < onDisc.TimeStamp
				check3 = prev.BlockNumber < onDisc.BlockNumber

				check2 = onDisc.TimeStamp == block.TimeStamp
				check4 = (bn == onDisc.BlockNumber && onDisc.BlockNumber == block.BlockNumber)
			} else {
				check1 = prev.TimeStamp < onDisc.TimeStamp
				check3 = prev.BlockNumber < onDisc.BlockNumber

				check2 = true
				check4 = bn == onDisc.BlockNumber
			}

			status := "Okay "
			if !check1 || !check3 {
				msg := fmt.Sprint("not sequential:", "prev:", prev, "item:", itemOnDisc)
				logger.Log(logger.Error, msg)
				status = "Error"
			}
			if !check4 {
				msg := fmt.Sprint("bn mismatch bn: ", bn, " item.Bn: ", itemOnDisc.Bn, " block.BlockNumber: ", block.BlockNumber)
				logger.Log(logger.Error, msg)
				status = "Error"
			}
			if !check2 {
				msg := fmt.Sprint("ts mismatch bn: ", bn, " item.Ts: ", itemOnDisc.Ts, " block.TimeStamp: ", block.TimeStamp)
				logger.Log(logger.Error, msg)
				status = "Error"
			}

			if false { // !utils.DebuggingOn {
				scanBar.Report(opts.Globals.Writer, status, fmt.Sprintf("%d.%d", block.BlockNumber, block.TimeStamp))
			}
		}

		prev = onDisc
	}

	return nil
}
