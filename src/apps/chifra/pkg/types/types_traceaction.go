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
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/utils"
)

// EXISTING_CODE

type TraceAction struct {
	Address        base.Address      `json:"address,omitempty"`
	Author         base.Address      `json:"author,omitempty"`
	Balance        base.Wei          `json:"balance,omitempty"`
	CallType       string            `json:"callType"`
	From           base.Address      `json:"from"`
	Gas            base.Gas          `json:"gas"`
	Init           string            `json:"init,omitempty"`
	Input          string            `json:"input,omitempty"`
	RefundAddress  base.Address      `json:"refundAddress,omitempty"`
	RewardType     string            `json:"rewardType,omitempty"`
	SelfDestructed base.Address      `json:"selfDestructed,omitempty"`
	To             base.Address      `json:"to"`
	Value          base.Wei          `json:"value"`
	Calcs          *TraceActionCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s TraceAction) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *TraceAction) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
		NewLabeler(s.Author, "author"),
		NewLabeler(s.From, "from"),
		NewLabeler(s.RefundAddress, "refundAddress"),
		NewLabeler(s.SelfDestructed, "selfDestructed"),
		NewLabeler(s.To, "to"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
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

// RawMap returns a map containing only the raw/base fields for this TraceAction.
func (s *TraceAction) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format == "json" {
		if !s.SelfDestructed.IsZero() {
			model["selfDestructed"] = s.SelfDestructed
		}
		if !s.From.IsZero() {
			model["from"] = s.From
		}
		if len(s.CallType) > 0 {
			model["callType"] = s.CallType
		}
		if s.Gas != 0 {
			model["gas"] = s.Gas
		}
		if len(s.Input) > 2 {
			model["input"] = s.Input
		}

		// Always include value as raw string
		model["value"] = s.Value.String()

		if !s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
		} else {
			// Handle the to field logic from original
			if s.To.IsZero() {
				model["to"] = "0x0"
			} else {
				model["to"] = s.To
			}
		}

		// Raw init field (not formatted)
		if len(s.Init) > 0 {
			model["init"] = s.Init
		}

		if !s.Address.IsZero() {
			model["address"] = s.Address
		}
		if !s.Author.IsZero() {
			model["author"] = s.Author
		}
		if len(s.RewardType) > 0 {
			model["rewardType"] = s.RewardType
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *TraceAction) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format == "json" {
		// Apply ether conversions
		asEther := p.ExtraOpts["ether"] == true
		if asEther {
			model["ether"] = s.Value.ToFloatString(18)
			if !s.RefundAddress.IsZero() {
				model["balanceEth"] = s.Balance.ToFloatString(18)
			}
		}

		// Apply code formatting for init field
		if p.ExtraOpts["traces"] != true && len(s.Init) > 0 {
			model["init"] = utils.FormattedCode(p.Verbose, s.Init)
		}
	}
	// EXISTING_CODE

	return model
}

func (s *TraceAction) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Author
	if err = base.WriteValue(writer, s.Author); err != nil {
		return err
	}

	// Balance
	if err = base.WriteValue(writer, &s.Balance); err != nil {
		return err
	}

	// CallType
	if err = base.WriteValue(writer, s.CallType); err != nil {
		return err
	}

	// From
	if err = base.WriteValue(writer, s.From); err != nil {
		return err
	}

	// Gas
	if err = base.WriteValue(writer, s.Gas); err != nil {
		return err
	}

	// Init
	if err = base.WriteValue(writer, s.Init); err != nil {
		return err
	}

	// Input
	if err = base.WriteValue(writer, s.Input); err != nil {
		return err
	}

	// RefundAddress
	if err = base.WriteValue(writer, s.RefundAddress); err != nil {
		return err
	}

	// RewardType
	if err = base.WriteValue(writer, s.RewardType); err != nil {
		return err
	}

	// SelfDestructed
	if err = base.WriteValue(writer, s.SelfDestructed); err != nil {
		return err
	}

	// To
	if err = base.WriteValue(writer, s.To); err != nil {
		return err
	}

	// Value
	if err = base.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *TraceAction) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// Author
	if err = base.ReadValue(reader, &s.Author, fileVersion); err != nil {
		return err
	}

	// Balance
	if err = base.ReadValue(reader, &s.Balance, fileVersion); err != nil {
		return err
	}

	// CallType
	if err = base.ReadValue(reader, &s.CallType, fileVersion); err != nil {
		return err
	}

	// From
	if err = base.ReadValue(reader, &s.From, fileVersion); err != nil {
		return err
	}

	// Gas
	if err = base.ReadValue(reader, &s.Gas, fileVersion); err != nil {
		return err
	}

	// Init
	if err = base.ReadValue(reader, &s.Init, fileVersion); err != nil {
		return err
	}

	// Input
	if err = base.ReadValue(reader, &s.Input, fileVersion); err != nil {
		return err
	}

	// RefundAddress
	if err = base.ReadValue(reader, &s.RefundAddress, fileVersion); err != nil {
		return err
	}

	// RewardType
	if err = base.ReadValue(reader, &s.RewardType, fileVersion); err != nil {
		return err
	}

	// SelfDestructed
	if err = base.ReadValue(reader, &s.SelfDestructed, fileVersion); err != nil {
		return err
	}

	// To
	if err = base.ReadValue(reader, &s.To, fileVersion); err != nil {
		return err
	}

	// Value
	if err = base.ReadValue(reader, &s.Value, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *TraceAction) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// TraceActionCalcs holds lazy-loaded calculated fields for TraceAction
type TraceActionCalcs struct {
	// EXISTING_CODE
	Ether      string `json:"ether,omitempty"`
	BalanceEth string `json:"balanceEth,omitempty"`
	// EXISTING_CODE
}

func (s *TraceAction) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &TraceActionCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// EXISTING_CODE
