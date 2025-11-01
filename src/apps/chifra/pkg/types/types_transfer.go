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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
)

// EXISTING_CODE

type Transfer struct {
	AmountIn            base.Wei       `json:"amountIn,omitempty"`
	AmountOut           base.Wei       `json:"amountOut,omitempty"`
	Asset               base.Address   `json:"asset"`
	BlockNumber         base.Blknum    `json:"blockNumber"`
	Decimals            uint64         `json:"decimals"`
	GasOut              base.Wei       `json:"gasOut,omitempty"`
	Holder              base.Address   `json:"holder"`
	InternalIn          base.Wei       `json:"internalIn,omitempty"`
	InternalOut         base.Wei       `json:"internalOut,omitempty"`
	LogIndex            base.Lognum    `json:"logIndex"`
	MinerBaseRewardIn   base.Wei       `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn base.Wei       `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        base.Wei       `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  base.Wei       `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           base.Wei       `json:"prefundIn,omitempty"`
	Recipient           base.Address   `json:"recipient"`
	SelfDestructIn      base.Wei       `json:"selfDestructIn,omitempty"`
	SelfDestructOut     base.Wei       `json:"selfDestructOut,omitempty"`
	Sender              base.Address   `json:"sender"`
	TransactionIndex    base.Txnum     `json:"transactionIndex"`
	Calcs               *TransferCalcs `json:"calcs,omitempty"`
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
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Asset, "asset"),
		NewLabeler(s.Holder, "holder"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "asset", "holder", "amount",
	}

	if extraOpts["ether"] == true {
		order = append(order, "amountEth")
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

// RawMap returns a map containing only the raw/base fields for this Transfer.
func (s *Transfer) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"asset":            s.Asset,
		"blockNumber":      s.BlockNumber,
		"holder":           s.Holder,
		"logIndex":         s.LogIndex,
		"transactionIndex": s.TransactionIndex,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Transfer) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		"amount": s.AmountNet().Text(10),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.ExtraOpts["ether"] == true {
		decimals := int(s.Decimals)
		model["amountEth"] = s.AmountNet().ToFloatString(decimals)
	}
	// EXISTING_CODE

	return model
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Transfer) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// TransferCalcs holds lazy-loaded calculated fields for Transfer
type TransferCalcs struct {
	// EXISTING_CODE
	Amount    base.Wei `json:"amount"`
	AmountEth string   `json:"amountEth,omitempty"`
	// EXISTING_CODE
}

func (s *Transfer) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
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

	s.Calcs = &TransferCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
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
