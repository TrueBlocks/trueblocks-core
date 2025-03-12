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
	Amount           base.Wei     `json:"amount"`
	Asset            base.Address `json:"asset"`
	BlockNumber      base.Blknum  `json:"blockNumber"`
	Holder           base.Address `json:"holder"`
	LogIndex         base.Lognum  `json:"logIndex"`
	TransactionIndex base.Txnum   `json:"transactionIndex"`
	// EXISTING_CODE
	Decimals         base.Value `json:"decimals"`
	CorrectingReason string     `json:"correctingReason"`
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
		"amount":           s.Amount.Text(10),
	}

	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "asset", "holder", "amount",
	}

	if extraOpts["ether"] == true {
		decimals := int(s.Decimals)
		model["amountEth"] = s.Amount.ToEtherStr(decimals)
		order = append(order, []string{"amountEth"}...)
	}
	if asset, loaded, found := nameAddress(extraOpts, s.Asset); found {
		model["assetName"] = asset.Name
		order = append(order, "assetName")
	} else if loaded && format != "json" {
		model["assetName"] = ""
		order = append(order, "assetName")
	}
	if holder, loaded, found := nameAddress(extraOpts, s.Holder); found {
		model["holderName"] = holder.Name
		order = append(order, "holderName")
	} else if loaded && format != "json" {
		model["holderName"] = ""
		order = append(order, "holderName")
	}
	order = reorderOrdering(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *TransferGroup) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Transfer") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

type TransferGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Transfers        []Transfer
}

func (s *TransferGroup) MarshalCache(writer io.Writer) (err error) {
	return base.WriteValue(writer, s.Transfers)
}

func (s *TransferGroup) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	return base.ReadValue(reader, &s.Transfers, fileVersion)
}

func (s *Transfer) MarshalCache(writer io.Writer) (err error) {
	// Amount
	if err = base.WriteValue(writer, &s.Amount); err != nil {
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

	// Holder
	if err = base.WriteValue(writer, s.Holder); err != nil {
		return err
	}

	// LogIndex
	if err = base.WriteValue(writer, s.LogIndex); err != nil {
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

	// Amount
	if err = base.ReadValue(reader, &s.Amount, fileVersion); err != nil {
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

	// Holder
	if err = base.ReadValue(reader, &s.Holder, fileVersion); err != nil {
		return err
	}

	// LogIndex
	if err = base.ReadValue(reader, &s.LogIndex, fileVersion); err != nil {
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
	Amount           base.Wei `json:"amount"`
	AmountIn         base.Wei
	AmountOut        base.Wei
	Asset            base.Address `json:"asset"`
	BegBal           base.Wei
	BlockNumber      base.Blknum `json:"blockNumber"`
	CorrectingReason string      `json:"correctingReason"`
	CorrectionId     base.Value  `json:"correctionId"`
	Decimals         base.Value  `json:"decimals"`
	EndBal           base.Wei
	Holder           base.Address `json:"holder"`
	LogIndex         base.Lognum  `json:"logIndex"`
	StatementId      base.Value   `json:"statementId"`
	TransactionIndex base.Txnum   `json:"transactionIndex"`
}

func (s *AssetTransfer) EndBalCalc() *base.Wei {
	return new(base.Wei).Add(&s.BegBal, s.AmountNet())
}

func (s *AssetTransfer) AmountNet() *base.Wei {
	return new(base.Wei).Sub(&s.AmountIn, &s.AmountOut)
}

func NewAssetTransfer(t Transfer) AssetTransfer {
	return AssetTransfer{
		Amount:           t.Amount,
		Asset:            t.Asset,
		BlockNumber:      t.BlockNumber,
		Holder:           t.Holder,
		LogIndex:         t.LogIndex,
		TransactionIndex: t.TransactionIndex,
		Decimals:         t.Decimals,
		CorrectingReason: t.CorrectingReason,
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
		s.CorrectingReason,
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

// EXISTING_CODE
