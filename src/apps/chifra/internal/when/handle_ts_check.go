package whenPkg

import (
	"fmt"
	"os"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/identifiers"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v6/pkg/types"
)

// HandleTimestampsCheck handles chifra when --timestamps --check
func (opts *WhenOptions) HandleTimestampsCheck(rCtx *output.RenderCtx) error {
	// Perform the timestamp checking
	errorCount, totalChecked, err := opts.performTimestampCheck()
	if err != nil {
		return err
	}

	// Create summary message
	var reason string
	isDeep := opts.Deep > 0
	if isDeep {
		reason = "deep on-chain check"
	} else {
		reason = "consistency check"
	}

	// Only return structured output if in API mode
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		_ = errorChan
		result := "passed"
		if errorCount > 0 {
			result = "failed"
		}

		reportCheck := &types.ReportCheck{
			Result:     result,
			Reason:     reason,
			CheckedCnt: uint64(totalChecked),
			PassedCnt:  uint64(totalChecked - errorCount),
			FailedCnt:  uint64(errorCount),
			VisitedCnt: uint64(totalChecked),
		}

		modelChan <- reportCheck
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

func (opts *WhenOptions) performTimestampCheck() (int, int, error) {
	chain := opts.Globals.Chain

	cnt, err := tslib.NTimestamps(chain)
	if err != nil {
		return 0, 0, err
	}

	isDeep := opts.Deep > 0

	// For display only
	skip := uint64(500)
	if isDeep {
		m, _ := opts.Conn.GetMetaData(opts.Globals.TestMode)
		skip = uint64(m.Latest) / 500
	}
	count := uint64(cnt)
	scanBar := progress.NewScanBar(count /* wanted */, (count / skip) /* freq */, count /* max */, (1. / 2.))

	blockNums, err := identifiers.GetBlockNumbers(chain, opts.BlockIds)
	if err != nil {
		return 0, 0, err
	}

	prev := types.NamedBlock{
		BlockNumber: base.NOPOSN,
		Timestamp:   base.NOPOSI,
	}

	errorCount := 0
	totalChecked := 0

	if len(blockNums) > 0 {
		for _, bn := range blockNums {
			if bn < cnt { // ranges may include blocks after last block
				if errs, err := opts.checkOneBlock(scanBar, &prev, bn); err != nil {
					return 0, 0, err
				} else {
					errorCount += errs
					totalChecked++
				}
			}
		}
	} else {
		inc := max(1, base.Blknum(opts.Deep))
		actualCount := uint64(cnt) / uint64(inc)
		if uint64(cnt)%uint64(inc) != 0 {
			actualCount++ // Account for partial last iteration
		}
		scanBar = progress.NewScanBar(actualCount /* wanted */, max(1, actualCount/500) /* freq */, actualCount /* max */, (1. / 2.))

		for bn := base.Blknum(0); bn < cnt; bn = bn + inc {
			if errs, err := opts.checkOneBlock(scanBar, &prev, bn); err != nil {
				return 0, 0, err
			} else {
				errorCount += errs
				totalChecked++
			}
		}
	}

	// Clear the progress bar line when done
	logger.CleanLine()

	return errorCount, totalChecked, nil
}

func (opts *WhenOptions) checkOneBlock(scanBar *progress.ScanBar, prev *types.NamedBlock, bn base.Blknum) (int, error) {
	chain := opts.Globals.Chain
	isDeep := opts.Deep > 0

	// The i'th item in the timestamp array on disc
	itemOnDisc, err := tslib.FromBn(chain, bn)
	if err != nil {
		return 0, err
	}

	// This just simplifies the code below by removing the need to type cast
	onDisc := types.NamedBlock{
		BlockNumber: base.Blknum(itemOnDisc.Bn),
		Timestamp:   base.Timestamp(itemOnDisc.Ts),
	}

	expected := types.LightBlock{BlockNumber: bn, Timestamp: onDisc.Timestamp}
	if isDeep {
		// If we're going deep, we need to query the node
		expected, _ = opts.Conn.GetBlockHeaderByNumber(bn)
	}

	errorCount := 0
	if prev.Timestamp != base.NOPOSI {
		status := "Okay"

		bnSequential := prev.BlockNumber < onDisc.BlockNumber
		if !bnSequential {
			msg := fmt.Sprintf("At block %d, block number %d is not one plus %d.%s", bn, onDisc.BlockNumber, prev.BlockNumber, clear)
			logger.Error(msg)
			status = "Error"
			errorCount++
		}

		tsSequential := prev.Timestamp < onDisc.Timestamp
		if !tsSequential {
			msg := fmt.Sprintf("At block %d, timestamp %d does not increase over previous %d%s", bn, onDisc.Timestamp, prev.Timestamp, clear)
			logger.Error(msg)
			status = "Error"
			errorCount++
		}

		deepTsCheck := !isDeep || (onDisc.Timestamp == expected.Timestamp)
		if !deepTsCheck {
			msg := fmt.Sprintf("At block %d, timestamp on disc %d does not agree with on chain %d%s", bn, onDisc.Timestamp, expected.Timestamp, clear)
			logger.Error(msg)
			status = "Error"
			errorCount++
		}

		posOnDisc := bn == onDisc.BlockNumber
		if isDeep {
			posOnDisc = (bn == onDisc.BlockNumber && onDisc.BlockNumber == expected.BlockNumber)
		}
		if !posOnDisc {
			msg := fmt.Sprintf("At block %d, onDisc block number %d does not match expected %d%s", bn, onDisc.BlockNumber, expected.BlockNumber, clear)
			logger.Error(msg)
			status = "Error"
			errorCount++
		}

		if status == "Okay" {
			scanBar.Report(os.Stderr, status, fmt.Sprintf(" bn: %d ts: %d", expected.BlockNumber, expected.Timestamp))
		}
	}

	*prev = onDisc
	return errorCount, nil
}

// TODO: There's got to be a better way
var clear = strings.Repeat(" ", 60)
