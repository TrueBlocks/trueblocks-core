// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package scrapePkg

import (
	"context"
	"encoding/binary"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/sigintTrap"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *ScrapeOptions) HandleTouch(rCtx *output.RenderCtx) error {
	chain := opts.Globals.Chain
	fetchData := func(modelChan chan types.Modeler, errorChan chan error) {
		meta, _ := opts.Conn.GetMetaData(false)
		rng := base.FileRange{
			First: meta.Finalized,
			Last:  opts.Touch,
		}
		indexPath := index.ToIndexPath(rng.RangeToFilename(chain))
		stagePath := index.ToStagingPath(rng.RangeToFilename(chain))
		var chunk index.Chunk
		var bm = BlazeManager{}
		_ = file.StringToAsciiFile(stagePath, base.SkippedSender.Hex()+"\t0\t0\n")
		appMap, _, nAppearances := bm.AsciiFileToAppearanceMap(stagePath)
		if report, err := chunk.Write(chain, base.ZeroAddr, indexPath, appMap, nAppearances); err != nil {
			errorChan <- err
		} else {
			modelChan <- &types.Message{Msg: report.Report(false /* isSnapped */, file.FileSize(stagePath))}
		}
		if reportStr, err := opts.backfillTimestamps(); err != nil {
			errorChan <- err
		} else {
			modelChan <- &types.Message{Msg: reportStr}
		}
	}
	return output.StreamMany(rCtx, fetchData, opts.Globals.OutputOpts())
}

func (opts *ScrapeOptions) backfillTimestamps() (string, error) {
	chain := opts.Globals.Chain
	tsPath := config.PathToTimestamps(chain)
	logger.Info(tsPath)

	sigintCtx, cancel := context.WithCancel(context.Background())
	cleanOnQuit := func() {
		// We only print a warning here, as the scrape.pid file will be
		// removed by the deferred function
		logger.Warn(sigintTrap.TrapMessage)
	}
	trapChannel := sigintTrap.Enable(sigintCtx, cancel, cleanOnQuit)
	defer sigintTrap.Disable(trapChannel)

	tmpPath := tsPath + ".backup"
	backup, err := file.MakeBackup(tmpPath, tsPath)
	if err != nil {
		return "", fmt.Errorf("error making backup = %+v", err)
	}
	defer backup.Restore() // put the file back if it still exists (i.e., an error occurred)

	file.Remove(tsPath)
	fp, err := os.OpenFile(tsPath, os.O_WRONLY|os.O_CREATE|os.O_APPEND, 0644)
	if err != nil {
		return "", err
	}
	defer func() {
		tslib.ClearCache(chain)
		fp.Close()
	}()

	if nTimestamps, err := tslib.NTimestamps(chain); err != nil {
		return "", err
	} else {
		logger.Info("nTimestamps:", nTimestamps)
		if nTimestamps > opts.Touch {
			return "", fmt.Errorf("nTimestamps > opts.Touch - should not happen")
		}

		firstTs := uint32(0)
		for block := uint32(0); block <= uint32(opts.Touch); block++ {
			if sigintCtx.Err() != nil {
				// This means the context got cancelled, i.e. we got a SIGINT.
				// The backup will be restored by the deferred function
				break
			}
			if block == 0 {
				firstTs = uint32(opts.Conn.GetBlockTimestamp(base.Value(block)))
			}
			ts := tslib.TimestampRecord{
				Bn: block,
				Ts: uint32(block + firstTs),
			}
			msg := fmt.Sprintf("Backfilling timestamps at block %d", block)
			logger.Progress(true, msg)
			if err := binary.Write(fp, binary.LittleEndian, &ts); err != nil {
				return "", err
			}
		}
		msg := fmt.Sprintf("{%s} were backfilled to block {%d}", "Timestamps", opts.Touch)
		ret := colors.ColoredWith(msg, colors.BrightBlue)
		os.Remove(tmpPath)
		return ret, nil
	}
}
