// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
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
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleParameter struct {
	Components    []SimpleParameter `json:"components,omitempty"`
	Indexed       bool              `json:"indexed,omitempty"`
	InternalType  string            `json:"internalType,omitempty"`
	Name          string            `json:"name"`
	StrDefault    string            `json:"strDefault,omitempty"`
	ParameterType string            `json:"type"`
	Value         any               `json:"value,omitempty"`
	raw           *RawParameter     `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleParameter) Raw() *RawParameter {
	return s.raw
}

func (s *SimpleParameter) SetRaw(raw *RawParameter) {
	s.raw = raw
}

func (s *SimpleParameter) Model(verbose bool, format string, extraOptions map[string]any) Model {
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

// EXISTING_CODE
//

// DisplayName returns parameter name if defined, or a default name "val_" + index
func (s *SimpleParameter) DisplayName(index int) string {
	if s.Name != "" {
		return s.Name
	}
	return "val_" + fmt.Sprint(index)
}

func parametersToMap(params []SimpleParameter) (result map[string]any) {
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

func (s *SimpleParameter) MarshalCache(writer io.Writer) (err error) {
	if err = cacheNew.WriteValue(writer, s.Components); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Indexed); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.InternalType); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Name); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.StrDefault); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.ParameterType); err != nil {
		return err
	}

	// s.Value can be anything, so we'll just marshal it into a string
	value, err := json.Marshal(s.Value)
	if err != nil {
		return fmt.Errorf("cannot marshal Value: %w", err)
	}
	if err = cacheNew.WriteValue(writer, value); err != nil {
		return err
	}

	return
}

func (s *SimpleParameter) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	s.Components = make([]SimpleParameter, 0)
	if err = cacheNew.ReadValue(reader, &s.Components, version); err != nil {
		return err
	}

	if err = cacheNew.ReadValue(reader, &s.Indexed, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.InternalType, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Name, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.StrDefault, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.ParameterType, version); err != nil {
		return err
	}

	// s.Value can be anything, so we store it as a marshaled string
	var value string
	if err = cacheNew.ReadValue(reader, &value, version); err != nil {
		return err
	}
	if err = json.Unmarshal([]byte(value), &s.Value); err != nil {
		return fmt.Errorf("cannot unmarshal Value: %w", err)
	}

	return
}

// EXISTING_CODE
