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
	Calcs        *ApprovalCalcs `json:"calcs,omitempty"`
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
	model := s.RawMap(props, &rawNames)
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

// RawMap returns a map containing only the raw/base fields for this Approval.
func (s *Approval) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
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
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		return labelAddresses(p, model, needed)
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Approval) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		"date":        s.Date(),
		"lastAppDate": base.FormattedDate(s.LastAppTs),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return model
}

func (s *Approval) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Approval) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// ApprovalCalcs holds lazy-loaded calculated fields for Approval
type ApprovalCalcs struct {
	// EXISTING_CODE
	Date        string `json:"date"`
	LastAppDate string `json:"lastAppDate"`
	// EXISTING_CODE
}

func (s *Approval) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &ApprovalCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// EXISTING_CODE
