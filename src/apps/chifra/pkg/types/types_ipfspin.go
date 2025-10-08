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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type IpfsPin struct {
	Cid        base.IpfsHash `json:"cid"`
	DatePinned string        `json:"datePinned"`
	FileName   string        `json:"fileName"`
	Size       int64         `json:"size"`
	Status     string        `json:"status"`
	Calcs      *IpfsPinCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s IpfsPin) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *IpfsPin) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"fileName",
		"cid",
		"datePinned",
		"status",
		"size",
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

// RawMap returns a map containing only the raw/base fields for this IpfsPin.
func (s *IpfsPin) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"fileName": s.FileName,
		"cid":      s.Cid,
		"status":   s.Status,
		"size":     s.Size,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *IpfsPin) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"datePinned": cleanDate(s.DatePinned),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *IpfsPin) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// IpfsPinCalcs holds lazy-loaded calculated fields for IpfsPin
type IpfsPinCalcs struct {
	// EXISTING_CODE
	DatePinned string `json:"datePinned"`
	// EXISTING_CODE
}

func (s *IpfsPin) EnsureCalcs(p *ModelProps, requestedFields []string) error {
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

	s.Calcs = &IpfsPinCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
func cleanDate(date string) string {
	parts := strings.Split(date, ".")
	return parts[0]
}

// EXISTING_CODE
