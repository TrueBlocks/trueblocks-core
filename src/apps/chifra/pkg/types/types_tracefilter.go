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
	After       uint64          `json:"after,omitempty"`
	Count       uint64          `json:"count,omitempty"`
	FromAddress []base.Address  `json:"fromAddress,omitempty"`
	FromBlock   string          `json:"fromBlock,omitempty"`
	ToAddress   []base.Address  `json:"toAddress,omitempty"`
	ToBlock     string          `json:"toBlock,omitempty"`
	raw         *RawTraceFilter `json:"-"`
	// EXISTING_CODE
	Seen uint64 `json:"-"`
	// EXISTING_CODE
}

func (s *SimpleTraceFilter) Raw() *RawTraceFilter {
	return s.raw
}

func (s *SimpleTraceFilter) SetRaw(raw *RawTraceFilter) {
	s.raw = raw
}

func (s *SimpleTraceFilter) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"after":     s.After,
		"count":     s.Count,
		"fromBlock": s.FromBlock,
		"toBlock":   s.ToBlock,
	}

	order = []string{
		"fromBlock",
		"toBlock",
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

func (s *SimpleTraceFilter) PassesAddressFilter(trace *SimpleTrace) bool {
	passesTo := len(s.ToAddress) == 0
	if !passesTo {
		for _, addr := range s.ToAddress {
			if trace.Action.To == addr {
				passesTo = true
				break
			}
		}
	}
	if !passesTo {
		return false
	}

	passesFrom := len(s.FromAddress) == 0
	if !passesFrom {
		for _, addr := range s.FromAddress {
			if trace.Action.From == addr {
				passesFrom = true
				break
			}
		}
	}
	if !passesFrom {
		return false
	}
	return true
}

func (s *SimpleTraceFilter) PassesFilter(trace *SimpleTrace, index int) bool {
	fromBlock := mustParseUint(s.FromBlock)
	if trace.BlockNumber < fromBlock {
		fmt.Println("block number too low", trace.BlockNumber, fromBlock)
		return false
	}

	toBlock := mustParseUint(s.ToBlock)
	if trace.BlockNumber > toBlock {
		fmt.Println("block number too high", trace.BlockNumber, toBlock)
		return false
	}

	if !(s.After == 0 || uint64(index) >= s.After) {
		fmt.Println("index too low", index, s.After)
		return false
	}

	if s.Seen > s.Count {
		fmt.Println("count too high", s.Seen, s.Count)
		return false
	}

	if !s.PassesAddressFilter(trace) {
		return false
	}

	fmt.Println("passed")
	s.Seen++
	return true
}

func (s *SimpleTraceFilter) ParseBangString(filter string) (ret map[string]any, br base.BlockRange) {
	br = base.BlockRange{First: 0, Last: utils.NOPOS}
	ret = make(map[string]any)
	parts := strings.Split(filter, "!")
	if len(parts) > 0 && mustParseUint(parts[0]) > 0 {
		br.First = mustParseUint(parts[0])
		s.FromBlock = fmt.Sprintf("0x%x", br.First)
		ret["fromBlock"] = s.FromBlock
	}
	if len(parts) > 1 && mustParseUint(parts[1]) > 0 {
		br.Last = mustParseUint(parts[1])
		s.ToBlock = fmt.Sprintf("0x%x", br.Last)
		ret["toBlock"] = s.ToBlock
	}
	if len(parts) > 2 && len(parts[2]) > 0 {
		s.FromAddress = append(s.FromAddress, base.HexToAddress(parts[2]))
		ret["fromAddress"] = s.FromAddress
	}
	if len(parts) > 3 && len(parts[3]) > 0 {
		s.ToAddress = append(s.ToAddress, base.HexToAddress(parts[3]))
		ret["toAddress"] = s.ToAddress
	}
	if len(parts) > 4 && mustParseUint(parts[4]) > 0 {
		s.After = mustParseUint(parts[4])
		ret["after"] = s.After
	}
	if len(parts) > 5 && mustParseUint(parts[5]) > 0 {
		s.Count = mustParseUint(parts[5])
		ret["count"] = s.Count
	}
	return ret, br
}

// EXISTING_CODE
