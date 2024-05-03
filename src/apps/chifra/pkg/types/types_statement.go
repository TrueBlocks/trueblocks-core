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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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
	PrevBal             string `json:"prevBal"`
	PriceSource         string `json:"priceSource"`
	Recipient           string `json:"recipient"`
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

type Statement struct {
	AccountedFor        base.Address   `json:"accountedFor"`
	AmountIn            base.Wei       `json:"amountIn,omitempty"`
	AmountOut           base.Wei       `json:"amountOut,omitempty"`
	AssetAddr           base.Address   `json:"assetAddr"`
	AssetSymbol         string         `json:"assetSymbol"`
	BegBal              base.Wei       `json:"begBal"`
	BlockNumber         base.Blknum    `json:"blockNumber"`
	CorrectingIn        base.Wei       `json:"correctingIn,omitempty"`
	CorrectingOut       base.Wei       `json:"correctingOut,omitempty"`
	CorrectingReason    string         `json:"correctingReason,omitempty"`
	Decimals            uint64         `json:"decimals"`
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
	SpotPrice           float64        `json:"spotPrice"`
	Timestamp           base.Timestamp `json:"timestamp"`
	TransactionHash     base.Hash      `json:"transactionHash"`
	TransactionIndex    base.Txnum     `json:"transactionIndex"`
	raw                 *RawStatement  `json:"-"`
	// EXISTING_CODE
	ReconType ReconType `json:"-"`
	AssetType string    `json:"-"`
	// EXISTING_CODE
}

func (s Statement) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Statement) Raw() *RawStatement {
	return s.raw
}

func (s *Statement) SetRaw(raw *RawStatement) {
	s.raw = raw
}

func (s *Statement) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	asEther := extraOptions["ether"] == true
	decimals := int(s.Decimals)
	model = map[string]any{
		"blockNumber":         s.BlockNumber,
		"transactionIndex":    s.TransactionIndex,
		"logIndex":            s.LogIndex,
		"transactionHash":     s.TransactionHash,
		"timestamp":           s.Timestamp,
		"date":                s.Date(),
		"assetAddr":           s.AssetAddr,
		"assetType":           s.AssetType,
		"assetSymbol":         s.AssetSymbol,
		"decimals":            s.Decimals,
		"spotPrice":           s.SpotPrice,
		"priceSource":         s.PriceSource,
		"accountedFor":        s.AccountedFor,
		"sender":              s.Sender,
		"recipient":           s.Recipient,
		"begBal":              base.FormattedValue(&s.BegBal, asEther, decimals),
		"amountNet":           base.FormattedValue(s.AmountNet(), asEther, decimals),
		"endBal":              base.FormattedValue(&s.EndBal, asEther, decimals),
		"reconciliationType":  s.ReconType.String(),
		"reconciled":          s.Reconciled(),
		"totalIn":             base.FormattedValue(s.TotalIn(), asEther, decimals),
		"amountIn":            base.FormattedValue(&s.AmountIn, asEther, decimals),
		"internalIn":          base.FormattedValue(&s.InternalIn, asEther, decimals),
		"selfDestructIn":      base.FormattedValue(&s.SelfDestructIn, asEther, decimals),
		"minerBaseRewardIn":   base.FormattedValue(&s.MinerBaseRewardIn, asEther, decimals),
		"minerNephewRewardIn": base.FormattedValue(&s.MinerNephewRewardIn, asEther, decimals),
		"minerTxFeeIn":        base.FormattedValue(&s.MinerTxFeeIn, asEther, decimals),
		"minerUncleRewardIn":  base.FormattedValue(&s.MinerUncleRewardIn, asEther, decimals),
		"correctingIn":        base.FormattedValue(&s.CorrectingIn, asEther, decimals),
		"prefundIn":           base.FormattedValue(&s.PrefundIn, asEther, decimals),
		"totalOut":            base.FormattedValue(s.TotalOut(), asEther, decimals),
		"amountOut":           base.FormattedValue(&s.AmountOut, asEther, decimals),
		"internalOut":         base.FormattedValue(&s.InternalOut, asEther, decimals),
		"correctingOut":       base.FormattedValue(&s.CorrectingOut, asEther, decimals),
		"selfDestructOut":     base.FormattedValue(&s.SelfDestructOut, asEther, decimals),
		"gasOut":              base.FormattedValue(&s.GasOut, asEther, decimals),
		"totalOutLessGas":     base.FormattedValue(s.TotalOutLessGas(), asEther, decimals),
		"begBalDiff":          base.FormattedValue(s.BegBalDiff(), asEther, decimals),
		"endBalDiff":          base.FormattedValue(s.EndBalDiff(), asEther, decimals),
		"endBalCalc":          base.FormattedValue(s.EndBalCalc(), asEther, decimals),
		"correctingReason":    s.CorrectingReason,
	}

	if s.ReconType&First == 0 {
		model["prevBal"] = base.FormattedValue(&s.PrevBal, asEther, decimals)
	} else if format != "json" {
		model["prevBal"] = ""
	}

	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "transactionHash", "timestamp", "date",
		"assetAddr", "assetType", "assetSymbol", "decimals", "spotPrice", "priceSource", "accountedFor",
		"sender", "recipient", "begBal", "amountNet", "endBal", "reconciliationType", "reconciled",
		"totalIn", "amountIn", "internalIn", "selfDestructIn", "minerBaseRewardIn", "minerNephewRewardIn",
		"minerTxFeeIn", "minerUncleRewardIn", "prefundIn", "totalOut", "amountOut", "internalOut",
		"selfDestructOut", "gasOut", "totalOutLessGas", "prevBal", "begBalDiff",
		"endBalDiff", "endBalCalc", "correctingReason",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Statement) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

type StatementGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Address          base.Address
	Statements       []Statement
}

func (s *StatementGroup) CacheName() string {
	return "Statement"
}

func (s *StatementGroup) CacheId() string {
	return fmt.Sprintf("%s-%09d-%05d", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex)
}

func (s *StatementGroup) CacheLocation() (directory string, extension string) {
	paddedId := s.CacheId()
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]

	subFolder := strings.ToLower(s.CacheName()) + "s"
	directory = filepath.Join(subFolder, filepath.Join(parts...))
	extension = "bin"

	return
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

	// AssetAddr
	if err = cache.WriteValue(writer, s.AssetAddr); err != nil {
		return err
	}

	// AssetSymbol
	if err = cache.WriteValue(writer, s.AssetSymbol); err != nil {
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

	// AssetAddr
	if err = cache.ReadValue(reader, &s.AssetAddr, vers); err != nil {
		return err
	}

	// AssetSymbol
	if err = cache.ReadValue(reader, &s.AssetSymbol, vers); err != nil {
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
		new(base.Wei).Sub(&s.BegBal, &s.PrevBal)
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
	return s.AssetAddr == base.FAKE_ETH_ADDRESS
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
	return stables[s.AssetAddr]
}

func (s *Statement) isNullTransfer(tx *Transaction) bool {
	lotsOfLogs := len(tx.Receipt.Logs) > 10
	mayBeAirdrop := s.Sender.IsZero() || s.Sender == tx.To
	noBalanceChange := s.EndBal.Cmp(&s.BegBal) == 0 && s.IsMaterial()
	ret := (lotsOfLogs || mayBeAirdrop) && noBalanceChange

	// TODO: BOGUS PERF
	// logger.Warn("Statement is not reconciled", s.AssetSymbol, "at", s.BlockNumber, s.TransactionIndex, s.LogIndex)
	logger.TestLog(true, "A possible nullTransfer")
	logger.TestLog(true, "  nLogs:            ", len(tx.Receipt.Logs))
	logger.TestLog(true, "    lotsOfLogs:      -->", lotsOfLogs)

	logger.TestLog(true, "  Sender.IsZero:    ", s.Sender, s.Sender.IsZero())
	logger.TestLog(true, "  or Sender == To:  ", s.Sender == tx.To)
	logger.TestLog(true, "    mayBeAirdrop:    -->", mayBeAirdrop)

	logger.TestLog(true, "  EndBal-BegBal:    ", s.EndBal.Cmp(&s.BegBal))
	logger.TestLog(true, "  material:         ", s.IsMaterial())
	logger.TestLog(true, "    noBalanceChange: -->", noBalanceChange)

	if !ret {
		logger.TestLog(true, "  ---> Not a nullTransfer")
	}
	return ret
}

func (s *Statement) CorrectForNullTransfer(tx *Transaction) bool {
	if !s.IsEth() {
		if s.isNullTransfer(tx) {
			logger.TestLog(true, "Correcting token transfer for a null transfer")
			amt := s.TotalIn() // use totalIn since this is the amount that was faked
			s.AmountOut = *new(base.Wei)
			s.AmountIn = *new(base.Wei)
			s.CorrectingIn = *amt
			s.CorrectingOut = *amt
			s.CorrectingReason = "null-transfer"
		} else {
			logger.TestLog(true, "Needs correction for token transfer")
		}
	} else {
		logger.TestLog(true, "Needs correction for eth")
	}
	return s.Reconciled()
}

func (s *Statement) CorrectForSomethingElse(tx *Transaction) bool {
	if s.IsEth() {
		if s.AssetType == "trace-eth" && s.ReconType&First != 0 && s.ReconType&Last != 0 {
			if s.EndBalCalc().Cmp(&s.EndBal) != 0 {
				s.EndBal = *s.EndBalCalc()
				s.CorrectingReason = "per-block-balance"
			}
		} else {
			logger.TestLog(true, "Needs correction for eth")
		}
	} else {
		logger.TestLog(true, "Correcting token transfer for unknown income or outflow")

		s.CorrectingIn.SetUint64(0)
		s.CorrectingOut.SetUint64(0)
		s.CorrectingReason = ""
		zero := new(base.Wei).SetInt64(0)
		cmpBegBal := s.BegBalDiff().Cmp(zero)
		cmpEndBal := s.EndBalDiff().Cmp(zero)

		if cmpBegBal > 0 {
			s.CorrectingIn = *s.BegBalDiff()
			s.CorrectingReason = "begbal"
		} else if cmpBegBal < 0 {
			s.CorrectingOut = *s.BegBalDiff()
			s.CorrectingReason = "begbal"
		}

		if cmpEndBal > 0 {
			n := new(base.Wei).Add(&s.CorrectingIn, s.EndBalDiff())
			s.CorrectingIn = *n
			s.CorrectingReason += "endbal"
		} else if cmpEndBal < 0 {
			n := new(base.Wei).Add(&s.CorrectingOut, s.EndBalDiff())
			s.CorrectingOut = *n
			s.CorrectingReason += "endbal"
		}
		s.CorrectingReason = strings.Replace(s.CorrectingReason, "begbalendbal", "begbal-endbal", -1)
	}

	return s.Reconciled()
}

type Ledgerer interface {
	Prev() base.Blknum
	Cur() base.Blknum
	Next() base.Blknum
}

func (s *Statement) DebugStatement(ctx Ledgerer) {
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, fmt.Sprintf("====> %s", s.AssetType))
	logger.TestLog(true, "===================================================")
	logger.TestLog(true, "Previous:              ", ctx.Prev())
	logger.TestLog(true, "Current:               ", s.BlockNumber)
	logger.TestLog(true, "Next:                  ", ctx.Next())
	logger.TestLog(true, "reconciliationType:    ", s.ReconType.String())
	logger.TestLog(true, "assetType:             ", s.AssetType)
	logger.TestLog(true, "accountedFor:          ", s.AccountedFor)
	logger.TestLog(true, "sender:                ", s.Sender, " ==> ", s.Recipient)
	logger.TestLog(true, "assetAddr:             ", s.AssetAddr, "("+s.AssetSymbol+")", fmt.Sprintf("decimals: %d", s.Decimals))
	logger.TestLog(true, "hash:                  ", s.TransactionHash)
	logger.TestLog(true, "timestamp:             ", s.Timestamp)
	if s.AssetType == "eth" {
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

func isZero(val *base.Wei) bool {
	return val.Cmp(base.NewWei(0)) == 0
}

func reportE(msg string, val *base.Wei) {
	logger.TestLog(!isZero(val), msg, base.FormattedValue(val, true, 18))
}

func report2(msg string, v1 *base.Wei, v2 *base.Wei) {
	s := ""
	if v1 != nil {
		s = base.FormattedValue(v1, true, 18)
	}
	if v2 != nil {
		s += " (" + base.FormattedValue(v2, true, 18) + ")"
	}
	logger.TestLog(true, msg, s)
}

func reportL(msg string) {
	report2(msg, nil, nil)
}

func report1(msg string, val *base.Wei) {
	report2(msg, val, nil)
}

// EXISTING_CODE
