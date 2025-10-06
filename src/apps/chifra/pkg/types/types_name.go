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

type Name struct {
	Address    base.Address `json:"address"`
	Decimals   uint64       `json:"decimals"`
	Deleted    bool         `json:"deleted,omitempty"`
	IsContract bool         `json:"isContract,omitempty"`
	IsCustom   bool         `json:"isCustom,omitempty"`
	IsErc20    bool         `json:"isErc20,omitempty"`
	IsErc721   bool         `json:"isErc721,omitempty"`
	IsPrefund  bool         `json:"isPrefund,omitempty"`
	Name       string       `json:"name"`
	Source     string       `json:"source"`
	Symbol     string       `json:"symbol"`
	Tags       string       `json:"tags"`
	// EXISTING_CODE
	Prefund base.Wei `json:"prefund,omitempty"`
	Parts   Parts    `json:"parts,omitempty"`
	// EXISTING_CODE
}

func (s Name) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Name) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	switch extraOpts["single"] {
	case "tags":
		return Model{
			Data:  map[string]any{"tags": s.Tags},
			Order: []string{"tags"},
		}
	case "address":
		return Model{
			Data:  map[string]any{"address": s.Address.Hex()},
			Order: []string{"address"},
		}
	case "asset":
		return Model{
			Data: map[string]any{
				"address":  s.Address.Hex(),
				"symbol":   s.Symbol,
				"name":     s.Name,
				"decimals": s.Decimals,
			},
			Order: []string{"address", "symbol", "name", "decimals"},
		}
	}

	order = []string{
		"tags",
		"address",
		"name",
		"symbol",
		"source",
		"decimals",
	}

	if len(s.Address.Bytes()) > 0 && s.Address != base.ZeroAddr {
		model["address"] = strings.ToLower(s.Address.String())
	} else {
		n := 0
		for _, v := range order {
			if v != "address" {
				order[n] = v
				n++
			}
		}
		order = order[:n]
		delete(model, "address")
	}

	isExpanded := extraOpts["expand"] == true
	isPrefund := extraOpts["prefund"] == true
	if !isExpanded && !isPrefund {
		n := 0
		for _, v := range order {
			if v != "source" {
				order[n] = v
				n++
			}
		}
		order = order[:n]
		delete(model, "source")
	}

	if isExpanded && isPrefund {
		model["prefund"] = s.Prefund.String()
		order = append(order, "prefund")
	}

	if format == "json" {
		if len(s.Symbol) == 0 {
			n := 0
			for _, v := range order {
				if v != "symbol" {
					order[n] = v
					n++
				}
			}
			order = order[:n]
			delete(model, "symbol")
		} else {
			model["symbol"] = s.Symbol
		}
		if s.Decimals == 0 {
			n := 0
			for _, v := range order {
				if v != "decimals" {
					order[n] = v
					n++
				}
			}
			order = order[:n]
			delete(model, "decimals")
		}
		if len(s.Source) > 0 {
			model["source"] = s.Source
			order = append(order, "source")
		}
		if s.Deleted {
			model["deleted"] = true
			order = append(order, "deleted")
		}
		if s.IsCustom {
			model["isCustom"] = true
			order = append(order, "isCustom")
		}
		if s.IsPrefund {
			model["isPrefund"] = true
			order = append(order, "isPrefund")
		}
		if s.IsContract {
			model["isContract"] = true
			order = append(order, "isContract")
		}
		if s.IsErc20 {
			model["isErc20"] = true
			order = append(order, "isErc20")
		}
		if s.IsErc721 {
			model["isErc721"] = true
			order = append(order, "isErc721")
		}
		if verbose && s.Parts != 0 && s.Parts != None {
			model["parts"] = s.Parts
			order = append(order, "parts")
		}
	} else {
		if len(s.Symbol) == 0 {
			model["symbol"] = ""
		}
		if s.Decimals == 0 {
			model["decimals"] = ""
		}

		if isExpanded {
			model["deleted"] = s.Deleted
			order = append(order, "deleted")
			model["isCustom"] = s.IsCustom
			order = append(order, "isCustom")
			model["isPrefund"] = s.IsPrefund
			order = append(order, "isPrefund")
			model["isContract"] = s.IsContract
			order = append(order, "isContract")
			model["isErc20"] = s.IsErc20
			order = append(order, "isErc20")
			model["isErc721"] = s.IsErc721
			order = append(order, "isErc721")
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

// RawMap returns a map containing only the raw/base fields for this Name.
// This excludes any calculated or derived fields.
func (s *Name) RawMap(p *ModelProps, needed []Labeler) map[string]any {
	model := map[string]any{
		"decimals": s.Decimals,
		"name":     s.Name,
		"source":   s.Source,
		"symbol":   s.Symbol,
		"tags":     s.Tags,
	}

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing only the calculated/derived fields for this Name.
// This is optimized for streaming contexts where the frontend receives the raw Name
// and needs to enhance it with calculated values.
func (s *Name) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{}

	// No calculated fields in Name - all processing happens in Model()

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Name) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
type Parts int

// Parts is a bitfield that defines what parts of a name to return and other options
const (
	None Parts = (1 << iota)
	Regular
	Custom
	Prefund
	Baddress
	Testing
	MatchCase
	Expanded
	Tags
	All = Regular | Custom | Prefund | Baddress
)

type SortBy int

// SortBy is a bitfield that defines how to sort the names
const (
	SortByAddress SortBy = iota
	SortByTags
)

func (n *Name) IsAirdrop() bool {
	str := strings.ToLower(n.Name + " " + n.Symbol + " " + n.Source + " " + n.Tags)
	searches := []string{"airdrop", "claim", "visit"}
	for _, search := range searches {
		if strings.Contains(str, search) {
			return true
		}
	}
	return false
}

// EXISTING_CODE
