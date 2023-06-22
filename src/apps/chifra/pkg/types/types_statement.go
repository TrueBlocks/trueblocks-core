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

type RawStatement struct {
	AccountedFor        string `json:"accountedFor"`
	AmountIn            string `json:"amountIn"`
	AmountOut           string `json:"amountOut"`
	AssetAddr           string `json:"assetAddr"`
	AssetSymbol         string `json:"assetSymbol"`
	BegBal              string `json:"begBal"`
	BlockNumber         string `json:"blockNumber"`
	CorrectingIn        string `json:"correctingIn"`
	CorrectingOut       string `json:"correctingOut"`
	CorrectingReason    string `json:"correctingReason"`
	Decimals            string `json:"decimals"`
	EndBal              string `json:"endBal"`
	GasOut              string `json:"gasOut"`
	InternalIn          string `json:"internalIn"`
	InternalOut         string `json:"internalOut"`
	LogIndex            string `json:"logIndex"`
	MinerBaseRewardIn   string `json:"minerBaseRewardIn"`
	MinerNephewRewardIn string `json:"minerNephewRewardIn"`
	MinerTxFeeIn        string `json:"minerTxFeeIn"`
	MinerUncleRewardIn  string `json:"minerUncleRewardIn"`
	PrefundIn           string `json:"prefundIn"`
	PrevAppBlk          string `json:"prevAppBlk"`
	PrevBal             string `json:"prevBal"`
	PriceSource         string `json:"priceSource"`
	Recipient           string `json:"recipient"`
	ReconciliationType  string `json:"reconciliationType"`
	SelfDestructIn      string `json:"selfDestructIn"`
	SelfDestructOut     string `json:"selfDestructOut"`
	Sender              string `json:"sender"`
	SpotPrice           string `json:"spotPrice"`
	Timestamp           string `json:"timestamp"`
	TransactionHash     string `json:"transactionHash"`
	TransactionIndex    string `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleStatement struct {
	AccountedFor        base.Address   `json:"accountedFor"`
	AmountIn            big.Int        `json:"amountIn,omitempty"`
	AmountOut           big.Int        `json:"amountOut,omitempty"`
	AssetAddr           base.Address   `json:"assetAddr"`
	AssetSymbol         string         `json:"assetSymbol"`
	BegBal              big.Int        `json:"begBal"`
	BlockNumber         base.Blknum    `json:"blockNumber"`
	CorrectingIn        big.Int        `json:"correctingIn,omitempty"`
	CorrectingOut       big.Int        `json:"correctingOut,omitempty"`
	CorrectingReason    string         `json:"correctingReason,omitempty"`
	Decimals            uint64         `json:"decimals"`
	EndBal              big.Int        `json:"endBal"`
	GasOut              big.Int        `json:"gasOut,omitempty"`
	InternalIn          big.Int        `json:"internalIn,omitempty"`
	InternalOut         big.Int        `json:"internalOut,omitempty"`
	LogIndex            base.Blknum    `json:"logIndex"`
	MinerBaseRewardIn   big.Int        `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn big.Int        `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        big.Int        `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  big.Int        `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           big.Int        `json:"prefundIn,omitempty"`
	PrevAppBlk          base.Blknum    `json:"prevAppBlk,omitempty"`
	PrevBal             big.Int        `json:"prevBal,omitempty"`
	PriceSource         string         `json:"priceSource"`
	Recipient           base.Address   `json:"recipient"`
	ReconciliationType  string         `json:"reconciliationType"`
	SelfDestructIn      big.Int        `json:"selfDestructIn,omitempty"`
	SelfDestructOut     big.Int        `json:"selfDestructOut,omitempty"`
	Sender              base.Address   `json:"sender"`
	SpotPrice           float64        `json:"spotPrice"`
	Timestamp           base.Timestamp `json:"timestamp"`
	TransactionHash     base.Hash      `json:"transactionHash"`
	TransactionIndex    base.Blknum    `json:"transactionIndex"`
	raw                 *RawStatement  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleStatement) Raw() *RawStatement {
	return s.raw
}

func (s *SimpleStatement) SetRaw(raw *RawStatement) {
	s.raw = raw
}

func (s *SimpleStatement) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleStatement) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleStatement) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
