// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
)

// HandleTimestampUpdate update the timestamp file to the latest block
func (opts *WhenOptions) HandleTimestampUpdate() error {
	if opts.Globals.TestMode {
		logger.Log(logger.Warning, "Update option not tested.")
		return nil
	}

	cnt, err := tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	meta, err := rpcClient.GetMetaData(opts.Globals.Chain, false)
	if err != nil {
		return err
	}

	if cnt >= meta.Latest {
		logger.Log(logger.Info, "Timestamp file is up to date.")
		return nil
	}

	timestamps := make([]tslib.Timestamp, 0, meta.Latest-cnt+2)

	logger.Log(logger.Info, "Updating timestamps file from", cnt, "to", meta.Latest, fmt.Sprintf("(%d blocks)", (meta.Latest-cnt)))
	for bn := cnt; bn < meta.Latest; bn++ {
		block, _ := rpcClient.GetBlockByNumber(opts.Globals.Chain, bn)
		record := tslib.Timestamp{Bn: uint32(block.BlockNumber), Ts: uint32(block.TimeStamp)}
		timestamps = append(timestamps, record)
		logger.Log(logger.Progress, "Adding block ", bn, " to timestamp array")
		if bn%1000 == 0 {
			logger.Log(logger.Info, "Writing...", len(timestamps), "timestamps at block", bn)
			tslib.Append(opts.Globals.Chain, timestamps)
			timestamps = []tslib.Timestamp{}
		}
	}

	if len(timestamps) > 0 {
		logger.Log(logger.Info, "Writing...", len(timestamps), "timestamps at block", meta.Latest)
		tslib.Append(opts.Globals.Chain, timestamps)
	}

	return nil
}
