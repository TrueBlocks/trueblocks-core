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
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
)

// EXISTING_CODE

type Parameter struct {
	Components    []Parameter     `json:"components,omitempty"`
	Indexed       bool            `json:"indexed,omitempty"`
	InternalType  string          `json:"internalType,omitempty"`
	Name          string          `json:"name"`
	StrDefault    string          `json:"strDefault,omitempty"`
	ParameterType string          `json:"type"`
	Value         any             `json:"value,omitempty"`
	Calcs         *ParameterCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Parameter) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Parameter) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"type",
		"name",
	}

	if format == "json" {
		if s.Indexed {
			order = append(order, "indexed")
		}
		if s.ParameterType != s.InternalType {
			order = append(order, "internalType")
		}
		if len(s.Components) > 0 {
			order = append(order, "components")
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

// RawMap returns a map containing only the raw/base fields for this Parameter.
func (s *Parameter) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"name": s.Name,
		"type": s.ParameterType,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Parameter) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format == "json" {
		if s.Indexed {
			model["indexed"] = s.Indexed
		}
		if s.ParameterType != s.InternalType {
			model["internalType"] = s.InternalType
		}
		if len(s.Components) > 0 {
			model["components"] = s.Components
		}
	}
	// EXISTING_CODE

	return model
}

func (s *Parameter) MarshalCache(writer io.Writer) (err error) {
	// Components
	components := make([]base.Marshaler, 0, len(s.Components))
	for _, component := range s.Components {
		components = append(components, &component)
	}
	if err = base.WriteValue(writer, components); err != nil {
		return err
	}

	// Indexed
	if err = base.WriteValue(writer, s.Indexed); err != nil {
		return err
	}

	// InternalType
	if err = base.WriteValue(writer, s.InternalType); err != nil {
		return err
	}

	// Name
	if err = base.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// StrDefault
	if err = base.WriteValue(writer, s.StrDefault); err != nil {
		return err
	}

	// ParameterType
	if err = base.WriteValue(writer, s.ParameterType); err != nil {
		return err
	}

	// Value
	value, err := json.Marshal(s.Value)
	if err != nil {
		return fmt.Errorf("cannot marshal Value: %w", err)
	}
	if err = base.WriteValue(writer, value); err != nil {
		return err
	}

	return nil
}

func (s *Parameter) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Components
	s.Components = make([]Parameter, 0)
	if err = base.ReadValue(reader, &s.Components, fileVersion); err != nil {
		return err
	}

	// Indexed
	if err = base.ReadValue(reader, &s.Indexed, fileVersion); err != nil {
		return err
	}

	// InternalType
	if err = base.ReadValue(reader, &s.InternalType, fileVersion); err != nil {
		return err
	}

	// Name
	if err = base.ReadValue(reader, &s.Name, fileVersion); err != nil {
		return err
	}

	// StrDefault
	if err = base.ReadValue(reader, &s.StrDefault, fileVersion); err != nil {
		return err
	}

	// ParameterType
	if err = base.ReadValue(reader, &s.ParameterType, fileVersion); err != nil {
		return err
	}

	// Value
	var value string
	if err = base.ReadValue(reader, &value, fileVersion); err != nil {
		return err
	}
	if err = json.Unmarshal([]byte(value), &s.Value); err != nil {
		return fmt.Errorf("cannot unmarshal Value: %w", err)
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Parameter) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// ParameterCalcs holds lazy-loaded calculated fields for Parameter
type ParameterCalcs struct {
	// EXISTING_CODE
	Indexed      bool   `json:"indexed,omitempty"`
	InternalType string `json:"internalType,omitempty"`
	// EXISTING_CODE
}

func (s *Parameter) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &ParameterCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
//

// DisplayName returns parameter name if defined, or a default name "val_" + index
func (s *Parameter) DisplayName(index int) string {
	if s.Name != "" {
		return s.Name
	}
	return "val_" + fmt.Sprint(index)
}

func parametersToMap(params []Parameter) (result map[string]any) {
	// This produces `null` in JSON instead of an empty object (`{}`)
	if len(params) == 0 {
		return nil
	}
	result = make(map[string]any)
	for index, param := range params {
		if param.Value == "0x" || param.Value == "0x0" {
			continue
		}
		result[param.DisplayName(index)] = param.Value
	}
	return
}

// EXISTING_CODE
