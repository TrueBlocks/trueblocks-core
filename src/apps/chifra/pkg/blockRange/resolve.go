// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package blockRange

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	tslibPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/tslib"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

func (br *BlockRange) Resolve(chain string) []uint64 {
	current, end := br.getBounds(chain)
	blocks := []uint64{}
	for current < end {
		blocks = append(blocks, current)
		current = br.nextBlock(current)
	}
	return blocks
}

func (br *BlockRange) getBounds(chain string) (uint64, uint64) {
	start := br.Start.resolvePoint(br.Modifier.Period, chain)
	end := br.End.resolvePoint(br.Modifier.Period, chain)
	if end == utils.NOPOS || end == 0 {
		end = start + 1
	}
	return start, end
}

func (br *BlockRange) nextBlock(current uint64) uint64 {
	if br.ModifierType == BlockRangeStep {
		return current + uint64(br.Modifier.Step)
	} else if br.ModifierType == BlockRangePeriod {
		switch br.Modifier.Period {
		case "hourly":
			return current + (60/14)*60
		case "daily":
			return current + (60/14)*60*24
		case "weekly":
			return current + (60/14)*60*24*7
		case "monthly":
			return current + (60/14)*60*24*31
		case "annually":
			return current + (60/14)*60*24*365
		default:
			return current + 1
		}

	} else {
		return current + 1
	}
}

func (p *Point) resolvePoint(period, chain string) uint64 {
	var bn uint64
	if p.BlockHash != "" {
		provider := config.GetRpcProvider(chain)
		bn, _ = rpcClient.BlockNumberFromHash(provider, p.BlockHash)
	} else if p.Date != "" {
		bn, _ = tslibPkg.BnFromDate(chain, p.Date)
	} else if p.Special != "" {
		bn, _ = tslibPkg.BnFromName(chain, p.Special)
	} else if p.BlockOrTs >= utils.EarliestTs {
		bn, _ = tslibPkg.BnFromTs(chain, uint64(p.BlockOrTs))
	} else {
		bn = uint64(p.BlockOrTs)
	}
	if period == "" {
		return bn
	}
	return snapTo(bn, period, chain)
}

func snapTo(bn uint64, period, chain string) uint64 {
	switch period {
	case "annually":
	case "monthly":
	case "weekly":
	case "daily":
	case "hourly":
	default:
	}
	return bn
}
