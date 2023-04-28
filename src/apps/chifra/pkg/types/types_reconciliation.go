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

type RawReconciliation struct {
	AccountedFor        string `json:"accountedFor"`
	AmountIn            string `json:"amountIn"`
	AmountNet           string `json:"amountNet"`
	AmountOut           string `json:"amountOut"`
	AssetAddr           string `json:"assetAddr"`
	AssetSymbol         string `json:"assetSymbol"`
	BegBal              string `json:"begBal"`
	BegBalDiff          string `json:"begBalDiff"`
	BlockNumber         string `json:"blockNumber"`
	Date                string `json:"date"`
	Decimals            string `json:"decimals"`
	Encoding            string `json:"encoding"`
	EndBal              string `json:"endBal"`
	EndBalCalc          string `json:"endBalCalc"`
	EndBalDiff          string `json:"endBalDiff"`
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
	Reconciled          string `json:"reconciled"`
	ReconciliationType  string `json:"reconciliationType"`
	SelfDestructIn      string `json:"selfDestructIn"`
	SelfDestructOut     string `json:"selfDestructOut"`
	Sender              string `json:"sender"`
	Signature           string `json:"signature"`
	SpotPrice           string `json:"spotPrice"`
	Timestamp           string `json:"timestamp"`
	TotalIn             string `json:"totalIn"`
	TotalOut            string `json:"totalOut"`
	TotalOutLessGas     string `json:"totalOutLessGas"`
	TransactionHash     string `json:"transactionHash"`
	TransactionIndex    string `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleReconciliation struct {
	AccountedFor        base.Address       `json:"accountedFor"`
	AmountIn            big.Int            `json:"amountIn,omitempty"`
	AmountNet           big.Int            `json:"amountNet"`
	AmountOut           big.Int            `json:"amountOut,omitempty"`
	AssetAddr           base.Address       `json:"assetAddr"`
	AssetSymbol         string             `json:"assetSymbol"`
	BegBal              big.Int            `json:"begBal"`
	BegBalDiff          big.Int            `json:"begBalDiff,omitempty"`
	BlockNumber         base.Blknum        `json:"blockNumber"`
	Date                string             `json:"date"`
	Decimals            uint64             `json:"decimals"`
	Encoding            string             `json:"encoding"`
	EndBal              big.Int            `json:"endBal"`
	EndBalCalc          big.Int            `json:"endBalCalc,omitempty"`
	EndBalDiff          big.Int            `json:"endBalDiff,omitempty"`
	GasOut              big.Int            `json:"gasOut,omitempty"`
	InternalIn          big.Int            `json:"internalIn,omitempty"`
	InternalOut         big.Int            `json:"internalOut,omitempty"`
	LogIndex            base.Blknum        `json:"logIndex"`
	MinerBaseRewardIn   big.Int            `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn big.Int            `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        big.Int            `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  big.Int            `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           big.Int            `json:"prefundIn,omitempty"`
	PrevAppBlk          base.Blknum        `json:"prevAppBlk,omitempty"`
	PrevBal             big.Int            `json:"prevBal,omitempty"`
	PriceSource         string             `json:"priceSource"`
	Recipient           base.Address       `json:"recipient"`
	Reconciled          bool               `json:"reconciled"`
	ReconciliationType  string             `json:"reconciliationType"`
	SelfDestructIn      big.Int            `json:"selfDestructIn,omitempty"`
	SelfDestructOut     big.Int            `json:"selfDestructOut,omitempty"`
	Sender              base.Address       `json:"sender"`
	Signature           string             `json:"signature"`
	SpotPrice           float64            `json:"spotPrice"`
	Timestamp           base.Timestamp     `json:"timestamp"`
	TotalIn             big.Int            `json:"totalIn"`
	TotalOut            big.Int            `json:"totalOut"`
	TotalOutLessGas     big.Int            `json:"totalOutLessGas"`
	TransactionHash     base.Hash          `json:"transactionHash"`
	TransactionIndex    base.Blknum        `json:"transactionIndex"`
	raw                 *RawReconciliation `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleReconciliation) Raw() *RawReconciliation {
	return s.raw
}

func (s *SimpleReconciliation) SetRaw(raw *RawReconciliation) {
	s.raw = raw
}

func (s *SimpleReconciliation) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleReconciliation) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleReconciliation) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
