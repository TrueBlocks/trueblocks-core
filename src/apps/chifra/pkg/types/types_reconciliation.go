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
	AmountIn            base.BigInt        `json:"amountIn,omitempty"`
	AmountNet           base.BigInt        `json:"amountNet"`
	AmountOut           base.BigInt        `json:"amountOut,omitempty"`
	AssetAddr           base.Address       `json:"assetAddr"`
	AssetSymbol         string             `json:"assetSymbol"`
	BegBal              base.BigInt        `json:"begBal"`
	BegBalDiff          base.BigInt        `json:"begBalDiff,omitempty"`
	BlockNumber         base.Blknum        `json:"blockNumber"`
	Date                string             `json:"date"`
	Decimals            uint64             `json:"decimals"`
	Encoding            string             `json:"encoding"`
	EndBal              base.BigInt        `json:"endBal"`
	EndBalCalc          base.BigInt        `json:"endBalCalc,omitempty"`
	EndBalDiff          base.BigInt        `json:"endBalDiff,omitempty"`
	GasOut              base.BigInt        `json:"gasOut,omitempty"`
	InternalIn          base.BigInt        `json:"internalIn,omitempty"`
	InternalOut         base.BigInt        `json:"internalOut,omitempty"`
	LogIndex            base.Blknum        `json:"logIndex"`
	MinerBaseRewardIn   base.BigInt        `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn base.BigInt        `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        base.BigInt        `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  base.BigInt        `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           base.BigInt        `json:"prefundIn,omitempty"`
	PrevAppBlk          base.Blknum        `json:"prevAppBlk,omitempty"`
	PrevBal             base.BigInt        `json:"prevBal,omitempty"`
	PriceSource         string             `json:"priceSource"`
	Recipient           base.Address       `json:"recipient"`
	Reconciled          bool               `json:"reconciled"`
	ReconciliationType  string             `json:"reconciliationType"`
	SelfDestructIn      base.BigInt        `json:"selfDestructIn,omitempty"`
	SelfDestructOut     base.BigInt        `json:"selfDestructOut,omitempty"`
	Sender              base.Address       `json:"sender"`
	Signature           string             `json:"signature"`
	SpotPrice           float64            `json:"spotPrice"`
	Timestamp           base.Timestamp     `json:"timestamp"`
	TotalIn             base.BigInt        `json:"totalIn"`
	TotalOut            base.BigInt        `json:"totalOut"`
	TotalOutLessGas     base.BigInt        `json:"totalOutLessGas"`
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
	model = map[string]any{
		"blockNumber":         s.BlockNumber,
		"transactionIndex":    s.TransactionIndex,
		"logIndex":            s.LogIndex,
		"transactionHash":     s.TransactionHash,
		"timestamp":           s.Timestamp,
		"date":                s.Date,
		"assetAddr":           s.AssetAddr,
		"assetSymbol":         s.AssetSymbol,
		"decimals":            s.Decimals,
		"spotPrice":           s.SpotPrice,
		"priceSource":         s.PriceSource,
		"accountedFor":        s.AccountedFor,
		"sender":              s.Sender,
		"recipient":           s.Recipient,
		"begBal":              s.BegBal.String(),
		"amountNet":           s.AmountNet.String(),
		"endBal":              s.EndBal.String(),
		"reconciliationType":  s.ReconciliationType,
		"reconciled":          s.Reconciled,
		"totalIn":             s.TotalIn.String(),
		"amountIn":            s.AmountIn.String(),
		"internalIn":          s.InternalIn.String(),
		"selfDestructIn":      s.SelfDestructIn.String(),
		"minerBaseRewardIn":   s.MinerBaseRewardIn.String(),
		"minerNephewRewardIn": s.MinerNephewRewardIn.String(),
		"minerTxFeeIn":        s.MinerTxFeeIn.String(),
		"minerUncleRewardIn":  s.MinerUncleRewardIn.String(),
		"prefundIn":           s.PrefundIn.String(),
		"totalOut":            s.TotalOut.String(),
		"amountOut":           s.AmountOut.String(),
		"internalOut":         s.InternalOut.String(),
		"selfDestructOut":     s.SelfDestructOut.String(),
		"gasOut":              s.GasOut.String(),
		"totalOutLessGas":     s.TotalOutLessGas.String(),
		"prevAppBlk":          s.PrevAppBlk,
		"prevBal":             s.PrevBal,
		"begBalDiff":          s.BegBalDiff.String(),
		"endBalDiff":          s.EndBalDiff.String(),
		"endBalCalc":          s.EndBalCalc.String(),
		// ENCODING
		// Encoding            string             `json:"encoding"`
		// SIGNATURE
		// Signature           string             `json:"signature"`
	}
	order = []string{
		"blockNumber", "transactionIndex", "logindex", "transactionHash", "timestamp", "date",
		"assetAddress", "assetSymbol", "decimals", "spotPrice", "priceSource", "accountedFor",
		"sender", "recipient", "begBal", "amountNet", "endBal", "reconciliationType", "reconciled",
		"totalIn", "amountIn", "internalIn", "selfDestructIn", "minerBaseRewardIn", "minerNephewRewardIn",
		"minerTxFeeIn", "minerUncleRewardIn", "prefundIn", "totalOut", "amountOut", "internalOut",
		"selfDestructOut", "gasOut", "totalOutLessGas", "prevAppBlk", "prevBal", "begBalDiff", "endBalDiff", "endBalCalc",
	}
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
