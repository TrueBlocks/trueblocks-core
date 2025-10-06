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

type AddrRecord struct {
	Address base.Address `json:"address"`
	Offset  uint32       `json:"offset"`
	Count   uint32       `json:"count"`
}

type AppRecord struct {
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
}

// EXISTING_CODE

type AppearanceTable struct {
	AddressRecord AddrRecord  `json:"AddressRecord"`
	Appearances   []AppRecord `json:"Appearances"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s AppearanceTable) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *AppearanceTable) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"address",
		"offset",
		"count",
		"appearances",
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

// RawMap returns a map containing only the raw/base fields for this AppearanceTable.
func (s *AppearanceTable) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":     s.AddressRecord.Address.Hex(),
		"offset":      s.AddressRecord.Offset,
		"count":       s.AddressRecord.Count,
		"appearances": s.Appearances,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this AppearanceTable.
func (s *AppearanceTable) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *AppearanceTable) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
