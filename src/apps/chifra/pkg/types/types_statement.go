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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// EXISTING_CODE

type Statement struct {
	AccountedFor        base.Address   `json:"accountedFor"`
	AmountIn            base.Wei       `json:"amountIn,omitempty"`
	AmountOut           base.Wei       `json:"amountOut,omitempty"`
	Asset               base.Address   `json:"asset"`
	BegBal              base.Wei       `json:"begBal"`
	BlockNumber         base.Blknum    `json:"blockNumber"`
	CorrectAmountIn     base.Wei       `json:"correctAmountIn,omitempty"`
	CorrectAmountOut    base.Wei       `json:"correctAmountOut,omitempty"`
	CorrectBegBalIn     base.Wei       `json:"correctBegBalIn,omitempty"`
	CorrectBegBalOut    base.Wei       `json:"correctBegBalOut,omitempty"`
	CorrectEndBalIn     base.Wei       `json:"correctEndBalIn,omitempty"`
	CorrectEndBalOut    base.Wei       `json:"correctEndBalOut,omitempty"`
	CorrectingReasons   string         `json:"correctingReasons,omitempty"`
	Decimals            base.Value     `json:"decimals"`
	EndBal              base.Wei       `json:"endBal"`
	GasOut              base.Wei       `json:"gasOut,omitempty"`
	InternalIn          base.Wei       `json:"internalIn,omitempty"`
	InternalOut         base.Wei       `json:"internalOut,omitempty"`
	LogIndex            base.Lognum    `json:"logIndex"`
	MinerBaseRewardIn   base.Wei       `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn base.Wei       `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        base.Wei       `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  base.Wei       `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           base.Wei       `json:"prefundIn,omitempty"`
	PrevBal             base.Wei       `json:"prevBal,omitempty"`
	PriceSource         string         `json:"priceSource"`
	Recipient           base.Address   `json:"recipient"`
	SelfDestructIn      base.Wei       `json:"selfDestructIn,omitempty"`
	SelfDestructOut     base.Wei       `json:"selfDestructOut,omitempty"`
	Sender              base.Address   `json:"sender"`
	SpotPrice           base.Float     `json:"spotPrice"`
	Symbol              string         `json:"symbol"`
	Timestamp           base.Timestamp `json:"timestamp"`
	TransactionHash     base.Hash      `json:"transactionHash"`
	TransactionIndex    base.Txnum     `json:"transactionIndex"`
	// EXISTING_CODE
	CorrectionId base.Value   `json:"correctionId"`
	Holder       base.Address `json:"holder"`
	StatementId  base.Value   `json:"statementId"`
	BegSentinel  bool         `json:"-"`
	EndSentinel  bool         `json:"-"`
	Transaction  *Transaction `json:"-"`
	Log          *Log         `json:"-"`
	// EXISTING_CODE
}

