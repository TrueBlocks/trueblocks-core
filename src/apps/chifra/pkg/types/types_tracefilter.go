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
	"io"
	"strings"
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
	FromAddress []string        `json:"fromAddress,omitempty"`
	FromBlock   string          `json:"fromBlock,omitempty"`
	ToAddress   []string        `json:"toAddress,omitempty"`
	ToBlock     string          `json:"toBlock,omitempty"`
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

func (s *SimpleTraceFilter) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTraceFilter) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleTraceFilter) ParseBangString(filter string) (ret map[string]any) {
	ret = make(map[string]any)
	parts := strings.Split(filter, "!")
	if len(parts) > 0 && mustParseUint(parts[0]) > 0 {
		s.FromBlock = fmt.Sprintf("0x%x", mustParseUint(parts[0]))
		ret["fromBlock"] = s.FromBlock
	}
	if len(parts) > 1 && mustParseUint(parts[1]) > 0 {
		s.ToBlock = fmt.Sprintf("0x%x", mustParseUint(parts[1]))
		ret["toBlock"] = s.ToBlock
	}
	if len(parts) > 2 && len(parts[2]) > 0 {
		s.FromAddress = append(s.FromAddress, parts[2])
		ret["fromAddress"] = s.FromAddress
	}
	if len(parts) > 3 && len(parts[3]) > 0 {
		s.ToAddress = append(s.ToAddress, parts[3])
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
	return ret
}

// EXISTING_CODE
