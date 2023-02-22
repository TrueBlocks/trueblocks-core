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
	Disp         string `json:"disp,omitempty"`
	Is_omitempty string `json:"is_omitempty"`
	Is_nowrite   string `json:"is_nowrite,omitempty"`
	Value        string `json:"value,omitempty"`
	Is_noaddfld  string `json:"is_noaddfld,omitempty"`
	Is_minimal   string `json:"is_minimal,omitempty"`
	Is_builtin   string `json:"is_builtin,omitempty"`
	Unused       string `json:"unused,omitempty"`
	Is_flags     string `json:"is_flags,omitempty"`
	Precision    string `json:"precision,omitempty"`
	MaxWidth     string `json:"maxWidth,omitempty"`
	Doc          string `json:"doc,omitempty"`
	Is_extra     string `json:"is_extra,omitempty"`
	Example      string `json:"example,omitempty"`
	Description  string `json:"description,omitempty"`
	Is_pointer   string `json:"is_pointer,omitempty"`
	Is_array     string `json:"is_array,omitempty"`
	Is_object    string `json:"is_object,omitempty"`
	Type         string `json:"type"`
	Name         string `json:"name"`
	StrDefault   string `json:"strDefault,omitempty"`
	Indexed      string `json:"indexed"`
	InternalType string `json:"internalType"`
	Components   string `json:"components"`
}

type SimpleParameter struct {
	Disp         uint64            `json:"disp,omitempty"`
	Is_omitempty bool              `json:"is_omitempty,omitempty"`
	Is_nowrite   bool              `json:"is_nowrite,omitempty"`
	Value        string            `json:"value"`
	Is_noaddfld  bool              `json:"is_noaddfld,omitempty"`
	Is_minimal   bool              `json:"is_minimal,omitempty"`
	Is_builtin   bool              `json:"is_builtin,omitempty"`
	Unused       bool              `json:"unused,omitempty"`
	Is_flags     uint64            `json:"is_flags,omitempty"`
	Precision    uint64            `json:"precision,omitempty"`
	MaxWidth     uint64            `json:"maxWidth,omitempty"`
	Doc          uint64            `json:"doc,omitempty"`
	Is_extra     bool              `json:"is_extra,omitempty"`
	Example      string            `json:"example,omitempty"`
	Description  string            `json:"description,omitempty"`
	Is_pointer   bool              `json:"is_pointer,omitempty"`
	Is_array     bool              `json:"is_array,omitempty"`
	Is_object    bool              `json:"is_object,omitempty"`
	Type         string            `json:"type"`
	Name         string            `json:"name"`
	StrDefault   string            `json:"strDefault,omitempty"`
	Indexed      bool              `json:"indexed"`
	InternalType string            `json:"internalType"`
	Components   []SimpleParameter `json:"components"`
	raw          *RawParameter
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
		"is_omitempty": s.Is_omitempty,
		"is_nowrite":   s.Is_nowrite,
		"value":        s.Value,
		"is_noaddfld":  s.Is_noaddfld,
		"is_minimal":   s.Is_minimal,
		"is_builtin":   s.Is_builtin,
		"is_flags":     s.Is_flags,
		"precision":    s.Precision,
		"maxWidth":     s.MaxWidth,
		"is_extra":     s.Is_extra,
		"is_pointer":   s.Is_pointer,
		"is_array":     s.Is_array,
		"is_object":    s.Is_object,
		"type":         s.Type,
		"name":         s.Name,
		"strDefault":   s.StrDefault,
		"internalType": s.InternalType,
	}

	order := []string{
		"is_omitempty",
		"is_nowrite",
		"value",
		"is_noaddfld",
		"is_minimal",
		"is_builtin",
		"is_flags",
		"precision",
		"maxWidth",
		"is_extra",
		"is_pointer",
		"is_array",
		"is_object",
		"type",
		"name",
		"strDefault",
		"internalType",
	}

	// EXISTING_CODE
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
