// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blockRange

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/bykof/gostradamus"
)

func (br *BlockRange) Resolve(chain string) []uint64 {
	meta, _ := rpcClient.GetMetaData(chain, false)
	latest := meta.Latest
	current, end := br.getBounds(chain)
	blocks := []uint64{}
	for current < end {
		blocks = append(blocks, current)
		current = br.nextBlock(chain, current, latest)
	}
	return blocks
}

func (br *BlockRange) getBounds(chain string) (uint64, uint64) {
	start := br.Start.resolvePoint(chain)
	switch br.ModifierType {
	case BlockRangePeriod:
		start, _ = snapBnToPeriod(start, chain, br.Modifier.Period)
	default:
		// do nothing
	}
	end := br.End.resolvePoint(chain)
	if end == utils.NOPOS || end == 0 {
		end = start + 1
	}
	return start, end
}

func snapBnToPeriod(bn uint64, chain, period string) (uint64, error) {
	dt, err := tslibPkg.FromBnToDate(chain, bn)
	if err != nil {
		return bn, err
	}
	switch period {
	case "hourly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorHour()
	case "daily":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorDay()
	case "weekly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorWeek() // returns Monday -- we want Sunday
		dt = dt.ShiftDays(-1).FloorDay()
	case "monthly":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorMonth()
	case "annually":
		dt = dt.ShiftMinutes(5)
		dt = dt.FloorYear()
	}
	firstDate := gostradamus.FromUnixTimestamp(utils.EarliestTs)
	if dt.Time().Before(firstDate.Time()) {
		dt = firstDate
	}
	ts := uint64(dt.UnixTimestamp())
	return tslibPkg.FromTsToBn(chain, ts)
}

func (br *BlockRange) nextBlock(chain string, current, latest uint64) uint64 {
	bn := current

	if br.ModifierType == BlockRangeStep {
		bn = bn + uint64(br.Modifier.Step)

	} else if br.ModifierType == BlockRangePeriod {
		dt, err := tslibPkg.FromBnToDate(chain, bn)
		if err != nil {
			bn = bn + 1
		} else {
			switch br.Modifier.Period {
			case "hourly":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftHours(1)
				dt = dt.FloorHour()
			case "daily":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftDays(1)
				dt = dt.FloorDay()
			case "weekly":
				dt = dt.ShiftDays(1)
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftWeeks(1)
				dt = dt.FloorWeek() // returns Monday -- we want Sunday
				dt = dt.ShiftDays(-1).FloorDay()
			case "monthly":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftMonths(1)
				dt = dt.FloorMonth()
			case "annually":
				dt = dt.ShiftMinutes(5)
				dt = dt.ShiftYears(1)
				dt = dt.FloorYear()
			default:
				// should not happen
			}

			ts := uint64(dt.UnixTimestamp())
			bn, err = tslibPkg.FromTsToBn(chain, ts)
			if err != nil {
				return utils.NOPOS
			}
		}

	} else {
		bn = bn + 1
	}

	if bn > latest {
		bn = latest
	}

	return bn
}

func (p *Point) resolvePoint(chain string) uint64 {
	var bn uint64
	if p.BlockHash != "" {
		provider := config.GetRpcProvider(chain)
		bn, _ = rpcClient.BlockNumberFromHash(provider, p.BlockHash)
	} else if p.Date != "" {
		bn, _ = tslibPkg.FromDateToBn(chain, p.Date)
	} else if p.Special != "" {
		bn, _ = tslibPkg.FromNameToBn(chain, p.Special)
	} else if p.BlockOrTs >= utils.EarliestTs {
		bn, _ = tslibPkg.FromTsToBn(chain, uint64(p.BlockOrTs))
	} else {
		bn = uint64(p.BlockOrTs)
	}
	return bn
}
