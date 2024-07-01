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
	// EXISTING_CODE
}

func (s Name) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Name) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	if extraOpts["single"] == "tags" || extraOpts["single"] == "address" {
		if extraOpts["single"] == "tags" {
			model["tags"] = s.Tags
		} else {
			model["address"] = s.Address.Hex()
		}
		order = append(order, extraOpts["single"].(string))
		return Model{
			Data:  model,
			Order: order,
		}
	}

	model = map[string]any{
		"address":  s.Address,
		"decimals": s.Decimals,
		"name":     s.Name,
		"source":   s.Source,
		"symbol":   s.Symbol,
		"tags":     s.Tags,
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
	}

	isExpanded := extraOpts["expand"] == true
	isPrefund := extraOpts["prefund"] == true
	if !isExpanded && !isPrefund {
		x := []string{}
		for _, v := range order {
			if v != "source" {
				x = append(x, v)
			}
		}
		order = x
		delete(model, "source")
	}

	if isExpanded && isPrefund {
		model["prefund"] = s.Prefund.String()
		order = append(order, "prefund")
	}

	if format == "json" {
		if len(s.Symbol) == 0 {
			x := []string{}
			for _, v := range order {
				if v != "symbol" {
					x = append(x, v)
				}
			}
			order = x
			delete(model, "symbol")
		} else {
			model["symbol"] = s.Symbol
		}
		if s.Decimals == 0 {
			x := []string{}
			for _, v := range order {
				if v != "decimals" {
					x = append(x, v)
				}
			}
			order = x
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

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Name) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

// EXISTING_CODE
