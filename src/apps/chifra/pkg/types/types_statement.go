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
	CorrectingReasons   []string       `json:"correctingReasons,omitempty"`
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
		"correctingReasons":   strings.Join(s.CorrectingReasons, "-"),
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
		"spotPrice":           s.SpotPrice,
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
		model["amountInEth"] = s.AmountIn.ToEtherStr(decimals)
		model["amountNetEth"] = s.AmountNet().ToEtherStr(decimals)
		model["amountOutEth"] = s.AmountOut.ToEtherStr(decimals)
		model["begBalDiffEth"] = s.BegBalDiff().ToEtherStr(decimals)
		model["begBalEth"] = s.BegBal.ToEtherStr(decimals)
		model["correctAmountInEth"] = s.CorrectAmountIn.ToEtherStr(decimals)
		model["correctAmountOutEth"] = s.CorrectAmountOut.ToEtherStr(decimals)
		model["correctBegBalInEth"] = s.CorrectBegBalIn.ToEtherStr(decimals)
		model["correctBegBalOutEth"] = s.CorrectBegBalOut.ToEtherStr(decimals)
		model["correctEndBalInEth"] = s.CorrectEndBalIn.ToEtherStr(decimals)
		model["correctEndBalOutEth"] = s.CorrectEndBalOut.ToEtherStr(decimals)
		model["endBalCalcEth"] = s.EndBalCalc().ToEtherStr(decimals)
		model["endBalDiffEth"] = s.EndBalDiff().ToEtherStr(decimals)
		model["endBalEth"] = s.EndBal.ToEtherStr(decimals)
		model["gasOutEth"] = s.GasOut.ToEtherStr(decimals)
		model["internalInEth"] = s.InternalIn.ToEtherStr(decimals)
		model["internalOutEth"] = s.InternalOut.ToEtherStr(decimals)
		model["minerBaseRewardInEth"] = s.MinerBaseRewardIn.ToEtherStr(decimals)
		model["minerNephewRewardInEth"] = s.MinerNephewRewardIn.ToEtherStr(decimals)
		model["minerTxFeeInEth"] = s.MinerTxFeeIn.ToEtherStr(decimals)
		model["minerUncleRewardInEth"] = s.MinerUncleRewardIn.ToEtherStr(decimals)
		model["prefundInEth"] = s.PrefundIn.ToEtherStr(decimals)
		model["prevBalEth"] = s.PrevBal.ToEtherStr(decimals)
		model["selfDestructInEth"] = s.SelfDestructIn.ToEtherStr(decimals)
		model["selfDestructOutEth"] = s.SelfDestructOut.ToEtherStr(decimals)
		model["totalInEth"] = s.TotalIn().ToEtherStr(decimals)
		model["totalOutEth"] = s.TotalOut().ToEtherStr(decimals)
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
	return s.TotalIn().Cmp(new(base.Wei)) != 0 || s.TotalOut().Cmp(new(base.Wei)) != 0
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
		stmt.CorrectingReasons = append(stmt.CorrectingReasons, "begBalIn")
		val := new(base.Wei).Add(&stmt.CorrectBegBalIn, absDiff)
		stmt.CorrectBegBalIn = *val
	} else if stmt.BegBalDiff().GreaterThan(base.ZeroWei) { // begBal > expected, increase totalOut
		stmt.CorrectingReasons = append(stmt.CorrectingReasons, "begBalOut")
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
		stmt.CorrectingReasons = append(stmt.CorrectingReasons, "endBalOut")
		val := new(base.Wei).Add(&stmt.CorrectEndBalOut, absDiff)
		stmt.CorrectEndBalOut = *val
		logger.TestLog(true, "correctEndBalOut:", stmt.CorrectEndBalOut.Text(10))
	} else if stmt.EndBalDiff().LessThan(base.ZeroWei) { // endBal < endBalCalc, increase totalIn
		stmt.CorrectingReasons = append(stmt.CorrectingReasons, "endBalIn")
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

func (s Statement) Report() string {
	return fmt.Sprintf("%s,%s,%s,%s,%s,%s,%s\n",
		s.CorrectAmountIn.Text(10), s.CorrectAmountOut.Text(10), s.CorrectBegBalIn.Text(10),
		s.CorrectBegBalOut.Text(10), s.CorrectEndBalIn.Text(10), s.CorrectEndBalOut.Text(10),
		strings.Join(s.CorrectingReasons, ","))
}

// EXISTING_CODE
