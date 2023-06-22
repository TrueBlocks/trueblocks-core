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
)

// EXISTING_CODE

type RawTransfer struct {
	Amount           string `json:"amount"`
	AssetAddr        string `json:"assetAddr"`
	AssetSymbol      string `json:"assetSymbol"`
	BlockNumber      string `json:"blockNumber"`
	Date             string `json:"date"`
	Decimals         string `json:"decimals"`
	Encoding         string `json:"encoding"`
	LogIndex         string `json:"logIndex"`
	PriceSource      string `json:"priceSource"`
	Recipient        string `json:"recipient"`
	Sender           string `json:"sender"`
	SpotPrice        string `json:"spotPrice"`
	Timestamp        string `json:"timestamp"`
	TransactionHash  string `json:"transactionHash"`
	TransactionIndex string `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTransfer struct {
	Amount           big.Int        `json:"amount"`
	AssetAddr        base.Address   `json:"assetAddr"`
	AssetSymbol      string         `json:"assetSymbol"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Date             string         `json:"date"`
	Decimals         uint64         `json:"decimals"`
	Encoding         string         `json:"encoding"`
	LogIndex         base.Blknum    `json:"logIndex"`
	PriceSource      string         `json:"priceSource"`
	Recipient        base.Address   `json:"recipient"`
	Sender           base.Address   `json:"sender"`
	SpotPrice        float64        `json:"spotPrice"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TransactionHash  base.Hash      `json:"transactionHash"`
	TransactionIndex base.Blknum    `json:"transactionIndex"`
	raw              *RawTransfer   `json:"-"`
	// EXISTING_CODE
	Log *SimpleLog `json:"-"`
	// EXISTING_CODE
}

func (s *SimpleTransfer) Raw() *RawTransfer {
	return s.raw
}

func (s *SimpleTransfer) SetRaw(raw *RawTransfer) {
	s.raw = raw
}

func (s *SimpleTransfer) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTransfer) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTransfer) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
