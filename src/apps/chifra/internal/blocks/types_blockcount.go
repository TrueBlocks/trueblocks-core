// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package blocksPkg

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type simpleBlockCount struct {
	AddressCnt      uint64         `json:"addressCnt,omitempty"`
	BlockNumber     base.Blknum    `json:"blockNumber"`
	LogsCnt         uint64         `json:"logsCnt,omitempty"`
	Timestamp       base.Timestamp `json:"timestamp"`
	TracesCnt       uint64         `json:"tracesCnt,omitempty"`
	TransactionsCnt uint64         `json:"transactionsCnt"`
	UnclesCnt       uint64         `json:"unclesCnt,omitempty"`
	WithdrawalsCnt  uint64         `json:"withdrawalsCnt,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleBlockCount) Raw() *types.RawModeler {
	return nil
}

func (s *simpleBlockCount) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
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

	return types.Model{
		Data:  model,
		Order: order,
	}
}

func (s *simpleBlockCount) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// EXISTING_CODE
// EXISTING_CODE
