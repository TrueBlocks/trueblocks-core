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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

// EXISTING_CODE

type RawParameter struct {
	Components    string `json:"components"`
	Indexed       string `json:"indexed"`
	InternalType  string `json:"internalType"`
	Name          string `json:"name"`
	StrDefault    string `json:"strDefault"`
	ParameterType string `json:"type"`
	Value         string `json:"value"`
}

type Parameter struct {
	Components    []Parameter   `json:"components,omitempty"`
	Indexed       bool          `json:"indexed,omitempty"`
	InternalType  string        `json:"internalType,omitempty"`
	Name          string        `json:"name"`
	StrDefault    string        `json:"strDefault,omitempty"`
	ParameterType string        `json:"type"`
	Value         any           `json:"value,omitempty"`
	raw           *RawParameter `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Parameter) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Parameter) Raw() *RawParameter {
	return s.raw
}

func (s *Parameter) SetRaw(raw *RawParameter) {
	s.raw = raw
}

func (s *Parameter) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"name": s.Name,
		"type": s.ParameterType,
	}

	order = []string{
		"type",
		"name",
	}

	if format == "json" {
		if s.Indexed {
			model["indexed"] = s.Indexed
			order = append(order, "indexed")
		}
		if s.ParameterType != s.InternalType {
			model["internalType"] = s.InternalType
			order = append(order, "internalType")
		}
		if len(s.Components) > 0 {
			model["components"] = s.Components
			order = append(order, "components")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Parameter) MarshalCache(writer io.Writer) (err error) {
	// Components
	components := make([]cache.Marshaler, 0, len(s.Components))
	for _, component := range s.Components {
		components = append(components, &component)
	}
	if err = cache.WriteValue(writer, components); err != nil {
		return err
	}

	// Indexed
	if err = cache.WriteValue(writer, s.Indexed); err != nil {
		return err
	}

	// InternalType
	if err = cache.WriteValue(writer, s.InternalType); err != nil {
		return err
	}

	// Name
	if err = cache.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// StrDefault
	if err = cache.WriteValue(writer, s.StrDefault); err != nil {
		return err
	}

	// ParameterType
	if err = cache.WriteValue(writer, s.ParameterType); err != nil {
		return err
	}

	// Value
	value, err := json.Marshal(s.Value)
	if err != nil {
		return fmt.Errorf("cannot marshal Value: %w", err)
	}
	if err = cache.WriteValue(writer, value); err != nil {
		return err
	}

	return nil
}

func (s *Parameter) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Components
	s.Components = make([]Parameter, 0)
	if err = cache.ReadValue(reader, &s.Components, vers); err != nil {
		return err
	}

	// Indexed
	if err = cache.ReadValue(reader, &s.Indexed, vers); err != nil {
		return err
	}

	// InternalType
	if err = cache.ReadValue(reader, &s.InternalType, vers); err != nil {
		return err
	}

	// Name
	if err = cache.ReadValue(reader, &s.Name, vers); err != nil {
		return err
	}

	// StrDefault
	if err = cache.ReadValue(reader, &s.StrDefault, vers); err != nil {
		return err
	}

	// ParameterType
	if err = cache.ReadValue(reader, &s.ParameterType, vers); err != nil {
		return err
	}

	// Value
	var value string
	if err = cache.ReadValue(reader, &value, vers); err != nil {
		return err
	}
	if err = json.Unmarshal([]byte(value), &s.Value); err != nil {
		return fmt.Errorf("cannot unmarshal Value: %w", err)
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Parameter) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
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
