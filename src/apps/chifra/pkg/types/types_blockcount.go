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

type BlockCount struct {
	AddressCnt      uint64           `json:"addressCnt,omitempty"`
	BlockNumber     base.Blknum      `json:"blockNumber"`
	LogsCnt         uint64           `json:"logsCnt,omitempty"`
	Timestamp       base.Timestamp   `json:"timestamp"`
	TracesCnt       uint64           `json:"tracesCnt,omitempty"`
	TransactionsCnt uint64           `json:"transactionsCnt"`
	UnclesCnt       uint64           `json:"unclesCnt,omitempty"`
	WithdrawalsCnt  uint64           `json:"withdrawalsCnt,omitempty"`
	Calcs           *BlockCountCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s BlockCount) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *BlockCount) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"blockNumber",
		"transactionsCnt",
	}

	if verbose {
		order = append(order, "timestamp", "date")
	}

	wantsUncles := extraOpts["uncles"] == true
	wantsLogs := extraOpts["logs"] == true
	wantsTraces := extraOpts["traces"] == true
	wantsUniqs := extraOpts["uniqs"] == true
	wantsWithdrawals := true

	if format != "json" {
		if wantsUncles {
			order = append(order, "unclesCnt")
		}
		if wantsLogs {
			order = append(order, "logsCnt")
		}
		if wantsTraces {
			order = append(order, "tracesCnt")
		}
		if wantsUniqs {
			order = append(order, "addressCnt")
		}
		if wantsWithdrawals {
			order = append(order, "withdrawalsCnt")
		}
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this BlockCount.
func (s *BlockCount) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"blockNumber":     s.BlockNumber,
		"transactionsCnt": s.TransactionsCnt,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		model["timestamp"] = s.Timestamp
	}

	wantsUncles := p.ExtraOpts["uncles"] == true
	wantsLogs := p.ExtraOpts["logs"] == true
	wantsTraces := p.ExtraOpts["traces"] == true
	wantsUniqs := p.ExtraOpts["uniqs"] == true
	wantsWithdrawals := true

	if p.Format == "json" {
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
		}
		if wantsLogs {
			model["logsCnt"] = s.LogsCnt
		}
		if wantsTraces {
			model["tracesCnt"] = s.TracesCnt
		}
		if wantsUniqs {
			model["addressCnt"] = s.AddressCnt
		}
		if wantsWithdrawals {
			model["withdrawalsCnt"] = s.WithdrawalsCnt
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *BlockCount) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		model["date"] = s.Date()
	}
	// EXISTING_CODE

	return model
}

func (s *BlockCount) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *BlockCount) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// BlockCountCalcs holds lazy-loaded calculated fields for BlockCount
type BlockCountCalcs struct {
	// EXISTING_CODE
	Date string `json:"date,omitempty"`
	// EXISTING_CODE
}

func (s *BlockCount) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &BlockCountCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// EXISTING_CODE
