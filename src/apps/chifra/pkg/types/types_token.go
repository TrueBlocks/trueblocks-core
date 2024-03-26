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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawToken struct {
	Address          string `json:"address"`
	Balance          string `json:"balance"`
	BlockNumber      string `json:"blockNumber"`
	Decimals         string `json:"decimals"`
	Diff             string `json:"diff"`
	Holder           string `json:"holder"`
	Name             string `json:"name"`
	PriorBalance     string `json:"priorBalance"`
	Symbol           string `json:"symbol"`
	Timestamp        string `json:"timestamp"`
	TokenType        string `json:"type"`
	TotalSupply      string `json:"totalSupply"`
	TransactionIndex string `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleToken struct {
	Address          base.Address   `json:"address"`
	Balance          base.Wei       `json:"balance"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Decimals         uint64         `json:"decimals"`
	Diff             base.Wei       `json:"diff,omitempty"`
	Holder           base.Address   `json:"holder"`
	Name             string         `json:"name"`
	PriorBalance     base.Wei       `json:"priorBalance,omitempty"`
	Symbol           string         `json:"symbol"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TokenType        TokenType      `json:"type"`
	TotalSupply      base.Wei       `json:"totalSupply"`
	TransactionIndex base.Blknum    `json:"transactionIndex,omitempty"`
	raw              *RawToken      `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleToken) Raw() *RawToken {
	return s.raw
}

func (s *SimpleToken) SetRaw(raw *RawToken) {
	s.raw = raw
}

func (s *SimpleToken) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	name := SimpleName{}
	name.Name = "Unknown"
	if extraOptions["namesMap"] != nil {
		name = extraOptions["namesMap"].(map[base.Address]SimpleName)[s.Address]
	}
	if name.Decimals == 0 {
		name.Decimals = 18
	}
	if name.Symbol == "" {
		name.Symbol = name.Address.Prefix(6)
	}

	wanted := extraOptions["parts"].([]string)
	if len(wanted) == 1 {
		if wanted[0] == "all" {
			if verbose {
				wanted = []string{"address", "blockNumber", "timestamp", "date", "name", "symbol", "decimals", "totalSupply"}
			} else {
				wanted = []string{"address", "blockNumber", "name", "symbol", "decimals", "totalSupply"}
			}
		} else if wanted[0] == "all_held" {
			if verbose {
				wanted = []string{
					"blockNumber", "timestamp", "date", "holder", "address", "name", "symbol", "decimals", "balance", "units",
				}
			} else {
				wanted = []string{
					"blockNumber", "holder", "address", "name", "symbol", "decimals", "balance", "units",
				}
			}
		}
	}

	order = wanted
	if len(wanted) > 0 && (wanted[0] != "address" && wanted[0] != "blockNumber") {
		if verbose && false {
			order = append([]string{"address", "blockNumber", "date"}, wanted...)
		} else {
			order = append([]string{"address", "blockNumber"}, wanted...)
		}
	}

	for _, part := range order {
		switch part {
		case "address":
			model["address"] = s.Address
		case "balance":
			model["balance"] = base.FormattedValue(&s.Balance, true, int(name.Decimals))
		case "blockNumber":
			model["blockNumber"] = s.BlockNumber
		case "date":
			model["date"] = s.Date()
		case "decimals":
			model["decimals"] = name.Decimals
		case "diff":
			model["diff"] = base.FormattedValue(&s.Diff, true, int(name.Decimals))
		case "holder":
			model["holder"] = s.Holder
		case "name":
			model["name"] = name.Name
		case "symbol":
			model["symbol"] = name.Symbol
		case "timestamp":
			model["timestamp"] = s.Timestamp
		case "totalSupply":
			model["totalSupply"] = base.FormattedValue(&s.TotalSupply, true, int(name.Decimals))
		case "transactionIndex":
			model["transactionIndex"] = s.TransactionIndex
		case "units":
			model["units"] = base.FormattedValue(&s.Balance, false, int(name.Decimals)) // present underlying units
		case "version":
			model["version"] = ""
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleToken) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleToken) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *SimpleToken) IsErc20() bool {
	return s.TokenType.IsErc20()
}

func (s *SimpleToken) IsErc721() bool {
	return s.TokenType.IsErc721()
}

type TokenType int

const (
	TokenErc20 TokenType = iota
	TokenErc721
)

func (t TokenType) IsErc20() bool {
	return t == TokenErc20
}

func (t TokenType) IsErc721() bool {
	return t == TokenErc721
}

// EXISTING_CODE
