// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/tslib"
)

// HandleTimestampsUpdate update the timestamp file to the latest block
func (opts *WhenOptions) HandleTimestampsUpdate(rCtx *output.RenderCtx) error {
	_ = rCtx
	chain := opts.Globals.Chain

	if opts.Globals.TestMode {
		logger.Warn("Update option not tested.")
		return nil
	}

	cnt, err := tslib.NTimestamps(chain)
	if err != nil {
		return err
	}

	meta, err := opts.Conn.GetMetaData(false)
	if err != nil {
		return err
	}

	if cnt >= meta.Latest {
		logger.Info("Timestamp file is up to date.")
		return nil
	}

	timestamps := make([]tslib.TimestampRecord, 0, meta.Latest-cnt+2)

	logger.Info("Updating timestamps file from", cnt, "to", meta.Latest, fmt.Sprintf("(%d blocks)", (meta.Latest-cnt)))
	for bn := cnt; bn < meta.Latest; bn++ {
		block, _ := opts.Conn.GetBlockHeaderByNumber(bn)
		record := tslib.TimestampRecord{Bn: uint32(block.BlockNumber), Ts: uint32(block.Timestamp)}
		timestamps = append(timestamps, record)
		logger.Progress(bn%23 == 0, "Adding block", bn, "of", meta.Latest, "to timestamp array")
		if bn%1000 == 0 {
			logger.Info("Writing...", len(timestamps), "timestamps at block", bn, "                          ")
			_ = tslib.Append(chain, timestamps)
			timestamps = []tslib.TimestampRecord{}
		}
	}

	if len(timestamps) > 0 {
		logger.Info("Writing...", len(timestamps), "timestamps at block", meta.Latest, "                          ")
		_ = tslib.Append(chain, timestamps)
	}

	return nil
}
