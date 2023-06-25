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
	"io"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawTokenBal struct {
	Address          string `json:"address"`
	Balance          string `json:"balance"`
	BlockNumber      string `json:"blockNumber"`
	Date             string `json:"date"`
	Decimals         string `json:"decimals"`
	Diff             string `json:"diff"`
	Holder           string `json:"holder"`
	IsContract       string `json:"isContract"`
	IsErc20          string `json:"isErc20"`
	IsErc721         string `json:"isErc721"`
	Name             string `json:"name"`
	PriorBalance     string `json:"priorBalance"`
	Symbol           string `json:"symbol"`
	TotalSupply      string `json:"totalSupply"`
	TransactionIndex string `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTokenBal struct {
	Address          base.Address `json:"address"`
	Balance          big.Int      `json:"balance"`
	BlockNumber      base.Blknum  `json:"blockNumber"`
	Date             string       `json:"date"`
	Decimals         uint64       `json:"decimals"`
	Diff             big.Int      `json:"diff"`
	Holder           base.Address `json:"holder"`
	IsContract       bool         `json:"isContract"`
	IsErc20          bool         `json:"isErc20"`
	IsErc721         bool         `json:"isErc721"`
	Name             string       `json:"name"`
	PriorBalance     big.Int      `json:"priorBalance"`
	Symbol           string       `json:"symbol"`
	TotalSupply      big.Int      `json:"totalSupply"`
	TransactionIndex base.Blknum  `json:"transactionIndex"`
	raw              *RawTokenBal `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTokenBal) Raw() *RawTokenBal {
	return s.raw
}

func (s *SimpleTokenBal) SetRaw(raw *RawTokenBal) {
	s.raw = raw
}

func (s *SimpleTokenBal) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	asEther := false
	model = map[string]interface{}{
		"address":     s.Address,
		"balance":     utils.FormattedValue(s.Balance, asEther, int(s.Decimals)),
		"blockNumber": s.BlockNumber,
		"decimals":    s.Decimals,
		"holder":      s.Holder,
		"isContract":  s.IsContract,
		"isErc20":     s.IsErc20,
		"name":        s.Name,
		"symbol":      s.Symbol,
	}
	order = []string{
		"address", "balance", "blockNumber", "decimals",
		"holder", "isContract", "isErc20",
		"name", "symbol",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTokenBal) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTokenBal) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
