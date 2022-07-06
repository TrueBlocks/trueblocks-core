// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package whenPkg

import (
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/colors"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/progress"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// HandleWhenShowTimestamps handles chifra when --timestamps --check
func (opts *WhenOptions) HandleWhenCheckTimestamps() error {
	cnt, err := tslib.NTimestamps(opts.Globals.Chain)
	if err != nil {
		return err
	}

	skip := uint64(500)
	if opts.Deep {
		skip = 10000
	}
	logger.Log(logger.Error, colors.Red, "Not implemented - no real checking is happening.", colors.Off)
	scanBar := progress.NewScanBar(cnt /* wanted */, (cnt / skip) /* freq */, cnt /* max */, (2. / 3.))
	prev := tslib.Timestamp{}
	for bn := 0; bn < int(cnt); bn++ {
		item, err := tslib.FromBn(opts.Globals.Chain, uint64(bn))
		if err != nil {
			return err
		}
		block := types.SimpleNamedBlock{BlockNumber: uint64(bn), TimeStamp: uint64(item.Ts)}
		if opts.Deep {
			block, _ = rpcClient.GetBlockByNumber(opts.Globals.Chain, uint64(bn))
		}
		if bn == 0 {
			block.TimeStamp, _ = rpcClient.GetBlockZeroTs(opts.Globals.Chain)
		}

		check1 := prev.Ts == 0 || prev.Ts < item.Ts
		check2 := prev.Bn == 0 || prev.Bn < item.Bn
		check3 := block.BlockNumber == 0 || (uint32(bn) == item.Bn && uint64(item.Bn) == block.BlockNumber)
		check4 := block.TimeStamp == 0 || (uint64(item.Ts) == block.TimeStamp)
		if !opts.Deep {
			check3 = uint32(bn) == item.Bn
			check4 = true
		}
		if check1 && check2 && check3 && check4 {
			msg := fmt.Sprintf("%d.%d", block.BlockNumber, block.TimeStamp)
			scanBar.Report(opts.Globals.Writer, "Checking", msg)
		} else {
			if !check1 || !check2 {
				fmt.Println("not sequential:", "prev:", prev, "item:", item)
			}
			if !check3 {
				fmt.Println("bn mismatch", "bn:", bn, "item.Bn:", item.Bn, "block.BlockNumber:", block.BlockNumber)
			}
			if !check4 {
				fmt.Println("ts mismatch", "bn:", bn, "item.Ts:", item.Ts, "block.TimeStamp:", block.TimeStamp)
			}
		}
		prev = *item
	}
	return nil
}
