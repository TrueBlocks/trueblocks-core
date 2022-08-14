package whenPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleTimestampsCheck handles chifra when --timestamps --check
func (opts *WhenOptions) HandleTimestampsCheck() error {
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
		end = start
		if len(blockNums) > 1 {
			end = blockNums[len(blockNums)-1]
		}
	}

	prev := types.SimpleTimestamp{
		BlockNumber: utils.NOPOS,
		TimeStamp:   utils.NOPOS,
	}

	for bn := start; bn < end; bn++ {
		// The i'th item in the timestamp array on disc
		itemOnDisc, err := tslib.FromBn(opts.Globals.Chain, bn)
		if err != nil {
			return err
		}

		// This just simplifies the code below by removing the need to type cast
		onDisc := types.SimpleTimestamp{
			BlockNumber: uint64(itemOnDisc.Bn),
			TimeStamp:   uint64(itemOnDisc.Ts),
		}

		expected := types.SimpleNamedBlock{BlockNumber: bn, TimeStamp: onDisc.TimeStamp}
		if opts.Deep {
			// If we're going deep, we need to query the node
			expected, _ = rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
		}

		if prev.TimeStamp != utils.NOPOS {
			status := "Okay"

			bnSequential := prev.BlockNumber < onDisc.BlockNumber
			if !bnSequential {
				msg := fmt.Sprintf("Block number %d is not one plus %d.%s", onDisc.BlockNumber, prev.BlockNumber, clear)
				logger.Log(logger.Error, msg)
				status = "Error"
			}

			tsSequential := prev.TimeStamp < onDisc.TimeStamp
			if !tsSequential {
				msg := fmt.Sprintf("At block %d, timestamp %d does not increase over previous %d%s", onDisc.BlockNumber, onDisc.TimeStamp, prev.TimeStamp, clear)
				logger.Log(logger.Error, msg)
				status = "Error"
			}

			deepTsCheck := !opts.Deep || (onDisc.TimeStamp == expected.TimeStamp)
			if !deepTsCheck {
				msg := fmt.Sprint("ts mismatch ts ==> ", bn, "onDisc.Ts ", onDisc.TimeStamp, " expected.TimeStamp ", expected.TimeStamp, "\n")
				logger.Log(logger.Error, msg)
				status = "Error"
			}

			check4 := bn == onDisc.BlockNumber
			if opts.Deep {
				check4 = (bn == onDisc.BlockNumber && onDisc.BlockNumber == expected.BlockNumber)
			}
			if !check4 {
				msg := fmt.Sprint("bn mismatch bn ==> ", bn, "onDisc.Bn ", onDisc.BlockNumber, " expected.BlockNumber ", expected.BlockNumber, "\n")
				logger.Log(logger.Error, msg)
				status = "Error"
			}

			if status == "Okay" {
				scanBar.Report(opts.Globals.Writer, status, fmt.Sprintf(" %d.%d", expected.BlockNumber, expected.TimeStamp))
			}
		}

		prev = onDisc
	}

	return nil
}

var clear = strings.Repeat(" ", 80)
