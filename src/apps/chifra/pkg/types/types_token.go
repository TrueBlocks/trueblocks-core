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
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type Token struct {
	Address          base.Address   `json:"address"`
	Balance          base.Wei       `json:"balance"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Decimals         uint64         `json:"decimals"`
	Holder           base.Address   `json:"holder"`
	Name             string         `json:"name"`
	PriorBalance     base.Wei       `json:"priorBalance,omitempty"`
	Symbol           string         `json:"symbol"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TotalSupply      base.Wei       `json:"totalSupply"`
	TransactionIndex base.Txnum     `json:"transactionIndex,omitempty"`
	TokenType        TokenType      `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Token) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Token) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	name := Name{}
	if addressName, _, found := nameAddress(extraOpts, s.Address); found {
		name = addressName
	}
	if name.Decimals == 0 {
		name.Decimals = 18
	}
	if name.Symbol == "" {
		name.Symbol = name.Address.DefaultSymbol()
	}

	wanted := extraOpts["parts"].([]string)
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
					"blockNumber", "timestamp", "date", "holder", "address", "name", "symbol", "decimals", "balance", "balanceDec",
				}
			} else {
				wanted = []string{
					"blockNumber", "holder", "address", "name", "symbol", "decimals", "balance", "balanceDec",
				}
			}
		}
	}

	order = wanted
	if len(wanted) > 0 && (wanted[0] != "address" && wanted[0] != "blockNumber") {
		order = append([]string{"address", "blockNumber"}, wanted...)
	}

	for _, part := range order {
		switch part {
		case "address":
			model["address"] = s.Address
		case "balance":
			model["balance"] = s.Balance.String()
		case "balanceDec":
			model["balanceDec"] = s.Balance.ToFloatStr(int(name.Decimals))
		case "blockNumber":
			model["blockNumber"] = s.BlockNumber
		case "date":
			model["date"] = s.Date()
		case "decimals":
			model["decimals"] = name.Decimals
		case "diff":
			model["diff"] = s.formattedDiff(name.Decimals)
		case "holder":
			model["holder"] = s.Holder
		case "name":
			model["name"] = name.Name
		case "symbol":
			if name.Symbol != "" && name.Symbol != "0x0" {
				model["symbol"] = name.Symbol
			} else {
				model["symbol"] = ""
			}
		case "timestamp":
			model["timestamp"] = s.Timestamp
		case "totalSupply":
			model["totalSupply"] = s.TotalSupply.ToFloatStr(int(name.Decimals))
		case "transactionIndex":
			model["transactionIndex"] = s.TransactionIndex
		case "version":
			model["version"] = ""
		}
	}

	if verbose {
		if name, loaded, found := nameAddress(extraOpts, s.Holder); found {
			model["holderName"] = name.Name
			order = append(order, "holderName")
		} else if loaded && format != "json" {
			model["holderName"] = ""
			order = append(order, "holderName")
		}
	}
	order = reorderOrdering(order)

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Token) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Token) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *Token) IsErc20() bool {
	return s.TokenType.IsErc20()
}

func (s *Token) IsErc721() bool {
	return s.TokenType.IsErc721()
}

func (s *Token) formattedDiff(dec uint64) string {
	b := s.Balance.BigInt()
	pB := s.PriorBalance.BigInt()
	diff := new(big.Int).Sub(b, pB)
	if diff.Sign() == -1 {
		diff = diff.Neg(diff)
		return "-" + (*base.Wei)(diff).ToFloatStr(int(dec))
	}
	return (*base.Wei)(diff).ToFloatStr(int(dec))
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
