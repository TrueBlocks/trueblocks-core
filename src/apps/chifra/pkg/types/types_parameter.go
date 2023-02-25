// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "fmt"

// EXISTING_CODE

type RawParameter struct {
	Components    string `json:"components"`
	Indexed       string `json:"indexed"`
	InternalType  string `json:"internalType"`
	Is_flags      string `json:"is_flags"`
	Name          string `json:"name"`
	Precision     string `json:"precision"`
	StrDefault    string `json:"strDefault"`
	ParameterType string `json:"type"`
	Unused        string `json:"unused"`
	Value         string `json:"value"`
}

type SimpleParameter struct {
	Components    []SimpleParameter `json:"components,omitempty"`
	Indexed       bool              `json:"indexed,omitempty"`
	InternalType  string            `json:"internalType,omitempty"`
	Is_flags      uint64            `json:"is_flags,omitempty"`
	Name          string            `json:"name"`
	Precision     uint64            `json:"precision,omitempty"`
	StrDefault    string            `json:"strDefault,omitempty"`
	ParameterType string            `json:"type"`
	Unused        bool              `json:"unused,omitempty"`
	Value         string            `json:"value,omitempty"`
	raw           *RawParameter
}

func (s *SimpleParameter) Raw() *RawParameter {
	return s.raw
}

func (s *SimpleParameter) SetRaw(raw *RawParameter) {
	s.raw = raw
}

func (s *SimpleParameter) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"name":          s.Name,
		"type":          s.ParameterType,
	}

	order := []string{
		"type",
		"name",
	}

	// EXISTING_CODE
	if format == "json" {
		if s.Indexed {
			model["indexed"] = s.Indexed
			order = append(order, "indexed")
		}
		if s.ParameterType != s.InternalType {
		 	model["internalType"] = s.InternalType
		 	order = append(order, "internalType")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// DisplayName returns parameter name if defined, or a default name "val_" + index
func (s *SimpleParameter) DisplayName(index int) string {
	if s.Name != "" {
		return s.Name
	}
	return "val_" + fmt.Sprint(index)
}

// EXISTING_CODE
