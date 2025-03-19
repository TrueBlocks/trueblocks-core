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
	"io"
	"path/filepath"
	"strings"

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
		model["amountEth"] = s.AmountNet().ToEtherStr(decimals)
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

func (s *Transfer) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Transfer") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *Transfer) MarshalCache(writer io.Writer) (err error) {
	// AmountIn
	if err = base.WriteValue(writer, &s.AmountIn); err != nil {
		return err
	}

	// AmountOut
	if err = base.WriteValue(writer, &s.AmountOut); err != nil {
		return err
	}

	// Asset
	if err = base.WriteValue(writer, s.Asset); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Decimals
	if err = base.WriteValue(writer, s.Decimals); err != nil {
		return err
	}

	// GasOut
	if err = base.WriteValue(writer, &s.GasOut); err != nil {
		return err
	}

	// Holder
	if err = base.WriteValue(writer, s.Holder); err != nil {
		return err
	}

	// InternalIn
	if err = base.WriteValue(writer, &s.InternalIn); err != nil {
		return err
	}

	// InternalOut
	if err = base.WriteValue(writer, &s.InternalOut); err != nil {
		return err
	}

	// LogIndex
	if err = base.WriteValue(writer, s.LogIndex); err != nil {
		return err
	}

	// MinerBaseRewardIn
	if err = base.WriteValue(writer, &s.MinerBaseRewardIn); err != nil {
		return err
	}

	// MinerNephewRewardIn
	if err = base.WriteValue(writer, &s.MinerNephewRewardIn); err != nil {
		return err
	}

	// MinerTxFeeIn
	if err = base.WriteValue(writer, &s.MinerTxFeeIn); err != nil {
		return err
	}

	// MinerUncleRewardIn
	if err = base.WriteValue(writer, &s.MinerUncleRewardIn); err != nil {
		return err
	}

	// PrefundIn
	if err = base.WriteValue(writer, &s.PrefundIn); err != nil {
		return err
	}

	// Recipient
	if err = base.WriteValue(writer, s.Recipient); err != nil {
		return err
	}

	// SelfDestructIn
	if err = base.WriteValue(writer, &s.SelfDestructIn); err != nil {
		return err
	}

	// SelfDestructOut
	if err = base.WriteValue(writer, &s.SelfDestructOut); err != nil {
		return err
	}

	// Sender
	if err = base.WriteValue(writer, s.Sender); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	return nil
}

func (s *Transfer) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// AmountIn
	if err = base.ReadValue(reader, &s.AmountIn, fileVersion); err != nil {
		return err
	}

	// AmountOut
	if err = base.ReadValue(reader, &s.AmountOut, fileVersion); err != nil {
		return err
	}

	// Asset
	if err = base.ReadValue(reader, &s.Asset, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// Decimals
	if err = base.ReadValue(reader, &s.Decimals, fileVersion); err != nil {
		return err
	}

	// GasOut
	if err = base.ReadValue(reader, &s.GasOut, fileVersion); err != nil {
		return err
	}

	// Holder
	if err = base.ReadValue(reader, &s.Holder, fileVersion); err != nil {
		return err
	}

	// InternalIn
	if err = base.ReadValue(reader, &s.InternalIn, fileVersion); err != nil {
		return err
	}

	// InternalOut
	if err = base.ReadValue(reader, &s.InternalOut, fileVersion); err != nil {
		return err
	}

	// LogIndex
	if err = base.ReadValue(reader, &s.LogIndex, fileVersion); err != nil {
		return err
	}

	// MinerBaseRewardIn
	if err = base.ReadValue(reader, &s.MinerBaseRewardIn, fileVersion); err != nil {
		return err
	}

	// MinerNephewRewardIn
	if err = base.ReadValue(reader, &s.MinerNephewRewardIn, fileVersion); err != nil {
		return err
	}

	// MinerTxFeeIn
	if err = base.ReadValue(reader, &s.MinerTxFeeIn, fileVersion); err != nil {
		return err
	}

	// MinerUncleRewardIn
	if err = base.ReadValue(reader, &s.MinerUncleRewardIn, fileVersion); err != nil {
		return err
	}

	// PrefundIn
	if err = base.ReadValue(reader, &s.PrefundIn, fileVersion); err != nil {
		return err
	}

	// Recipient
	if err = base.ReadValue(reader, &s.Recipient, fileVersion); err != nil {
		return err
	}

	// SelfDestructIn
	if err = base.ReadValue(reader, &s.SelfDestructIn, fileVersion); err != nil {
		return err
	}

	// SelfDestructOut
	if err = base.ReadValue(reader, &s.SelfDestructOut, fileVersion); err != nil {
		return err
	}

	// Sender
	if err = base.ReadValue(reader, &s.Sender, fileVersion); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.ReadValue(reader, &s.TransactionIndex, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
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
	CorrectingReasons []string    `json:"correctingReasons"`
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
		strings.Join(s.CorrectingReasons, "-"),
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

// EXISTING_CODE
