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

type Approval struct {
	Allowance    base.Wei       `json:"allowance"`
	BlockNumber  base.Blknum    `json:"blockNumber"`
	LastAppBlock base.Blknum    `json:"lastAppBlock"`
	LastAppLogID base.Lognum    `json:"lastAppLogID"`
	LastAppTs    base.Timestamp `json:"lastAppTs"`
	LastAppTxID  base.Txnum     `json:"lastAppTxID"`
	Owner        base.Address   `json:"owner"`
	Spender      base.Address   `json:"spender"`
	Timestamp    base.Timestamp `json:"timestamp"`
	Token        base.Address   `json:"token"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Approval) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Approval) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Owner, "owner"),
		NewLabeler(s.Spender, "spender"),
		NewLabeler(s.Token, "token"),
	}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"blockNumber",
		"timestamp",
		"date",
		"owner",
		"spender",
		"token",
		"allowance",
		"lastAppBlock",
		"lastAppLogID",
		"lastAppTxID",
		"lastAppTs",
		"lastAppDate",
	}

	if verbose {
		for _, item := range rawNames {
			key := item.name + "Name"
			if _, exists := model[key]; exists {
				order = append(order, key)
			}
		}
		order = reorderFields(order)
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Approval.
// This excludes any calculated or derived fields.
func (s *Approval) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"blockNumber":  s.BlockNumber,
		"timestamp":    s.Timestamp,
		"owner":        s.Owner,
		"spender":      s.Spender,
		"token":        s.Token,
		"allowance":    s.Allowance.String(),
		"lastAppBlock": s.LastAppBlock,
		"lastAppLogID": s.LastAppLogID,
		"lastAppTxID":  s.LastAppTxID,
		"lastAppTs":    s.LastAppTs,
	}

	// Handle address labeling only if verbose
	if p.Verbose {
		return labelAddresses(p, model, needed)
	}
	return model
}

// CalcMap returns a map containing the calculated/derived fields for this Approval.
// This includes formatted dates and other computed values.
func (s *Approval) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		"date":        s.Date(),
		"lastAppDate": base.FormattedDate(s.LastAppTs),
	}

	return model
}

func (s *Approval) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Approval) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