func (s Statement) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Statement) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"accountedFor":        s.AccountedFor,
		"amountIn":            s.AmountIn.Text(10),
		"amountNet":           s.AmountNet().Text(10),
		"amountOut":           s.AmountOut.Text(10),
		"asset":               s.Asset,
		"begBal":              s.BegBal.Text(10),
		"begBalDiff":          s.BegBalDiff().Text(10),
		"blockNumber":         s.BlockNumber,
		"correctAmountIn":     s.CorrectAmountIn.Text(10),
		"correctAmountOut":    s.CorrectAmountOut.Text(10),
		"correctBegBalIn":     s.CorrectBegBalIn.Text(10),
		"correctBegBalOut":    s.CorrectBegBalOut.Text(10),
		"correctEndBalIn":     s.CorrectEndBalIn.Text(10),
		"correctEndBalOut":    s.CorrectEndBalOut.Text(10),
		"correctingReasons":   strings.Trim(s.CorrectingReasons, ","),
		"date":                s.Date(),
		"decimals":            s.Decimals,
		"endBal":              s.EndBal.Text(10),
		"endBalCalc":          s.EndBalCalc().Text(10),
		"endBalDiff":          s.EndBalDiff().Text(10),
		"gasOut":              s.GasOut.Text(10),
		"internalIn":          s.InternalIn.Text(10),
		"internalOut":         s.InternalOut.Text(10),
		"logIndex":            s.LogIndex,
		"minerBaseRewardIn":   s.MinerBaseRewardIn.Text(10),
		"minerNephewRewardIn": s.MinerNephewRewardIn.Text(10),
		"minerTxFeeIn":        s.MinerTxFeeIn.Text(10),
		"minerUncleRewardIn":  s.MinerUncleRewardIn.Text(10),
		"prefundIn":           s.PrefundIn.Text(10),
		"prevBal":             s.PrevBal.Text(10),
		"priceSource":         s.PriceSource,
		"recipient":           s.Recipient,
		"reconciled":          s.Reconciled(),
		"selfDestructIn":      s.SelfDestructIn.Text(10),
		"selfDestructOut":     s.SelfDestructOut.Text(10),
		"sender":              s.Sender,
		"spotPrice":           s.SpotPrice.String(),
		"symbol":              s.Symbol,
		"timestamp":           s.Timestamp,
		"totalIn":             s.TotalIn().Text(10),
		"totalOut":            s.TotalOut().Text(10),
		"transactionHash":     s.TransactionHash,
		"transactionIndex":    s.TransactionIndex,
	}

	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "transactionHash", "timestamp", "date",
		"asset", "symbol", "decimals", "spotPrice", "priceSource", "accountedFor",
		"sender", "recipient", "begBal", "amountNet", "endBal", "reconciled",
		"totalIn", "amountIn", "internalIn", "correctBegBalIn", "correctAmountIn", "correctEndBalIn",
		"selfDestructIn", "minerBaseRewardIn", "minerNephewRewardIn",
		"minerTxFeeIn", "minerUncleRewardIn", "prefundIn", "totalOut", "amountOut", "internalOut",
		"correctBegBalOut", "correctAmountOut", "correctEndBalOut", "selfDestructOut",
		"gasOut", "prevBal", "begBalDiff", "endBalDiff", "endBalCalc", "correctingReasons",
	}

	if extraOpts["ether"] == true {
		decimals := int(s.Decimals)
		model["amountInEth"] = s.AmountIn.ToFloatString(decimals)
		model["amountNetEth"] = s.AmountNet().ToFloatString(decimals)
		model["amountOutEth"] = s.AmountOut.ToFloatString(decimals)
		model["begBalDiffEth"] = s.BegBalDiff().ToFloatString(decimals)
		model["begBalEth"] = s.BegBal.ToFloatString(decimals)
		model["correctAmountInEth"] = s.CorrectAmountIn.ToFloatString(decimals)
		model["correctAmountOutEth"] = s.CorrectAmountOut.ToFloatString(decimals)
		model["correctBegBalInEth"] = s.CorrectBegBalIn.ToFloatString(decimals)
		model["correctBegBalOutEth"] = s.CorrectBegBalOut.ToFloatString(decimals)
		model["correctEndBalInEth"] = s.CorrectEndBalIn.ToFloatString(decimals)
		model["correctEndBalOutEth"] = s.CorrectEndBalOut.ToFloatString(decimals)
		model["endBalCalcEth"] = s.EndBalCalc().ToFloatString(decimals)
		model["endBalDiffEth"] = s.EndBalDiff().ToFloatString(decimals)
		model["endBalEth"] = s.EndBal.ToFloatString(decimals)
		model["gasOutEth"] = s.GasOut.ToFloatString(decimals)
		model["internalInEth"] = s.InternalIn.ToFloatString(decimals)
		model["internalOutEth"] = s.InternalOut.ToFloatString(decimals)
		model["minerBaseRewardInEth"] = s.MinerBaseRewardIn.ToFloatString(decimals)
		model["minerNephewRewardInEth"] = s.MinerNephewRewardIn.ToFloatString(decimals)
		model["minerTxFeeInEth"] = s.MinerTxFeeIn.ToFloatString(decimals)
		model["minerUncleRewardInEth"] = s.MinerUncleRewardIn.ToFloatString(decimals)
		model["prefundInEth"] = s.PrefundIn.ToFloatString(decimals)
		model["prevBalEth"] = s.PrevBal.ToFloatString(decimals)
		model["selfDestructInEth"] = s.SelfDestructIn.ToFloatString(decimals)
		model["selfDestructOutEth"] = s.SelfDestructOut.ToFloatString(decimals)
		model["totalInEth"] = s.TotalIn().ToFloatString(decimals)
		model["totalOutEth"] = s.TotalOut().ToFloatString(decimals)
		order = append(order, []string{"begBalEth", "amountNetEth", "endBalEth",
			"totalInEth", "amountInEth", "internalInEth", "selfDestructInEth",
			"minerBaseRewardInEth", "minerNephewRewardInEth", "minerTxFeeInEth",
			"minerUncleRewardInEth", "correctBegBalInEth", "correctAmountInEth", "correctEndBalInEth",
			"prefundInEth", "totalOutEth", "amountOutEth", "internalOutEth", "correctBegBalOutEth",
			"correctAmountOutEth", "correctEndBalOutEth", "selfDestructOutEth", "gasOutEth",
			"begBalDiffEth", "endBalDiffEth", "endBalCalcEth", "prevBalEth"}...)
	}

	for _, item := range []struct {
		address   base.Address
		keyPrefix string
	}{
		{s.Asset, "asset"},
		{s.AccountedFor, "accountedFor"},
		{s.Sender, "sender"},
		{s.Recipient, "recipient"},
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

func (s *Statement) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *StatementGroup) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%s-%s-%09d-%05d", s.Holder.Hex()[2:], s.Asset.Hex()[2:], s.BlockNumber, s.TransactionIndex)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Statement") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

type StatementGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Holder           base.Address
	Asset            base.Address
	Statements       []Statement
}

func (s *StatementGroup) MarshalCache(writer io.Writer) (err error) {
	return base.WriteValue(writer, s.Statements)
}

func (s *StatementGroup) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	return base.ReadValue(reader, &s.Statements, fileVersion)
}

func (s *Statement) MarshalCache(writer io.Writer) (err error) {
	// AccountedFor
	if err = base.WriteValue(writer, s.AccountedFor); err != nil {
		return err
	}

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

	// BegBal
	if err = base.WriteValue(writer, &s.BegBal); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// CorrectAmountIn
	if err = base.WriteValue(writer, &s.CorrectAmountIn); err != nil {
		return err
	}

	// CorrectAmountOut
	if err = base.WriteValue(writer, &s.CorrectAmountOut); err != nil {
		return err
	}

	// CorrectBegBalIn
	if err = base.WriteValue(writer, &s.CorrectBegBalIn); err != nil {
		return err
	}

	// CorrectBegBalOut
	if err = base.WriteValue(writer, &s.CorrectBegBalOut); err != nil {
		return err
	}

	// CorrectEndBalIn
	if err = base.WriteValue(writer, &s.CorrectEndBalIn); err != nil {
		return err
	}

	// CorrectEndBalOut
	if err = base.WriteValue(writer, &s.CorrectEndBalOut); err != nil {
		return err
	}

	// CorrectingReasons
	if err = base.WriteValue(writer, s.CorrectingReasons); err != nil {
		return err
	}

	// Decimals
	if err = base.WriteValue(writer, s.Decimals); err != nil {
		return err
	}

	// EndBal
	if err = base.WriteValue(writer, &s.EndBal); err != nil {
		return err
	}

	// GasOut
	if err = base.WriteValue(writer, &s.GasOut); err != nil {
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

	// PrevBal
	if err = base.WriteValue(writer, &s.PrevBal); err != nil {
		return err
	}

	// PriceSource
	if err = base.WriteValue(writer, s.PriceSource); err != nil {
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

	// SpotPrice
	if err = base.WriteValue(writer, &s.SpotPrice); err != nil {
		return err
	}

	// Symbol
	if err = base.WriteValue(writer, s.Symbol); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// TransactionHash
	if err = base.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	return nil
}

func (s *Statement) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// AccountedFor
	if err = base.ReadValue(reader, &s.AccountedFor, fileVersion); err != nil {
		return err
	}

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

	// BegBal
	if err = base.ReadValue(reader, &s.BegBal, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// CorrectAmountIn
	if err = base.ReadValue(reader, &s.CorrectAmountIn, fileVersion); err != nil {
		return err
	}

	// CorrectAmountOut
	if err = base.ReadValue(reader, &s.CorrectAmountOut, fileVersion); err != nil {
		return err
	}

	// CorrectBegBalIn
	if err = base.ReadValue(reader, &s.CorrectBegBalIn, fileVersion); err != nil {
		return err
	}

	// CorrectBegBalOut
	if err = base.ReadValue(reader, &s.CorrectBegBalOut, fileVersion); err != nil {
		return err
	}

	// CorrectEndBalIn
	if err = base.ReadValue(reader, &s.CorrectEndBalIn, fileVersion); err != nil {
		return err
	}

	// CorrectEndBalOut
	if err = base.ReadValue(reader, &s.CorrectEndBalOut, fileVersion); err != nil {
		return err
	}

	// CorrectingReasons
	if err = base.ReadValue(reader, &s.CorrectingReasons, fileVersion); err != nil {
		return err
	}

	// Decimals
	if err = base.ReadValue(reader, &s.Decimals, fileVersion); err != nil {
		return err
	}

	// EndBal
	if err = base.ReadValue(reader, &s.EndBal, fileVersion); err != nil {
		return err
	}

	// GasOut
	if err = base.ReadValue(reader, &s.GasOut, fileVersion); err != nil {
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

	// PrevBal
	if err = base.ReadValue(reader, &s.PrevBal, fileVersion); err != nil {
		return err
	}

	// PriceSource
	if err = base.ReadValue(reader, &s.PriceSource, fileVersion); err != nil {
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

	// SpotPrice
	if err = base.ReadValue(reader, &s.SpotPrice, fileVersion); err != nil {
		return err
	}

	// Symbol
	if err = base.ReadValue(reader, &s.Symbol, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// TransactionHash
	if err = base.ReadValue(reader, &s.TransactionHash, fileVersion); err != nil {
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
func (s *Statement) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

var (
	sai  = base.HexToAddress("0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359")
	dai  = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
	usdc = base.HexToAddress("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48")
	usdt = base.HexToAddress("0xdac17f958d2ee523a2206206994597c13d831ec7")
)

func (s *Statement) IsMaterial() bool {
	return !s.TotalIn().IsZero() || !s.TotalOut().IsZero()
}

func (s *Statement) IsEth() bool {
	return s.Asset == base.FAKE_ETH_ADDRESS
}

func (s *Statement) IsStableCoin() bool {
	stables := map[base.Address]bool{
		sai:  true,
		dai:  true,
		usdc: true,
		usdt: true,
	}
	return stables[s.Asset]
}

func (stmt *Statement) CorrectBeginBalance() bool {
	if stmt.BegBalDiff().Equal(base.ZeroWei) {
		return stmt.Reconciled()
	}

	diff := stmt.BegBalDiff()
	absDiff := diff.Abs()
	logger.TestLog(true, "Correcting beginning balance", diff.Text(10), "absDiff:", absDiff.Text(10))
	if stmt.BegBalDiff().LessThan(base.ZeroWei) { // begBal < expected, increase totalIn
		stmt.CorrectingReasons += "begBalIn,"
		val := new(base.Wei).Add(&stmt.CorrectBegBalIn, absDiff)
		stmt.CorrectBegBalIn = *val
	} else if stmt.BegBalDiff().GreaterThan(base.ZeroWei) { // begBal > expected, increase totalOut
		stmt.CorrectingReasons += "begBalOut,"
		val := new(base.Wei).Add(&stmt.CorrectBegBalOut, absDiff)
		stmt.CorrectBegBalOut = *val
	}

	return stmt.Reconciled()
}

func (stmt *Statement) CorrectEndBalance() bool {
	if stmt.EndBalDiff().Equal(base.ZeroWei) {
		return stmt.Reconciled()
	}

	diff := stmt.EndBalDiff()
	absDiff := diff.Abs()
	logger.TestLog(true, "Correcting ending balance", "diff:", diff.Text(10), "absDiff:", absDiff.Text(10))
	if stmt.EndBalDiff().GreaterThan(base.ZeroWei) { // endBal > endBalCalc, reduce totalOut
		stmt.CorrectingReasons += "endBalOut,"
		val := new(base.Wei).Add(&stmt.CorrectEndBalOut, absDiff)
		stmt.CorrectEndBalOut = *val
		logger.TestLog(true, "correctEndBalOut:", stmt.CorrectEndBalOut.Text(10))
	} else if stmt.EndBalDiff().LessThan(base.ZeroWei) { // endBal < endBalCalc, increase totalIn
		stmt.CorrectingReasons += "endBalIn,"
		val := new(base.Wei).Add(&stmt.CorrectEndBalIn, absDiff)
		stmt.CorrectEndBalIn = *val
		logger.TestLog(true, "correctEndBalIn:", stmt.CorrectEndBalIn.Text(10))
	}

	return stmt.Reconciled()
}

func (s *Statement) AmountNet() *base.Wei {
	return new(base.Wei).Sub(s.TotalIn(), s.TotalOut())
}

func (s *Statement) BegBalDiff() *base.Wei {
	val := &base.Wei{}

	if s.BlockNumber == 0 {
		val = new(base.Wei).SetInt64(0)
	} else {
		val = new(base.Wei).Sub(&s.BegBal, &s.PrevBal)
	}

	return val
}

func (s *Statement) EndBalCalc() *base.Wei {
	return new(base.Wei).Add(&s.BegBal, s.AmountNet())
}

func (s *Statement) EndBalDiff() *base.Wei {
	return new(base.Wei).Sub(s.EndBalCalc(), &s.EndBal)
}

func (s *Statement) Reconciled() bool {
	zero := new(base.Wei).SetInt64(0)
	return (s.EndBalDiff().Equal(zero) && s.BegBalDiff().Equal(zero))
}

func (s *Statement) TotalIn() *base.Wei {
	vals := []base.Wei{
		s.AmountIn,
		s.InternalIn,
		s.SelfDestructIn,
		s.MinerBaseRewardIn,
		s.MinerNephewRewardIn,
		s.MinerTxFeeIn,
		s.MinerUncleRewardIn,
		s.CorrectBegBalIn,
		s.CorrectAmountIn,
		s.CorrectEndBalIn,
		s.PrefundIn,
	}

	sum := base.NewWei(0)
	for _, n := range vals {
		sum = new(base.Wei).Add(sum, &n)
	}

	return sum
}

func (s *Statement) TotalOut() *base.Wei {
	vals := []base.Wei{
		s.AmountOut,
		s.InternalOut,
		s.CorrectBegBalOut,
		s.CorrectAmountOut,
		s.CorrectEndBalOut,
		s.SelfDestructOut,
		s.GasOut,
	}

	sum := base.NewWei(0)
	for _, n := range vals {
		sum = new(base.Wei).Add(sum, &n)
	}

	return sum
}

func (s *Statement) TotalOutLessGas() *base.Wei {
	totalOut := s.TotalOut()
	return totalOut.Sub(totalOut, &s.GasOut)
}

func (s Statement) Report() string {
	return fmt.Sprintf("%s,%s,%s,%s,%s,%s,%s\n",
		s.CorrectAmountIn.Text(10), s.CorrectAmountOut.Text(10), s.CorrectBegBalIn.Text(10),
		s.CorrectBegBalOut.Text(10), s.CorrectEndBalIn.Text(10), s.CorrectEndBalOut.Text(10),
		s.CorrectingReasons)
}

func (s *Statement) Type() string {
	if s.Asset == base.FAKE_ETH_ADDRESS {
		return "eth"
	}
	return "token"
}

func (s *Statement) DateTime() base.DateTime {
	return base.NewDateTimeTs(s.Timestamp)
}

func (s *Statement) Encoding() string {
	if s.Transaction == nil || len(s.Transaction.Input) < 10 {
		return "0x"
	}
	return s.Transaction.Input[:10]
}

func (s *Statement) Signature() string {
	if s.Transaction == nil || s.Transaction.ArticulatedTx == nil {
		return s.Encoding()
	}
	return s.Transaction.ArticulatedTx.Signature
}

func (s *Statement) ReconciliationType() string {
	// if s.Reconciled() {
	// 	return "ok"
	// }
	return "reconciliationType"
}

// EXISTING_CODE
