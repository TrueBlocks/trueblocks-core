// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
// EXISTING_CODE

type RawBlockCount interface{}

type SimpleBlockCount struct {
	BlockNumber uint64 `json:"blockNumber"`
	Timestamp int64 `json:"timestamp"`
	TransactionsCnt uint64 `json:"transactionsCnt"`
	UnclesCnt uint64 `json:"unclesCnt,omitempty"`
	LogsCnt uint64 `json:"logsCnt,omitempty"`
	TracesCnt uint64 `json:"tracesCnt,omitempty"`
	AppearancesCnt uint64 `json:"appearancesCnt,omitempty"`
}

func (s *SimpleBlockCount) Raw() *RawBlockCount {
	return nil
}

func (s *SimpleBlockCount) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	model := map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"timestamp": s.Timestamp,
		"transactionsCnt": s.TransactionsCnt,
	}

	order := []string{
		"blockNumber",
		"timestamp",
		"transactionsCnt",
	}

	// EXISTING_CODE
	if format == "json" {
		wantsUncles := extraOptions["uncles"] == true
		wantsLogs := extraOptions["logs"] == true
		wantsTraces := extraOptions["trace"] == true
		wantsApps := extraOptions["apps"] == true || extraOptions["uniq"] == true

		if s.UnclesCnt == 0 || !wantsUncles {
			delete(model, "unclesCnt")
		} else {
			order = append(order, "unclesCnt")
		}

		if s.LogsCnt == 0 || !wantsLogs {
			delete(model, "logsCnt")
		} else {
			order = append(order, "logsCnt")
		}

		if s.TracesCnt == 0 || !wantsTraces {
			delete(model, "tracesCnt")
		} else {
			order = append(order, "tracesCnt")
		}

		if s.AppearancesCnt == 0 || !wantsApps {
			delete(model, "appearancesCnt")
		} else {
			order = append(order, "appearancesCnt")
		}

	} else {
		order = append(order, "unclesCnt")
		order = append(order, "logsCnt")
		order = append(order, "tracesCnt")
		order = append(order, "appearancesCnt")
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE