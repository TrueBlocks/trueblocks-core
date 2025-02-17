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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

// EXISTING_CODE

type Statement struct {
	AccountedFor        base.Address   `json:"accountedFor"`
	AmountIn            base.Wei       `json:"amountIn,omitempty"`
	AmountOut           base.Wei       `json:"amountOut,omitempty"`
	AssetAddress        base.Address   `json:"assetAddress"`
	AssetSymbol         string         `json:"assetSymbol"`
	AssetType           TrialBalType   `json:"assetType,omitempty"`
	BegBal              base.Wei       `json:"begBal"`
	BlockNumber         base.Blknum    `json:"blockNumber"`
	BlockNumberNext     base.Blknum    `json:"blockNumberNext"`
	BlockNumberPrev     base.Blknum    `json:"blockNumberPrev"`
	CorrectingIn        base.Wei       `json:"correctingIn,omitempty"`
	CorrectingOut       base.Wei       `json:"correctingOut,omitempty"`
	CorrectingReason    string         `json:"correctingReason,omitempty"`
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
	PostFirst           bool           `json:"postFirst"`
	PostLast            bool           `json:"postLast"`
	PrefundIn           base.Wei       `json:"prefundIn,omitempty"`
	PrevBal             base.Wei       `json:"prevBal,omitempty"`
	PriceSource         string         `json:"priceSource"`
	Recipient           base.Address   `json:"recipient"`
	RollingBalance      base.Wei       `json:"rollingBalance,omitempty"`
	SelfDestructIn      base.Wei       `json:"selfDestructIn,omitempty"`
	SelfDestructOut     base.Wei       `json:"selfDestructOut,omitempty"`
	Sender              base.Address   `json:"sender"`
	SpotPrice           base.Float     `json:"spotPrice"`
	Timestamp           base.Timestamp `json:"timestamp"`
	TransactionHash     base.Hash      `json:"transactionHash"`
	TransactionIndex    base.Txnum     `json:"transactionIndex"`
	// EXISTING_CODE
	PostType PostType `json:"postType"`
	// EXISTING_CODE
}

func (s Statement) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Statement) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"blockNumber":         s.BlockNumber,
		"transactionIndex":    s.TransactionIndex,
		"logIndex":            s.LogIndex,
		"transactionHash":     s.TransactionHash,
		"timestamp":           s.Timestamp,
		"date":                s.Date(),
		"assetAddress":        s.AssetAddress,
		"assetType":           s.AssetType.String(),
		"assetSymbol":         s.AssetSymbol,
		"decimals":            s.Decimals,
		"spotPrice":           s.SpotPrice,
		"priceSource":         s.PriceSource,
		"accountedFor":        s.AccountedFor,
		"sender":              s.Sender,
		"recipient":           s.Recipient,
		"reconciled":          s.Reconciled(),
		"correctingReason":    s.CorrectingReason,
		"begBal":              s.BegBal.Text(10),
		"amountNet":           s.AmountNet().Text(10),
		"endBal":              s.EndBal.Text(10),
		"totalIn":             s.TotalIn().Text(10),
		"amountIn":            s.AmountIn.Text(10),
		"internalIn":          s.InternalIn.Text(10),
		"selfDestructIn":      s.SelfDestructIn.Text(10),
		"minerBaseRewardIn":   s.MinerBaseRewardIn.Text(10),
		"minerNephewRewardIn": s.MinerNephewRewardIn.Text(10),
		"minerTxFeeIn":        s.MinerTxFeeIn.Text(10),
		"minerUncleRewardIn":  s.MinerUncleRewardIn.Text(10),
		"correctingIn":        s.CorrectingIn.Text(10),
		"prefundIn":           s.PrefundIn.Text(10),
		"totalOut":            s.TotalOut().Text(10),
		"amountOut":           s.AmountOut.Text(10),
		"internalOut":         s.InternalOut.Text(10),
		"correctingOut":       s.CorrectingOut.Text(10),
		"selfDestructOut":     s.SelfDestructOut.Text(10),
		"gasOut":              s.GasOut.Text(10),
		"totalOutLessGas":     s.TotalOutLessGas().Text(10),
		"begBalDiff":          s.BegBalDiff().Text(10),
		"endBalDiff":          s.EndBalDiff().Text(10),
		"endBalCalc":          s.EndBalCalc().Text(10),
		"prevBal":             s.PrevBal.Text(10),
	}

	if extraOpts["testMode"] == true {
		model["postType"] = s.getPostType().String()
		model["postFirst"] = s.PostFirst
		model["postLast"] = s.PostLast
	}

	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "transactionHash", "timestamp", "date",
		"assetAddress", "assetType", "assetSymbol", "decimals", "spotPrice", "priceSource", "accountedFor",
		"sender", "recipient", "begBal", "amountNet", "endBal", "reconciled",
		"totalIn", "amountIn", "internalIn", "selfDestructIn", "minerBaseRewardIn", "minerNephewRewardIn",
		"minerTxFeeIn", "minerUncleRewardIn", "prefundIn", "totalOut", "amountOut", "internalOut",
		"selfDestructOut", "gasOut", "totalOutLessGas", "prevBal", "begBalDiff",
		"endBalDiff", "endBalCalc", "correctingReason",
	}

	if extraOpts["ether"] == true {
		decimals := int(s.Decimals)
		model["begBalEth"] = s.BegBal.ToEtherStr(decimals)
		model["amountNetEth"] = s.AmountNet().ToEtherStr(decimals)
		model["endBalEth"] = s.EndBal.ToEtherStr(decimals)
		model["totalInEth"] = s.TotalIn().ToEtherStr(decimals)
		model["amountInEth"] = s.AmountIn.ToEtherStr(decimals)
		model["internalInEth"] = s.InternalIn.ToEtherStr(decimals)
		model["selfDestructInEth"] = s.SelfDestructIn.ToEtherStr(decimals)
		model["minerBaseRewardInEth"] = s.MinerBaseRewardIn.ToEtherStr(decimals)
		model["minerNephewRewardInEth"] = s.MinerNephewRewardIn.ToEtherStr(decimals)
		model["minerTxFeeInEth"] = s.MinerTxFeeIn.ToEtherStr(decimals)
		model["minerUncleRewardInEth"] = s.MinerUncleRewardIn.ToEtherStr(decimals)
		model["correctingInEth"] = s.CorrectingIn.ToEtherStr(decimals)
		model["prefundInEth"] = s.PrefundIn.ToEtherStr(decimals)
		model["totalOutEth"] = s.TotalOut().ToEtherStr(decimals)
		model["amountOutEth"] = s.AmountOut.ToEtherStr(decimals)
		model["internalOutEth"] = s.InternalOut.ToEtherStr(decimals)
		model["correctingOutEth"] = s.CorrectingOut.ToEtherStr(decimals)
		model["selfDestructOutEth"] = s.SelfDestructOut.ToEtherStr(decimals)
		model["gasOutEth"] = s.GasOut.ToEtherStr(decimals)
		model["totalOutLessGasEth"] = s.TotalOutLessGas().ToEtherStr(decimals)
		model["begBalDiffEth"] = s.BegBalDiff().ToEtherStr(decimals)
		model["endBalDiffEth"] = s.EndBalDiff().ToEtherStr(decimals)
		model["endBalCalcEth"] = s.EndBalCalc().ToEtherStr(decimals)
		model["prevBalEth"] = s.PrevBal.ToEtherStr(decimals)
		order = append(order, []string{"begBalEth", "amountNetEth", "endBalEth",
			"totalInEth", "amountInEth", "internalInEth", "selfDestructInEth",
			"minerBaseRewardInEth", "minerNephewRewardInEth", "minerTxFeeInEth",
			"minerUncleRewardInEth", "correctingInEth", "prefundInEth",
			"totalOutEth", "amountOutEth", "internalOutEth", "correctingOutEth",
			"selfDestructOutEth", "gasOutEth", "totalOutLessGasEth", "begBalDiffEth",
			"endBalDiffEth", "endBalCalcEth", "prevBalEth"}...)
	}
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
	paddedId := fmt.Sprintf("%s-%09d-%05d", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex)
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
	Address          base.Address
	Statements       []Statement
}

func (s *StatementGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Statements)
}

func (s *StatementGroup) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Statements, vers)
}

func (s *Statement) MarshalCache(writer io.Writer) (err error) {
	// AccountedFor
	if err = cache.WriteValue(writer, s.AccountedFor); err != nil {
		return err
	}

	// AmountIn
	if err = cache.WriteValue(writer, &s.AmountIn); err != nil {
		return err
	}

	// AmountOut
	if err = cache.WriteValue(writer, &s.AmountOut); err != nil {
		return err
	}

	// AssetAddress
	if err = cache.WriteValue(writer, s.AssetAddress); err != nil {
		return err
	}

	// AssetSymbol
	if err = cache.WriteValue(writer, s.AssetSymbol); err != nil {
		return err
	}

	// AssetType
	if err = cache.WriteValue(writer, s.AssetType); err != nil {
		return err
	}

	// BegBal
	if err = cache.WriteValue(writer, &s.BegBal); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// BlockNumberNext
	if err = cache.WriteValue(writer, s.BlockNumberNext); err != nil {
		return err
	}

	// BlockNumberPrev
	if err = cache.WriteValue(writer, s.BlockNumberPrev); err != nil {
		return err
	}

	// CorrectingIn
	if err = cache.WriteValue(writer, &s.CorrectingIn); err != nil {
		return err
	}

	// CorrectingOut
	if err = cache.WriteValue(writer, &s.CorrectingOut); err != nil {
		return err
	}

	// CorrectingReason
	if err = cache.WriteValue(writer, s.CorrectingReason); err != nil {
		return err
	}

	// Decimals
	if err = cache.WriteValue(writer, s.Decimals); err != nil {
		return err
	}

	// EndBal
	if err = cache.WriteValue(writer, &s.EndBal); err != nil {
		return err
	}

	// GasOut
	if err = cache.WriteValue(writer, &s.GasOut); err != nil {
		return err
	}

	// InternalIn
	if err = cache.WriteValue(writer, &s.InternalIn); err != nil {
		return err
	}

	// InternalOut
	if err = cache.WriteValue(writer, &s.InternalOut); err != nil {
		return err
	}

	// LogIndex
	if err = cache.WriteValue(writer, s.LogIndex); err != nil {
		return err
	}

	// MinerBaseRewardIn
	if err = cache.WriteValue(writer, &s.MinerBaseRewardIn); err != nil {
		return err
	}

	// MinerNephewRewardIn
	if err = cache.WriteValue(writer, &s.MinerNephewRewardIn); err != nil {
		return err
	}

	// MinerTxFeeIn
	if err = cache.WriteValue(writer, &s.MinerTxFeeIn); err != nil {
		return err
	}

	// MinerUncleRewardIn
	if err = cache.WriteValue(writer, &s.MinerUncleRewardIn); err != nil {
		return err
	}

	// PostFirst
	if err = cache.WriteValue(writer, s.PostFirst); err != nil {
		return err
	}

	// PostLast
	if err = cache.WriteValue(writer, s.PostLast); err != nil {
		return err
	}

	// PrefundIn
	if err = cache.WriteValue(writer, &s.PrefundIn); err != nil {
		return err
	}

	// PrevBal
	if err = cache.WriteValue(writer, &s.PrevBal); err != nil {
		return err
	}

	// PriceSource
	if err = cache.WriteValue(writer, s.PriceSource); err != nil {
		return err
	}

	// Recipient
	if err = cache.WriteValue(writer, s.Recipient); err != nil {
		return err
	}

	// RollingBalance
	if err = cache.WriteValue(writer, &s.RollingBalance); err != nil {
		return err
	}

	// SelfDestructIn
	if err = cache.WriteValue(writer, &s.SelfDestructIn); err != nil {
		return err
	}

	// SelfDestructOut
	if err = cache.WriteValue(writer, &s.SelfDestructOut); err != nil {
		return err
	}

	// Sender
	if err = cache.WriteValue(writer, s.Sender); err != nil {
		return err
	}

	// SpotPrice
	if err = cache.WriteValue(writer, s.SpotPrice); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	return nil
}

func (s *Statement) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// AccountedFor
	if err = cache.ReadValue(reader, &s.AccountedFor, vers); err != nil {
		return err
	}

	// AmountIn
	if err = cache.ReadValue(reader, &s.AmountIn, vers); err != nil {
		return err
	}

	// AmountOut
	if err = cache.ReadValue(reader, &s.AmountOut, vers); err != nil {
		return err
	}

	// AssetAddress
	if err = cache.ReadValue(reader, &s.AssetAddress, vers); err != nil {
		return err
	}

	// AssetSymbol
	if err = cache.ReadValue(reader, &s.AssetSymbol, vers); err != nil {
		return err
	}

	// AssetType
	if err = cache.ReadValue(reader, &s.AssetType, vers); err != nil {
		return err
	}

	// BegBal
	if err = cache.ReadValue(reader, &s.BegBal, vers); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// BlockNumberNext
	if err = cache.ReadValue(reader, &s.BlockNumberNext, vers); err != nil {
		return err
	}

	// BlockNumberPrev
	if err = cache.ReadValue(reader, &s.BlockNumberPrev, vers); err != nil {
		return err
	}

	// CorrectingIn
	if err = cache.ReadValue(reader, &s.CorrectingIn, vers); err != nil {
		return err
	}

	// CorrectingOut
	if err = cache.ReadValue(reader, &s.CorrectingOut, vers); err != nil {
		return err
	}

	// CorrectingReason
	if err = cache.ReadValue(reader, &s.CorrectingReason, vers); err != nil {
		return err
	}

	// Decimals
	if err = cache.ReadValue(reader, &s.Decimals, vers); err != nil {
		return err
	}

	// EndBal
	if err = cache.ReadValue(reader, &s.EndBal, vers); err != nil {
		return err
	}

	// GasOut
	if err = cache.ReadValue(reader, &s.GasOut, vers); err != nil {
		return err
	}

	// InternalIn
	if err = cache.ReadValue(reader, &s.InternalIn, vers); err != nil {
		return err
	}

	// InternalOut
	if err = cache.ReadValue(reader, &s.InternalOut, vers); err != nil {
		return err
	}

	// LogIndex
	if err = cache.ReadValue(reader, &s.LogIndex, vers); err != nil {
		return err
	}

	// MinerBaseRewardIn
	if err = cache.ReadValue(reader, &s.MinerBaseRewardIn, vers); err != nil {
		return err
	}

	// MinerNephewRewardIn
	if err = cache.ReadValue(reader, &s.MinerNephewRewardIn, vers); err != nil {
		return err
	}

	// MinerTxFeeIn
	if err = cache.ReadValue(reader, &s.MinerTxFeeIn, vers); err != nil {
		return err
	}

	// MinerUncleRewardIn
	if err = cache.ReadValue(reader, &s.MinerUncleRewardIn, vers); err != nil {
		return err
	}

	// PostFirst
	if err = cache.ReadValue(reader, &s.PostFirst, vers); err != nil {
		return err
	}

	// PostLast
	if err = cache.ReadValue(reader, &s.PostLast, vers); err != nil {
		return err
	}

	// PrefundIn
	if err = cache.ReadValue(reader, &s.PrefundIn, vers); err != nil {
		return err
	}

	// PrevBal
	if err = cache.ReadValue(reader, &s.PrevBal, vers); err != nil {
		return err
	}

	// PriceSource
	if err = cache.ReadValue(reader, &s.PriceSource, vers); err != nil {
		return err
	}

	// Recipient
	if err = cache.ReadValue(reader, &s.Recipient, vers); err != nil {
		return err
	}

	// RollingBalance
	if err = cache.ReadValue(reader, &s.RollingBalance, vers); err != nil {
		return err
	}

	// SelfDestructIn
	if err = cache.ReadValue(reader, &s.SelfDestructIn, vers); err != nil {
		return err
	}

	// SelfDestructOut
	if err = cache.ReadValue(reader, &s.SelfDestructOut, vers); err != nil {
		return err
	}

	// Sender
	if err = cache.ReadValue(reader, &s.Sender, vers); err != nil {
		return err
	}

	// SpotPrice
	if err = cache.ReadValue(reader, &s.SpotPrice, vers); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, vers); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.ReadValue(reader, &s.TransactionHash, vers); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.ReadValue(reader, &s.TransactionIndex, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Statement) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *Statement) TotalIn() *base.Wei {
	vals := []base.Wei{
		s.AmountIn,
		s.InternalIn,
		s.SelfDestructIn,
		s.MinerBaseRewardIn,
		s.MinerNephewRewardIn,
		s.MinerTxFeeIn,
		s.MinerUncleRewardIn,
		s.CorrectingIn,
		s.PrefundIn,
	}

	sum := base.NewWei(0)
	for _, n := range vals {
		sum = sum.Add(sum, &n)
	}

	return sum
}

func (s *Statement) TotalOut() *base.Wei {
	vals := []base.Wei{
		s.AmountOut,
		s.InternalOut,
		s.CorrectingOut,
		s.SelfDestructOut,
		s.GasOut,
	}

	sum := base.NewWei(0)
	for _, n := range vals {
		sum = sum.Add(sum, &n)
	}

	return sum
}

func (s *Statement) IsMaterial() bool {
	return s.TotalIn().Cmp(new(base.Wei)) != 0 || s.TotalOut().Cmp(new(base.Wei)) != 0
}

func (s *Statement) AmountNet() *base.Wei {
	return new(base.Wei).Sub(s.TotalIn(), s.TotalOut())
}

func (s *Statement) TotalOutLessGas() *base.Wei {
	val := s.TotalOut()
	return new(base.Wei).Sub(val, &s.GasOut)
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
	return (s.EndBalDiff().Cmp(zero) == 0 && s.BegBalDiff().Cmp(zero) == 0)
}

func (s *Statement) IsEth() bool {
	return s.AssetAddress == base.FAKE_ETH_ADDRESS
}

var (
	sai  = base.HexToAddress("0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359")
	dai  = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
	usdc = base.HexToAddress("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48")
	usdt = base.HexToAddress("0xdac17f958d2ee523a2206206994597c13d831ec7")
)

func (s *Statement) IsStableCoin() bool {
	stables := map[base.Address]bool{
		sai:  true,
		dai:  true,
		usdc: true,
		usdt: true,
	}
	return stables[s.AssetAddress]
}

