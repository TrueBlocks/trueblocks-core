// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

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
	Deleted    bool    `json:"deleted"`
	IsCustom   bool    `json:"isCustom"`
	IsPrefund  bool    `json:"isPrefund"`
	IsContract bool    `json:"isContract"`
	IsErc20    bool    `json:"isErc20"`
	IsErc721   bool    `json:"isErc721"`
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
		"tags":       s.Tags,
		"address":    s.Address,
		"name":       s.Name,
		"symbol":     s.Symbol,
		"source":     s.Source,
		"petname":    s.Petname,
		"deleted":    s.Deleted,
		"isCustom":   s.IsCustom,
		"isPrefund":  s.IsPrefund,
		"isContract": s.IsContract,
		"isErc20":    s.IsErc20,
		"isErc721":   s.IsErc721,
	}

	order := []string{
		"tags",
		"address",
		"name",
		"symbol",
		"source",
		"petname",
		"deleted",
		"isCustom",
		"isPrefund",
		"isContract",
		"isErc20",
		"isErc721",
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
