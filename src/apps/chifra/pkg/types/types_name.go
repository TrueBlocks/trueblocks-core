// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

import "strings"

// EXISTING_CODE
// EXISTING_CODE

type RawName struct {
	Tags       string `json:"tags"`
	Address    string `json:"address"`
	Name       string `json:"name"`
	Symbol     string `json:"symbol"`
	Source     string `json:"source"`
	Decimals   string `json:"decimals"`
	Petname    string `json:"petname"`
	Deleted    string `json:"deleted"`
	IsCustom   string `json:"isCustom"`
	IsPrefund  string `json:"isPrefund"`
	IsContract string `json:"isContract"`
	IsErc20    string `json:"isErc20"`
	IsErc721   string `json:"isErc721"`
}

type SimpleName struct {
	Tags       string  `json:"tags"`
	Address    Address `json:"address"`
	Name       string  `json:"name"`
	Symbol     string  `json:"symbol"`
	Source     string  `json:"source"`
	Decimals   uint64  `json:"decimals,omitempty"`
	Petname    string  `json:"petname"`
	Deleted    bool    `json:"deleted,omitempty"`
	IsCustom   bool    `json:"isCustom,omitempty"`
	IsPrefund  bool    `json:"isPrefund,omitempty"`
	IsContract bool    `json:"isContract,omitempty"`
	IsErc20    bool    `json:"isErc20,omitempty"`
	IsErc721   bool    `json:"isErc721,omitempty"`
	raw        *RawName
}

func (s *SimpleName) Raw() *RawName {
	return s.raw
}

func (s *SimpleName) SetRaw(raw *RawName) {
	s.raw = raw
}

func (s *SimpleName) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"tags":     s.Tags,
		"address":  s.Address,
		"name":     s.Name,
		"symbol":   s.Symbol,
		"source":   s.Source,
		"decimals": s.Decimals,
		"petname":  s.Petname,
	}

	order := []string{
		"tags",
		"address",
		"name",
		"symbol",
		"source",
		"decimals",
		"petname",
	}

	// EXISTING_CODE
	if len(s.Address.Bytes()) > 0 && s.Address != HexToAddress("0x0") {
		model["address"] = strings.ToLower(s.Address.String())
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
		} else {
			model["decimals"] = s.Decimals
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
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