func (s *Statement) DebugStatement(prev, next base.Blknum) {
	reportE := func(msg string, val *base.Wei) {
		isZero := func(val *base.Wei) bool {
			return val.Cmp(base.NewWei(0)) == 0
		}
		logger.TestLog(!isZero(val), msg, val.ToEtherStr(18))
	}

	report2 := func(msg string, v1 *base.Wei, v2 *base.Wei) {
		s := ""
		if v1 != nil {
			s = v1.ToEtherStr(18)
		}
		if v2 != nil {
			s += " (" + v2.ToEtherStr(18) + ")"
		}
		logger.TestLog(true, msg, s)
	}

	reportL := func(msg string) {
		report2(msg, nil, nil)
	}

	report1 := func(msg string, val *base.Wei) {
		report2(msg, val, nil)
	}

	logger.TestLog(true, "===================================================")
	logger.TestLog(true, fmt.Sprintf("====> %s", s.AssetType))
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, "Previous:              ", prev)
	logger.TestLog(true, "Current:               ", s.BlockNumber)
	logger.TestLog(true, "Next:                  ", next)
	logger.TestLog(true, "postType:              ", s.getPostType().String())
	logger.TestLog(true, "postFirst:             ", s.PostFirst)
	logger.TestLog(true, "postLast:              ", s.PostLast)
	logger.TestLog(true, "assetType:             ", s.AssetType)
	logger.TestLog(true, "accountedFor:          ", s.AccountedFor)
	logger.TestLog(true, "sender:                ", s.Sender, " ==> ", s.Recipient)
	logger.TestLog(true, "assetAddress:          ", s.AssetAddress, "("+s.AssetSymbol+")", fmt.Sprintf("decimals: %d", s.Decimals))
	logger.TestLog(true, "hash:                  ", s.TransactionHash)
	logger.TestLog(true, "timestamp:             ", s.Timestamp)
	if s.AssetType != TrialBalToken && s.AssetType != TrialBalNft {
		logger.TestLog(true, fmt.Sprintf("blockNumber:            %d.%d", s.BlockNumber, s.TransactionIndex))
	} else {
		logger.TestLog(true, fmt.Sprintf("blockNumber:            %d.%d.%d", s.BlockNumber, s.TransactionIndex, s.LogIndex))
	}
	logger.TestLog(true, "priceSource:           ", s.SpotPrice, "("+s.PriceSource+")")
	reportL("---------------------------------------------------")
	logger.TestLog(true, "Trial balance:")
	report1("   prevBal:            ", &s.PrevBal)
	report2("   begBal:             ", &s.BegBal, s.EndBalDiff())
	report1("   totalIn:            ", s.TotalIn())
	report1("   totalOut:           ", s.TotalOut())
	report1("   amountNet:          ", s.AmountNet())
	reportL("                       =======================")
	report2("   endBal:             ", &s.EndBal, s.BegBalDiff())
	report1("   endBalCalc:         ", s.EndBalCalc())
	report1("   rollingBalance:     ", &s.RollingBalance)
	reportL("---------------------------------------------------")
	reportE("   amountIn:           ", &s.AmountIn)
	reportE("   internalIn:         ", &s.InternalIn)
	reportE("   minerBaseRewardIn:  ", &s.MinerBaseRewardIn)
	reportE("   minerNephewRewardIn:", &s.MinerNephewRewardIn)
	reportE("   minerTxFeeIn:       ", &s.MinerTxFeeIn)
	reportE("   minerUncleRewardIn: ", &s.MinerUncleRewardIn)
	reportE("   correctingIn:       ", &s.CorrectingIn)
	reportE("   prefundIn:          ", &s.PrefundIn)
	reportE("   selfDestructIn:     ", &s.SelfDestructIn)
	reportE("   amountOut:          ", &s.AmountOut)
	reportE("   internalOut:        ", &s.InternalOut)
	reportE("   correctingOut:      ", &s.CorrectingOut)
	reportE("   selfDestructOut:    ", &s.SelfDestructOut)
	reportE("   gasOut:             ", &s.GasOut)
	logger.TestLog(s.CorrectingReason != "", "   correctingReason:   ", s.CorrectingReason)
	logger.TestLog(true, "   material:           ", s.IsMaterial())
	logger.TestLog(true, "   reconciled:         ", s.Reconciled())
	if !s.Reconciled() {
		logger.TestLog(true, " ^^ we need to fix this ^^")
	}
	logger.TestLog(true, "---------------------------------------------------")
	logger.TestLog(true, "End of trial balance report")
}

func (s *Statement) getPostType() PostType {
	if s.BlockNumber == 0 {
		return Genesis
	}
	prevDiff := s.BlockNumberPrev != s.BlockNumber
	nextDiff := s.BlockNumberNext != s.BlockNumber
	if prevDiff {
		if nextDiff {
			return DiffDiff
		} else {
			return DiffSame
		}
	} else {
		if nextDiff {
			return SameDiff
		} else {
			return SameSame
		}
	}
}

// EXISTING_CODE
