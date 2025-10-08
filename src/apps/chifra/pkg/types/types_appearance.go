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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

const (
	// below are stored in cache, do not change
	BlockReward     base.Txnum = 99999 // the largest five-digit decimal number
	UncleReward     base.Txnum = 99998
	MisconfigReward base.Txnum = 99997
	ExternalReward  base.Txnum = 99996
	WithdrawalAmt   base.Txnum = 99995
	// above are stored in cache, do not change
	NephewReward base.Txnum = 99994
	TxFeeReward  base.Txnum = 99993
)

type ChunkAppearance = Appearance

// EXISTING_CODE

type Appearance struct {
	Address          base.Address     `json:"address"`
	BlockNumber      uint32           `json:"blockNumber"`
	Reason           string           `json:"reason,omitempty"`
	Timestamp        base.Timestamp   `json:"timestamp"`
	TraceIndex       uint32           `json:"traceIndex,omitempty"`
	TransactionIndex uint32           `json:"transactionIndex"`
	Calcs            *AppearanceCalcs `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Appearance) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Appearance) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
	if extraOpts["appearances"] == true {
		order = []string{
			"blockNumber",
			"transactionIndex",
		}
		return Model{
			Data:  model,
			Order: order,
		}
	}

	order = []string{
		"address",
		"blockNumber",
		"transactionIndex",
	}

	if extraOpts["uniq"] == true {
		order = append(order, "traceIndex", "reason")
		if verbose {
			order = append(order, "timestamp", "date")
		}
	} else if extraOpts["export"] == true && format == "json" {
		// Order handled in RawMap/CalcMap
	} else {
		if verbose {
			order = append(order, "traceIndex", "reason", "timestamp", "date")
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

// RawMap returns a map containing only the raw/base fields for this Appearance.
func (s *Appearance) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":          s.Address,
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.ExtraOpts["appearances"] == true {
		model := map[string]any{
			"blockNumber":      s.BlockNumber,
			"transactionIndex": s.TransactionIndex,
		}
		return model
	}

	if p.ExtraOpts["uniq"] == true {
		if s.TraceIndex > 0 {
			model["traceIndex"] = s.TraceIndex
		} else if p.Format != "json" {
			model["traceIndex"] = ""
		}
		model["reason"] = s.Reason
		if p.Verbose {
			model["timestamp"] = s.Timestamp
		}
	} else if p.ExtraOpts["export"] == true && p.Format == "json" {
		if p.Verbose {
			if s.Timestamp != base.NOPOSI {
				model["timestamp"] = s.Timestamp
			}
		}
	} else {
		if p.Verbose {
			if s.TraceIndex > 0 {
				model["traceIndex"] = s.TraceIndex
			} else if p.Format != "json" {
				model["traceIndex"] = ""
			}
			model["reason"] = s.Reason
			model["timestamp"] = s.Timestamp
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Appearance) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.ExtraOpts["appearances"] == true {
		return model
	}

	if p.ExtraOpts["uniq"] == true {
		if p.Verbose {
			model["date"] = s.Date()
		}
	} else if p.ExtraOpts["export"] == true && p.Format == "json" {
		if p.Verbose {
			model["date"] = s.Date()
		}
	} else {
		if p.Verbose {
			model["date"] = s.Date()
		}
	}
	// EXISTING_CODE

	return model
}

func (s *Appearance) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Appearance) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// AppearanceCalcs holds lazy-loaded calculated fields for Appearance
type AppearanceCalcs struct {
	// EXISTING_CODE
	Date string `json:"date,omitempty"`
	// EXISTING_CODE
}

func (s *Appearance) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &AppearanceCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
//

func (s *Appearance) GetKey() string {
	return fmt.Sprintf("%s\t%09d\t%05d", s.Address, s.BlockNumber, s.TransactionIndex)
}

func (s *Appearance) Orig() string {
	return s.Reason // when converted from an Identifier, this is the original string
}

// EXISTING_CODE
