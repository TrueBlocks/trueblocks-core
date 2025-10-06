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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type TraceResult struct {
	Address base.Address `json:"address,omitempty"`
	Code    string       `json:"code,omitempty"`
	GasUsed base.Gas     `json:"gasUsed,omitempty"`
	Output  string       `json:"output,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s TraceResult) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *TraceResult) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
	}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	if format == "json" {
		if s.GasUsed > 0 {
			order = append(order, "gasUsed")
		}
		if len(s.Output) > 2 { // "0x" is empty
			order = append(order, "output")
		}
		if !s.Address.IsZero() {
			order = append(order, "address")
		}
		if extraOpts["traces"] != true && len(s.Code) > 2 { // "0x" is empty
			order = append(order, "code")
		}
	} else {
		order = []string{
			"gasUsed",
			"output",
		}
		if !s.Address.IsZero() {
			order = append(order, "address")
		}
	}

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this TraceResult.
// This excludes any calculated or derived fields.
func (s *TraceResult) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{}

	// Apply the same conditional logic as original - only add fields when conditions are met
	if p.Format == "json" {
		// For JSON format, only add fields when they have meaningful values
		if s.GasUsed > 0 {
			model["gasUsed"] = s.GasUsed
		}
		if len(s.Output) > 2 { // "0x" is empty
			model["output"] = s.Output
		}
		if !s.Address.IsZero() {
			model["address"] = s.Address
		}
		if p.ExtraOpts["traces"] != true && len(s.Code) > 2 { // "0x" is empty
			model["code"] = s.Code
		}
	} else {
		model["gasUsed"] = s.GasUsed
		model["output"] = s.Output
		if !s.Address.IsZero() {
			model["address"] = s.Address
		}
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing only the calculated/derived fields for this TraceResult.
// This is optimized for streaming contexts where the frontend receives the raw TraceResult
// and needs to enhance it with calculated values.
func (s *TraceResult) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	// Apply calculated transformations
	if p.Format == "json" {
		// Replace raw code with formatted code when conditions are met
		if p.ExtraOpts["traces"] != true && len(s.Code) > 2 { // "0x" is empty
			model["code"] = utils.FormattedCode(p.Verbose, s.Code)
		}
	} else {
		// Replace raw address with hex encoded version for non-JSON formats
		if !s.Address.IsZero() {
			model["address"] = hexutil.Encode(s.Address.Bytes())
		}
	}

	return model
}

func (s *TraceResult) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Code
	if err = base.WriteValue(writer, s.Code); err != nil {
		return err
	}

	// GasUsed
	if err = base.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}

	// Output
	if err = base.WriteValue(writer, s.Output); err != nil {
		return err
	}

	return nil
}

func (s *TraceResult) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// Code
	if err = base.ReadValue(reader, &s.Code, fileVersion); err != nil {
		return err
	}

	// GasUsed
	if err = base.ReadValue(reader, &s.GasUsed, fileVersion); err != nil {
		return err
	}

	// Output
	if err = base.ReadValue(reader, &s.Output, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *TraceResult) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
