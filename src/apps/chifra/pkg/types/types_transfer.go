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
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type Transfer struct {
	AmountIn            base.Wei     `json:"amountIn,omitempty"`
	AmountOut           base.Wei     `json:"amountOut,omitempty"`
	Asset               base.Address `json:"asset"`
	BlockNumber         base.Blknum  `json:"blockNumber"`
	Decimals            uint64       `json:"decimals"`
	GasOut              base.Wei     `json:"gasOut,omitempty"`
	Holder              base.Address `json:"holder"`
	InternalIn          base.Wei     `json:"internalIn,omitempty"`
	InternalOut         base.Wei     `json:"internalOut,omitempty"`
	LogIndex            base.Lognum  `json:"logIndex"`
	MinerBaseRewardIn   base.Wei     `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn base.Wei     `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        base.Wei     `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  base.Wei     `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           base.Wei     `json:"prefundIn,omitempty"`
	Recipient           base.Address `json:"recipient"`
	SelfDestructIn      base.Wei     `json:"selfDestructIn,omitempty"`
	SelfDestructOut     base.Wei     `json:"selfDestructOut,omitempty"`
	Sender              base.Address `json:"sender"`
	TransactionIndex    base.Txnum   `json:"transactionIndex"`
	// EXISTING_CODE
	Log         *Log         `json:"log,omitempty"`
	Transaction *Transaction `json:"transaction,omitempty"`
	// EXISTING_CODE
}

func (s Transfer) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Transfer) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
		"logIndex":         s.LogIndex,
		"asset":            s.Asset,
		"holder":           s.Holder,
		"amount":           s.AmountNet().Text(10),
	}

	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "asset", "holder", "amount",
	}

	if extraOpts["ether"] == true {
		decimals := int(s.Decimals)
		model["amountEth"] = s.AmountNet().ToFloatString(decimals)
		order = append(order, []string{"amountEth"}...)
	}

	for _, item := range []struct {
		address   base.Address
		keyPrefix string
	}{
		{s.Asset, "asset"},
		{s.Holder, "holder"},
	} {
		key := item.keyPrefix + "Name"
		if result, loaded, found := nameAddress(extraOpts, item.address); found {
			model[key] = result.Name
			order = append(order, key)
		} else if loaded && format != "json" {
			model[key] = ""
			order = append(order, key)
		}
	}

	order = reorderOrdering(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Transfer) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
type AssetTransfer struct {
	Amount            base.Wei `json:"amount"`
	AmountIn          base.Wei
	AmountOut         base.Wei
	Asset             base.Address `json:"asset"`
	BegBal            base.Wei
	BlockNumber       base.Blknum `json:"blockNumber"`
	CorrectingReasons string      `json:"correctingReasons"`
	CorrectionId      base.Value  `json:"correctionId"`
	Decimals          uint64      `json:"decimals"`
	EndBal            base.Wei
	Holder            base.Address `json:"holder"`
	LogIndex          base.Lognum  `json:"logIndex"`
	StatementId       base.Value   `json:"statementId"`
	TransactionIndex  base.Txnum   `json:"transactionIndex"`
}

func (s *AssetTransfer) EndBalCalc() *base.Wei {
	return new(base.Wei).Add(&s.BegBal, s.AmountNet())
}

func (s *AssetTransfer) AmountNet() *base.Wei {
	return new(base.Wei).Sub(&s.AmountIn, &s.AmountOut)
}

func NewAssetTransfer(t Transfer) AssetTransfer {
	return AssetTransfer{
		Amount:           *t.AmountNet(),
		Asset:            t.Asset,
		BlockNumber:      t.BlockNumber,
		Holder:           t.Holder,
		LogIndex:         t.LogIndex,
		TransactionIndex: t.TransactionIndex,
		Decimals:         t.Decimals,
	}
}

