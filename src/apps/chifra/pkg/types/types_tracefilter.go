// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawTraceFilter struct {
	After       string `json:"after"`
	Count       string `json:"count"`
	FromAddress string `json:"fromAddress"`
	FromBlock   string `json:"fromBlock"`
	ToAddress   string `json:"toAddress"`
	ToBlock     string `json:"toBlock"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTraceFilter struct {
	Count       uint64          `json:"count,omitempty"`
	After       uint64          `json:"after,omitempty"`
	FromAddress base.Address    `json:"fromAddress,omitempty"`
	ToAddress   base.Address    `json:"toAddress,omitempty"`
	FromBlock   base.Blknum     `json:"fromBlock,omitempty"`
	ToBlock     base.Blknum     `json:"toBlock,omitempty"`
	raw         *RawTraceFilter `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTraceFilter) Raw() *RawTraceFilter {
	return s.raw
}

func (s *SimpleTraceFilter) SetRaw(raw *RawTraceFilter) {
	s.raw = raw
}

func (s *SimpleTraceFilter) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
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

// EXISTING_CODE
//

func (s *SimpleTraceFilter) PassesBasic(trace *SimpleTrace, nTested uint64, nPassed uint64) (bool, string) {
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

func (s *SimpleTraceFilter) ParseBangString(chain, filter string) (ret map[string]interface{}, br base.BlockRange) {
	parts := strings.Split(filter, "!")
	for {
		if len(parts) >= 6 {
			break
		}
		parts = append(parts, "")
	}

	// ret = make(map[string]any)
	s.FromBlock = mustParseUint(parts[0])
	s.ToBlock = mustParseUint(parts[1])
	if s.ToBlock < s.FromBlock || s.ToBlock < 1 {
		s.ToBlock = utils.NOPOS
	}
	if base.IsValidAddress(parts[2]) {
		s.FromAddress = base.HexToAddress(parts[2])
	}
	if base.IsValidAddress(parts[3]) {
		s.ToAddress = base.HexToAddress(parts[3])
	}
	s.After = mustParseUint(parts[4])
	s.Count = mustParseUint(parts[5])
	if s.Count == 0 {
		s.Count = utils.NOPOS
	}
	return s.Model(chain, "", false, nil).Data, base.BlockRange{First: s.FromBlock, Last: s.ToBlock}
}

// EXISTING_CODE
