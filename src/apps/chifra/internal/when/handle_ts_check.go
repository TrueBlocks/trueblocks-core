package whenPkg

import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// HandleTimestampsCheck handles chifra when --timestamps --check
func (opts *WhenOptions) HandleTimestampsCheck() error {
	chain := opts.Globals.Chain

	cnt, err := tslib.NTimestamps(chain)
	if err != nil {
		return err
	}

	// For display only
	skip := uint64(500)
	if opts.Deep {
		m, _ := opts.Conn.GetMetaData(opts.Globals.TestMode)
		skip = m.Latest / 500
	}
	scanBar := progress.NewScanBar(cnt /* wanted */, (cnt / skip) /* freq */, cnt /* max */, (2. / 3.))

	blockNums, err := identifiers.GetBlockNumbers(chain, opts.BlockIds)
	if err != nil {
		return err
	}

	prev := types.SimpleNamedBlock{
		BlockNumber: utils.NOPOS,
		Timestamp:   utils.NOPOSI,
	}

	if len(blockNums) > 0 {
		for _, bn := range blockNums {
			if bn < cnt { // ranges may include blocks after last block
				if err = opts.checkOneBlock(scanBar, &prev, bn); err != nil {
					return err
				}
			}
		}
	} else {
		for bn := uint64(0); bn < cnt; bn++ {
			if err = opts.checkOneBlock(scanBar, &prev, bn); err != nil {
				return err
			}
		}
	}

	return nil
}

func (opts *WhenOptions) checkOneBlock(scanBar *progress.ScanBar, prev *types.SimpleNamedBlock, bn uint64) error {
	chain := opts.Globals.Chain

	// The i'th item in the timestamp array on disc
	itemOnDisc, err := tslib.FromBn(chain, bn)
	if err != nil {
		return err
	}

	// This just simplifies the code below by removing the need to type cast
	onDisc := types.SimpleNamedBlock{
		BlockNumber: uint64(itemOnDisc.Bn),
		Timestamp:   base.Timestamp(itemOnDisc.Ts),
	}

	expected := types.SimpleBlock[string]{BlockNumber: bn, Timestamp: onDisc.Timestamp}
	if opts.Deep {
		// If we're going deep, we need to query the node
		expected, _ = opts.Conn.GetBlockHeaderByNumber(bn)
	}

	if prev.Timestamp != utils.NOPOSI {
		status := "Okay"

		bnSequential := prev.BlockNumber < onDisc.BlockNumber
		if !bnSequential {
			msg := fmt.Sprintf("At block %d, block number %d is not one plus %d.%s", bn, onDisc.BlockNumber, prev.BlockNumber, clear)
			logger.Error(msg)
			status = "Error"
		}

		tsSequential := prev.Timestamp < onDisc.Timestamp
		if !tsSequential {
			msg := fmt.Sprintf("At block %d, timestamp %d does not increase over previous %d%s", bn, onDisc.Timestamp, prev.Timestamp, clear)
			logger.Error(msg)
			status = "Error"
		}

		deepTsCheck := !opts.Deep || (onDisc.Timestamp == expected.Timestamp)
		if !deepTsCheck {
			msg := fmt.Sprintf("At block %d, timestamp on disc %d does not agree with on chain %d%s", bn, onDisc.Timestamp, expected.Timestamp, clear)
			logger.Error(msg)
			status = "Error"
		}

		posOnDisc := bn == onDisc.BlockNumber
		if opts.Deep {
			posOnDisc = (bn == onDisc.BlockNumber && onDisc.BlockNumber == expected.BlockNumber)
		}
		if !posOnDisc {
			msg := fmt.Sprintf("At block %d, onDisc block number %d does not match expected %d%s", bn, onDisc.BlockNumber, expected.BlockNumber, clear)
			logger.Error(msg)
			status = "Error"
		}

		if status == "Okay" {
			scanBar.Report(opts.Globals.Writer, status, fmt.Sprintf(" bn: %d ts: %d", expected.BlockNumber, expected.Timestamp))
		}
	}

	*prev = onDisc
	return nil
}

// TODO: There's got to be a better way
var clear = strings.Repeat(" ", 60)