func (s *AssetTransfer) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	_, _, _, _ = chain, format, verbose, extraOpts
	check1, check2, reconciles, byCheckpoint := s.Reconciled()
	calc := s.EndBalCalc()
	fmt.Printf("%s\t%s\t%d\t%d\t%d\t%d\t%d\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%t\t%t\n",
		s.Asset.Hex(),
		s.Holder.Hex(),
		s.BlockNumber,
		s.TransactionIndex,
		s.LogIndex,
		s.StatementId,
		s.CorrectionId,
		s.CorrectingReasons,
		s.BegBal.Text(10),
		s.AmountNet().Text(10),
		calc.Text(10),
		s.EndBal.Text(10),
		check1.Text(10),
		check2.Text(10),
		reconciles,
		byCheckpoint,
	)

	return Model{
		Data:  model,
		Order: order,
	}
}

// ---------------------------------------------------------
func (p *AssetTransfer) Reconciled() (base.Wei, base.Wei, bool, bool) {
	calc := p.EndBalCalc()
	checkVal := *new(base.Wei).Add(&p.BegBal, p.AmountNet())
	tentativeDiff := *new(base.Wei).Sub(&checkVal, calc)
	checkpointDiff := *new(base.Wei).Sub(&checkVal, &p.EndBal)

	checkpointEqual := checkVal.Equal(&p.EndBal)
	if checkpointEqual {
		return tentativeDiff, checkpointDiff, true, true
	}

	tentativeEqual := checkVal.Equal(calc)
	return tentativeDiff, checkpointDiff, tentativeEqual, false
}

func (s *Transfer) IsMaterial() bool {
	return !s.TotalIn().IsZero() || !s.TotalOut().IsZero()
}

func (s *Transfer) AmountNet() *base.Wei {
	return new(base.Wei).Sub(s.TotalIn(), s.TotalOut())
}

func (s *Transfer) TotalIn() *base.Wei {
	vals := []base.Wei{
		s.AmountIn,
		s.InternalIn,
		s.MinerBaseRewardIn,
		s.MinerNephewRewardIn,
		s.MinerTxFeeIn,
		s.MinerUncleRewardIn,
		s.PrefundIn,
		s.SelfDestructIn,
	}

	sum := base.NewWei(0)
	for _, n := range vals {
		sum = new(base.Wei).Add(sum, &n)
	}

	return sum
}

func (s *Transfer) TotalOut() *base.Wei {
	vals := []base.Wei{
		s.AmountOut,
		s.InternalOut,
		s.GasOut,
		s.SelfDestructOut,
	}

	sum := base.NewWei(0)
	for _, n := range vals {
		sum = new(base.Wei).Add(sum, &n)
	}

	return sum
}

func (s *Transfer) ToStatement(trans *Transaction, asEther bool) *Statement {
	sym := "WEI"
	if asEther {
		sym = "ETH"
	}
	return &Statement{
		Transaction:         trans,
		Log:                 s.Log,
		BlockNumber:         s.BlockNumber,
		TransactionIndex:    s.TransactionIndex,
		LogIndex:            s.LogIndex,
		Sender:              s.Sender,
		Recipient:           s.Recipient,
		AccountedFor:        s.Holder,
		Asset:               s.Asset,
		Symbol:              sym,
		Decimals:            base.Value(s.Decimals),
		Holder:              s.Holder,
		AmountIn:            s.AmountIn,
		InternalIn:          s.InternalIn,
		MinerBaseRewardIn:   s.MinerBaseRewardIn,
		MinerNephewRewardIn: s.MinerNephewRewardIn,
		MinerTxFeeIn:        s.MinerTxFeeIn,
		MinerUncleRewardIn:  s.MinerUncleRewardIn,
		PrefundIn:           s.PrefundIn,
		SelfDestructIn:      s.SelfDestructIn,
		AmountOut:           s.AmountOut,
		InternalOut:         s.InternalOut,
		GasOut:              s.GasOut,
		SelfDestructOut:     s.SelfDestructOut,
		TransactionHash:     trans.Hash,
		Timestamp:           trans.Timestamp,
		PriceSource:         "not-priced",
	}
}

// EXISTING_CODE
