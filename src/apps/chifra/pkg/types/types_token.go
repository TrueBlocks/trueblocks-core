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
	Calcs            *TokenCalcs    `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Token) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Token) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Holder, "holder"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	wanted := extraOpts["parts"].([]string)
	if len(wanted) == 1 {
		switch wanted[0] {
		case "all":
			if verbose {
				wanted = []string{"address", "blockNumber", "timestamp", "date", "name", "symbol", "decimals", "totalSupply"}
			} else {
				wanted = []string{"address", "blockNumber", "name", "symbol", "decimals", "totalSupply"}
			}
		case "all_held":
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

// RawMap returns a map containing only the raw/base fields for this Token.
func (s *Token) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	name := Name{}
	if addressName, _, found := labelAddress(p.ExtraOpts, s.Address); found {
		name = addressName
	}
	if name.Decimals == 0 {
		name.Decimals = 18
	}
	if name.Symbol == "" {
		name.Symbol = name.Address.DefaultSymbol()
	}

	// Determine which fields to include based on order logic from original
	wanted := p.ExtraOpts["parts"].([]string)
	if len(wanted) == 1 {
		switch wanted[0] {
		case "all":
			if p.Verbose {
				wanted = []string{"address", "blockNumber", "timestamp", "date", "name", "symbol", "decimals", "totalSupply"}
			} else {
				wanted = []string{"address", "blockNumber", "name", "symbol", "decimals", "totalSupply"}
			}
		case "all_held":
			if p.Verbose {
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

	order := wanted
	if len(wanted) > 0 && (wanted[0] != "address" && wanted[0] != "blockNumber") {
		order = append([]string{"address", "blockNumber"}, wanted...)
	}

	// Add fields based on order - raw fields only
	for _, part := range order {
		switch part {
		case "address":
			model["address"] = s.Address
		case "balance":
			model["balance"] = s.Balance.String()
		case "blockNumber":
			model["blockNumber"] = s.BlockNumber
		case "decimals":
			model["decimals"] = name.Decimals
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
		case "transactionIndex":
			model["transactionIndex"] = s.TransactionIndex
		case "version":
			model["version"] = ""
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Token) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	name := Name{}
	if addressName, _, found := labelAddress(p.ExtraOpts, s.Address); found {
		name = addressName
	}
	if name.Decimals == 0 {
		name.Decimals = 18
	}

	// Determine which fields to include based on order logic from original
	wanted := p.ExtraOpts["parts"].([]string)
	if len(wanted) == 1 {
		switch wanted[0] {
		case "all":
			if p.Verbose {
				wanted = []string{"address", "blockNumber", "timestamp", "date", "name", "symbol", "decimals", "totalSupply"}
			} else {
				wanted = []string{"address", "blockNumber", "name", "symbol", "decimals", "totalSupply"}
			}
		case "all_held":
			if p.Verbose {
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

	order := wanted
	if len(wanted) > 0 && (wanted[0] != "address" && wanted[0] != "blockNumber") {
		order = append([]string{"address", "blockNumber"}, wanted...)
	}

	// Add calculated/derived fields based on order
	for _, part := range order {
		switch part {
		case "balanceDec":
			model["balanceDec"] = s.Balance.ToFloatString(int(name.Decimals))
		case "date":
			model["date"] = s.Date()
		case "diff":
			model["diff"] = s.formattedDiff(name.Decimals)
		case "totalSupply":
			model["totalSupply"] = s.TotalSupply.ToFloatString(int(name.Decimals))
		}
	}
	// EXISTING_CODE

	return model
}

func (s *Token) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Token) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// TokenCalcs holds lazy-loaded calculated fields for Token
type TokenCalcs struct {
	// EXISTING_CODE
	BalanceDec  string `json:"balanceDec,omitempty"`
	Date        string `json:"date,omitempty"`
	Diff        string `json:"diff,omitempty"`
	TotalSupply string `json:"totalSupply,omitempty"`
	// EXISTING_CODE
}

func (s *Token) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &TokenCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
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
		return "-" + (*base.Wei)(diff).ToFloatString(int(dec))
	}
	return (*base.Wei)(diff).ToFloatString(int(dec))
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
