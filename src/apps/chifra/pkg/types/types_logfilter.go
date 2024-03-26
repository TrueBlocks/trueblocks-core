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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawLogFilter struct {
	BlockHash string   `json:"blockHash"`
	Emitters  []string `json:"emitters"`
	FromBlock string   `json:"fromBlock"`
	ToBlock   string   `json:"toBlock"`
	Topics    []string `json:"topics"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleLogFilter struct {
	BlockHash base.Hash      `json:"blockHash"`
	Emitters  []base.Address `json:"emitters"`
	FromBlock base.Blknum    `json:"fromBlock"`
	ToBlock   base.Blknum    `json:"toBlock"`
	Topics    []base.Hash    `json:"topics"`
	raw       *RawLogFilter  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleLogFilter) Raw() *RawLogFilter {
	return s.raw
}

func (s *SimpleLogFilter) SetRaw(raw *RawLogFilter) {
	s.raw = raw
}

func (s *SimpleLogFilter) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleLogFilter) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func NewLogFilter(emitters []string, topics []string) *SimpleLogFilter {
	logFilter := &SimpleLogFilter{}
	for _, e := range emitters {
		logFilter.Emitters = append(logFilter.Emitters, base.HexToAddress(e))
	}
	for _, t := range topics {
		logFilter.Topics = append(logFilter.Topics, base.HexToHash(t))
	}
	return logFilter
}

func (filter *SimpleLogFilter) PassesFilter(log *SimpleLog) bool {
	foundEmitter := false
	for _, e := range filter.Emitters {
		if e == log.Address {
			foundEmitter = true
			break
		}
	}

	topicsFound := 0
	for _, t := range filter.Topics {
		for _, lt := range log.Topics {
			if t == lt {
				topicsFound++
			}
		}
	}

	passesEmitter := len(filter.Emitters) == 0 || foundEmitter
	passesTopic := len(filter.Topics) == 0 || topicsFound > 0

	return passesEmitter && passesTopic
}

// EXISTING_CODE
