// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/ranges"
)

// EXISTING_CODE

type TraceFilter struct {
	After       uint64       `json:"after,omitempty"`
	Count       uint64       `json:"count,omitempty"`
	FromAddress base.Address `json:"fromAddress,omitempty"`
	FromBlock   base.Blknum  `json:"fromBlock,omitempty"`
	ToAddress   base.Address `json:"toAddress,omitempty"`
	ToBlock     base.Blknum  `json:"toBlock,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s TraceFilter) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *TraceFilter) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"after":       s.After,
		"count":       s.Count,
		"fromAddress": s.FromAddress,
		"fromBlock":   s.FromBlock,
		"toAddress":   s.ToAddress,
		"toBlock":     s.ToBlock,
	}

	order = []string{
		"fromBlock",
		"toBlock",
		"fromAddress",
		"toAddress",
		"after",
		"count",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *TraceFilter) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *TraceFilter) PassesBasic(trace *Trace, nTested uint64, nPassed uint64) (bool, string) {
	_ = nTested
	_ = nPassed
	if s.FromBlock != 0 && trace.BlockNumber < s.FromBlock {
		reason := fmt.Sprintf("block number (%d) less than fromBlock (%d)", trace.BlockNumber, s.FromBlock)
		return false, reason
	}
	if s.ToBlock != 0 && trace.BlockNumber > s.ToBlock {
		reason := fmt.Sprintf("block number (%d) greater than toBlock (%d)", trace.BlockNumber, s.ToBlock)
		return false, reason
	}
	if !s.FromAddress.IsZero() && trace.Action.From.Hex() != s.FromAddress.Hex() {
		reason := fmt.Sprintf("from address (%s) doesn't match (%s)", trace.Action.From.Hex(), s.FromAddress.Hex())
		return false, reason
	}
	if !s.ToAddress.IsZero() && trace.Action.To.Hex() != s.ToAddress.Hex() {
		reason := fmt.Sprintf("to address (%s) doesn't match (%s)", trace.Action.To.Hex(), s.ToAddress.Hex())
		return false, reason
	}
	return true, ""
}

func (s *TraceFilter) ParseBangString(chain, filter string) (ret map[string]any, br ranges.BlockRange) {
	parts := strings.Split(filter, "!")
	for {
		if len(parts) >= 6 {
			break
		}
		parts = append(parts, "")
	}

	// ret = make(map[string]any)
	s.FromBlock = base.MustParseBlknum(parts[0])
	s.ToBlock = base.MustParseBlknum(parts[1])
	if s.ToBlock < s.FromBlock || s.ToBlock < 1 {
		s.ToBlock = base.NOPOSN
	}
	if base.IsValidAddress(parts[2]) {
		s.FromAddress = base.HexToAddress(parts[2])
	}
	if base.IsValidAddress(parts[3]) {
		s.ToAddress = base.HexToAddress(parts[3])
	}
	s.After = base.MustParseUint64(parts[4])
	s.Count = base.MustParseUint64(parts[5])
	if s.Count == 0 {
		s.Count = base.NOPOS
	}
	return s.Model(chain, "", false, nil).Data, ranges.BlockRange{First: s.FromBlock, Last: s.ToBlock}
}

// EXISTING_CODE
