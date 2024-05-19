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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawBlockCount struct {
	AddressCnt      string `json:"addressCnt"`
	BlockNumber     string `json:"blockNumber"`
	LogsCnt         string `json:"logsCnt"`
	Timestamp       string `json:"timestamp"`
	TracesCnt       string `json:"tracesCnt"`
	TransactionsCnt string `json:"transactionsCnt"`
	UnclesCnt       string `json:"unclesCnt"`
	WithdrawalsCnt  string `json:"withdrawalsCnt"`
}

type BlockCount struct {
	AddressCnt      uint64         `json:"addressCnt,omitempty"`
	BlockNumber     base.Blknum    `json:"blockNumber"`
	LogsCnt         uint64         `json:"logsCnt,omitempty"`
	Timestamp       base.Timestamp `json:"timestamp"`
	TracesCnt       uint64         `json:"tracesCnt,omitempty"`
	TransactionsCnt uint64         `json:"transactionsCnt"`
	UnclesCnt       uint64         `json:"unclesCnt,omitempty"`
	WithdrawalsCnt  uint64         `json:"withdrawalsCnt,omitempty"`
	raw             *RawBlockCount `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s BlockCount) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *BlockCount) Raw() *RawBlockCount {
	return s.raw
}

func (s *BlockCount) SetRaw(raw *RawBlockCount) {
	s.raw = raw
}

func (s *BlockCount) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockNumber":     s.BlockNumber,
		"transactionsCnt": s.TransactionsCnt,
	}

	order = []string{
		"blockNumber",
		"transactionsCnt",
	}

	if verbose {
		model["timestamp"] = s.Timestamp
		order = append(order, "timestamp")
		model["date"] = s.Date()
		order = append(order, "date")
	}

	wantsUncles := extraOptions["uncles"] == true
	wantsLogs := extraOptions["logs"] == true
	wantsTraces := extraOptions["traces"] == true
	wantsUniqs := extraOptions["uniqs"] == true
	wantsWithdrawals := true

	if format == "json" {
		if wantsUncles && s.UnclesCnt > 0 {
			model["unclesCnt"] = s.UnclesCnt
		}
		if wantsLogs && s.LogsCnt > 0 {
			model["logsCnt"] = s.LogsCnt
		}
		if wantsTraces && s.TracesCnt > 0 {
			model["tracesCnt"] = s.TracesCnt
		}
		if wantsUniqs && s.AddressCnt > 0 {
			model["addressCnt"] = s.AddressCnt
		}
		if wantsWithdrawals && s.WithdrawalsCnt > 0 {
			model["withdrawalsCnt"] = s.WithdrawalsCnt
		}

	} else {
		if wantsUncles {
			model["unclesCnt"] = s.UnclesCnt
			order = append(order, "unclesCnt")
		}
		if wantsLogs {
			model["logsCnt"] = s.LogsCnt
			order = append(order, "logsCnt")
		}
		if wantsTraces {
			model["tracesCnt"] = s.TracesCnt
			order = append(order, "tracesCnt")
		}
		if wantsUniqs {
			model["addressCnt"] = s.AddressCnt
			order = append(order, "addressCnt")
		}
		if wantsWithdrawals {
			model["withdrawalsCnt"] = s.WithdrawalsCnt
			order = append(order, "withdrawalsCnt")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *BlockCount) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *BlockCount) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
