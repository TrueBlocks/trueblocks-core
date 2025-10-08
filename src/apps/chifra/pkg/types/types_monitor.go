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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type Monitor struct {
	Address     base.Address  `json:"address"`
	Deleted     bool          `json:"deleted"`
	FileSize    int64         `json:"fileSize"`
	IsEmpty     bool          `json:"isEmpty"`
	IsStaged    bool          `json:"isStaged"`
	LastScanned uint32        `json:"lastScanned"`
	NRecords    int64         `json:"nRecords"`
	Name        string        `json:"name"`
	Calcs       *MonitorCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Monitor) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Monitor) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"address",
		"nRecords",
		"fileSize",
	}

	if extraOpts["list"] == true {
		order = append(order, "isEmpty")
		order = append(order, "isStaged")
	}

	if verbose {
		order = append(order, "lastScanned")
		order = append(order, "deleted")
	}

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)
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

// RawMap returns a map containing only the raw/base fields for this Monitor.
func (s *Monitor) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":  s.Address,
		"nRecords": s.NRecords,
		"fileSize": s.FileSize,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Monitor) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.ExtraOpts["list"] == true {
		model["isEmpty"] = s.IsEmpty
		model["isStaged"] = s.IsStaged
	}

	if p.Verbose {
		model["lastScanned"] = s.LastScanned
		model["deleted"] = s.Deleted
		if p.ExtraOpts["testMode"] == true {
			model["lastScanned"] = "--lastScanned--"
		}
	}
	// EXISTING_CODE

	return model
}

func (s *Monitor) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Deleted
	if err = base.WriteValue(writer, s.Deleted); err != nil {
		return err
	}

	// FileSize
	if err = base.WriteValue(writer, s.FileSize); err != nil {
		return err
	}

	// IsEmpty
	if err = base.WriteValue(writer, s.IsEmpty); err != nil {
		return err
	}

	// IsStaged
	if err = base.WriteValue(writer, s.IsStaged); err != nil {
		return err
	}

	// LastScanned
	if err = base.WriteValue(writer, s.LastScanned); err != nil {
		return err
	}

	// NRecords
	if err = base.WriteValue(writer, s.NRecords); err != nil {
		return err
	}

	// Name
	if err = base.WriteValue(writer, s.Name); err != nil {
		return err
	}

	return nil
}

func (s *Monitor) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// Deleted
	if err = base.ReadValue(reader, &s.Deleted, fileVersion); err != nil {
		return err
	}

	// FileSize
	if err = base.ReadValue(reader, &s.FileSize, fileVersion); err != nil {
		return err
	}

	// IsEmpty
	vIsEmpty := version.NewVersion("3.1.2")
	if fileVersion > vIsEmpty.Uint64() {
		// IsEmpty
		if err = base.ReadValue(reader, &s.IsEmpty, fileVersion); err != nil {
			return err
		}
	}

	// IsStaged
	vIsStaged := version.NewVersion("3.1.2")
	if fileVersion > vIsStaged.Uint64() {
		// IsStaged
		if err = base.ReadValue(reader, &s.IsStaged, fileVersion); err != nil {
			return err
		}
	}

	// LastScanned
	if err = base.ReadValue(reader, &s.LastScanned, fileVersion); err != nil {
		return err
	}

	// NRecords
	if err = base.ReadValue(reader, &s.NRecords, fileVersion); err != nil {
		return err
	}

	// Name
	if err = base.ReadValue(reader, &s.Name, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Monitor) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// MonitorCalcs holds lazy-loaded calculated fields for Monitor
type MonitorCalcs struct {
	// EXISTING_CODE
	IsEmpty  bool `json:"isEmpty,omitempty"`
	IsStaged bool `json:"isStaged,omitempty"`
	// EXISTING_CODE
}

func (s *Monitor) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &MonitorCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// EXISTING_CODE
