// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawTokenBalance struct {
	Address          string `json:"address"`
	Balance          string `json:"balance"`
	BlockNumber      string `json:"blockNumber"`
	Decimals         string `json:"decimals"`
	Diff             string `json:"diff"`
	Holder           string `json:"holder"`
	IsContract       string `json:"isContract"`
	IsErc20          string `json:"isErc20"`
	IsErc721         string `json:"isErc721"`
	Name             string `json:"name"`
	PriorBalance     string `json:"priorBalance"`
	Symbol           string `json:"symbol"`
	Timestamp        string `json:"timestamp"`
	TotalSupply      string `json:"totalSupply"`
	TransactionIndex string `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTokenBalance struct {
	Address          base.Address     `json:"address"`
	Balance          big.Int          `json:"balance"`
	BlockNumber      base.Blknum      `json:"blockNumber"`
	Decimals         uint64           `json:"decimals"`
	Diff             big.Int          `json:"diff,omitempty"`
	Holder           base.Address     `json:"holder"`
	IsContract       bool             `json:"isContract,omitempty"`
	IsErc20          bool             `json:"isErc20,omitempty"`
	IsErc721         bool             `json:"isErc721,omitempty"`
	Name             string           `json:"name"`
	PriorBalance     big.Int          `json:"priorBalance,omitempty"`
	Symbol           string           `json:"symbol"`
	Timestamp        base.Timestamp   `json:"timestamp"`
	TotalSupply      big.Int          `json:"totalSupply"`
	TransactionIndex base.Blknum      `json:"transactionIndex,omitempty"`
	raw              *RawTokenBalance `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTokenBalance) Raw() *RawTokenBalance {
	return s.raw
}

func (s *SimpleTokenBalance) SetRaw(raw *RawTokenBalance) {
	s.raw = raw
}

func (s *SimpleTokenBalance) Model(verbose bool, format string, extraOptions map[string]any) Model {
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
			if verbose && false {
				wanted = []string{"address", "blockNumber", "date", "name", "symbol", "decimals", "totalSupply"}
			} else {
				wanted = []string{"address", "blockNumber", "name", "symbol", "decimals", "totalSupply"}
			}
		} else if wanted[0] == "all_held" {
			if verbose && false {
				wanted = []string{
					"blockNumber", "date", "holder", "address", "name", "symbol", "decimals", "balance", "units",
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
			model["balance"] = utils.FormattedValue(s.Balance, true, int(name.Decimals))
		case "blockNumber":
			model["blockNumber"] = s.BlockNumber
		case "date":
			model["date"] = s.Date()
		case "decimals":
			model["decimals"] = name.Decimals
		case "diff":
			model["diff"] = utils.FormattedValue(s.Diff, true, int(name.Decimals))
		case "holder":
			model["holder"] = s.Holder
		case "name":
			model["name"] = name.Name
		case "symbol":
			model["symbol"] = name.Symbol
		case "timestamp":
			model["timestamp"] = s.Timestamp
		case "totalSupply":
			model["totalSupply"] = utils.FormattedValue(s.TotalSupply, true, int(name.Decimals))
		case "transactionIndex":
			model["transactionIndex"] = s.TransactionIndex
		case "units":
			model["units"] = utils.FormattedValue(s.Balance, false, int(name.Decimals)) // present underlying units
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

// EXISTING_CODE
//

func (s *SimpleTokenBalance) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// EXISTING_CODE
