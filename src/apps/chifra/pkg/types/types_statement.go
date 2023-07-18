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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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
	PrevAppBlk          string `json:"prevAppBlk"`
	PrevBal             string `json:"prevBal"`
	PriceSource         string `json:"priceSource"`
	Recipient           string `json:"recipient"`
	ReconciliationType  string `json:"reconciliationType"`
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

type SimpleStatement struct {
	AccountedFor        base.Address   `json:"accountedFor"`
	AmountIn            big.Int        `json:"amountIn,omitempty"`
	AmountOut           big.Int        `json:"amountOut,omitempty"`
	AssetAddr           base.Address   `json:"assetAddr"`
	AssetSymbol         string         `json:"assetSymbol"`
	BegBal              big.Int        `json:"begBal"`
	BlockNumber         base.Blknum    `json:"blockNumber"`
	CorrectingIn        big.Int        `json:"correctingIn,omitempty"`
	CorrectingOut       big.Int        `json:"correctingOut,omitempty"`
	CorrectingReason    string         `json:"correctingReason,omitempty"`
	Decimals            uint64         `json:"decimals"`
	EndBal              big.Int        `json:"endBal"`
	GasOut              big.Int        `json:"gasOut,omitempty"`
	InternalIn          big.Int        `json:"internalIn,omitempty"`
	InternalOut         big.Int        `json:"internalOut,omitempty"`
	LogIndex            base.Blknum    `json:"logIndex"`
	MinerBaseRewardIn   big.Int        `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn big.Int        `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        big.Int        `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  big.Int        `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           big.Int        `json:"prefundIn,omitempty"`
	PrevAppBlk          base.Blknum    `json:"prevAppBlk,omitempty"`
	PrevBal             big.Int        `json:"prevBal,omitempty"`
	PriceSource         string         `json:"priceSource"`
	Recipient           base.Address   `json:"recipient"`
	ReconciliationType  string         `json:"reconciliationType"`
	SelfDestructIn      big.Int        `json:"selfDestructIn,omitempty"`
	SelfDestructOut     big.Int        `json:"selfDestructOut,omitempty"`
	Sender              base.Address   `json:"sender"`
	SpotPrice           float64        `json:"spotPrice"`
	Timestamp           base.Timestamp `json:"timestamp"`
	TransactionHash     base.Hash      `json:"transactionHash"`
	TransactionIndex    base.Blknum    `json:"transactionIndex"`
	raw                 *RawStatement  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleStatement) Raw() *RawStatement {
	return s.raw
}

func (s *SimpleStatement) SetRaw(raw *RawStatement) {
	s.raw = raw
}

func (s *SimpleStatement) Model(verbose bool, format string, extraOptions map[string]any) Model {
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
		"assetSymbol":         s.AssetSymbol,
		"decimals":            s.Decimals,
		"spotPrice":           s.SpotPrice,
		"priceSource":         s.PriceSource,
		"accountedFor":        s.AccountedFor,
		"sender":              s.Sender,
		"recipient":           s.Recipient,
		"begBal":              utils.FormattedValue(s.BegBal, asEther, decimals),
		"amountNet":           utils.FormattedValue(*s.AmountNet(), asEther, decimals),
		"endBal":              utils.FormattedValue(s.EndBal, asEther, decimals),
		"reconciliationType":  s.ReconciliationType,
		"reconciled":          s.Reconciled(),
		"totalIn":             utils.FormattedValue(*s.TotalIn(), asEther, decimals),
		"amountIn":            utils.FormattedValue(s.AmountIn, asEther, decimals),
		"internalIn":          utils.FormattedValue(s.InternalIn, asEther, decimals),
		"selfDestructIn":      utils.FormattedValue(s.SelfDestructIn, asEther, decimals),
		"minerBaseRewardIn":   utils.FormattedValue(s.MinerBaseRewardIn, asEther, decimals),
		"minerNephewRewardIn": utils.FormattedValue(s.MinerNephewRewardIn, asEther, decimals),
		"minerTxFeeIn":        utils.FormattedValue(s.MinerTxFeeIn, asEther, decimals),
		"minerUncleRewardIn":  utils.FormattedValue(s.MinerUncleRewardIn, asEther, decimals),
		"correctingIn":        utils.FormattedValue(s.CorrectingIn, asEther, decimals),
		"prefundIn":           utils.FormattedValue(s.PrefundIn, asEther, decimals),
		"totalOut":            utils.FormattedValue(*s.TotalOut(), asEther, decimals),
		"amountOut":           utils.FormattedValue(s.AmountOut, asEther, decimals),
		"internalOut":         utils.FormattedValue(s.InternalOut, asEther, decimals),
		"correctingOut":       utils.FormattedValue(s.CorrectingOut, asEther, decimals),
		"selfDestructOut":     utils.FormattedValue(s.SelfDestructOut, asEther, decimals),
		"gasOut":              utils.FormattedValue(s.GasOut, asEther, decimals),
		"totalOutLessGas":     utils.FormattedValue(*s.TotalOutLessGas(), asEther, decimals),
		"prevAppBlk":          s.PrevAppBlk,
		"prevBal":             utils.FormattedValue(s.PrevBal, asEther, decimals),
		"begBalDiff":          utils.FormattedValue(*s.BegBalDiff(), asEther, decimals),
		"endBalDiff":          utils.FormattedValue(*s.EndBalDiff(), asEther, decimals),
		"endBalCalc":          utils.FormattedValue(*s.EndBalCalc(), asEther, decimals),
		"correctingReason":    s.CorrectingReason,
	}
	order = []string{
		"blockNumber", "transactionIndex", "logIndex", "transactionHash", "timestamp", "date",
		"assetAddr", "assetSymbol", "decimals", "spotPrice", "priceSource", "accountedFor",
		"sender", "recipient", "begBal", "amountNet", "endBal", "reconciliationType", "reconciled",
		"totalIn", "amountIn", "internalIn", "selfDestructIn", "minerBaseRewardIn", "minerNephewRewardIn",
		"minerTxFeeIn", "minerUncleRewardIn", "prefundIn", "totalOut", "amountOut", "internalOut",
		"selfDestructOut", "gasOut", "totalOutLessGas", "prevAppBlk", "prevBal", "begBalDiff",
		"endBalDiff", "endBalCalc", "correctingReason",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleStatement) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleStatement) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleStatement) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

func (s *SimpleStatement) TotalIn() *big.Int {
	vals := []big.Int{
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

	sum := big.NewInt(0)
	for _, n := range vals {
		sum = sum.Add(sum, &n)
	}

	return sum
}

func (s *SimpleStatement) TotalOut() *big.Int {
	vals := []big.Int{
		s.AmountOut,
		s.InternalOut,
		s.CorrectingOut,
		s.SelfDestructOut,
		s.GasOut,
	}

	sum := big.NewInt(0)
	for _, n := range vals {
		sum = sum.Add(sum, &n)
	}

	return sum
}

func (s *SimpleStatement) MoneyMoved() bool {
	return s.TotalIn() != new(big.Int).SetUint64(0) || s.TotalOut() != new(big.Int).SetUint64(0)
}

func (s *SimpleStatement) AmountNet() *big.Int {
	return new(big.Int).Sub(s.TotalIn(), s.TotalOut())
}

func (s *SimpleStatement) TotalOutLessGas() *big.Int {
	val := s.TotalOut()
	return new(big.Int).Sub(val, &s.GasOut)
}

func (s *SimpleStatement) BegBalDiff() *big.Int {
	val := &big.Int{}

	if s.BlockNumber == 0 {
		val = new(big.Int).SetInt64(0)
	} else {
		new(big.Int).Sub(&s.BegBal, &s.PrevBal)
	}

	return val
}

func (s *SimpleStatement) EndBalCalc() *big.Int {
	return new(big.Int).Add(&s.BegBal, s.AmountNet())
}

func (s *SimpleStatement) EndBalDiff() *big.Int {
	return new(big.Int).Sub(s.EndBalCalc(), &s.EndBal)
}

func (s *SimpleStatement) Reconciled() bool {
	zero := new(big.Int).SetInt64(0)
	return (s.EndBalDiff().Cmp(zero) == 0 && s.BegBalDiff().Cmp(zero) == 0)
}

// ClearInternal clears all the internal accounting values. Keeps AmountIn, AmountOut and GasOut
// because those are at the top level (both the transaction itself and trace '0' have them). We
// skip trace '0' because it's the same as the transaction.
func (s *SimpleStatement) ClearInternal() {
	// s.AmountIn.SetUint64(0)
	s.InternalIn.SetUint64(0)
	s.MinerBaseRewardIn.SetUint64(0)
	s.MinerNephewRewardIn.SetUint64(0)
	s.MinerTxFeeIn.SetUint64(0)
	s.MinerUncleRewardIn.SetUint64(0)
	s.CorrectingIn.SetUint64(0)
	s.PrefundIn.SetUint64(0)
	s.SelfDestructIn.SetUint64(0)

	// s.AmountOut.SetUint64(0)
	// s.GasOut.SetUint64(0)
	s.InternalOut.SetUint64(0)
	s.CorrectingOut.SetUint64(0)
	s.SelfDestructOut.SetUint64(0)
}

func (s *SimpleStatement) IsEth() bool {
	return s.AssetAddr == base.FAKE_ETH_ADDRESS
}

var (
	sai  = base.HexToAddress("0x89d24a6b4ccb1b6faa2625fe562bdd9a23260359")
	dai  = base.HexToAddress("0x6b175474e89094c44da98b954eedeac495271d0f")
	usdc = base.HexToAddress("0xa0b86991c6218b36c1d19d4a2e9eb0ce3606eb48")
	usdt = base.HexToAddress("0xdac17f958d2ee523a2206206994597c13d831ec7")
)

func (s *SimpleStatement) IsStableCoin() bool {
	stables := map[base.Address]bool{
		sai:  true,
		dai:  true,
		usdc: true,
		usdt: true,
	}
	return stables[s.AssetAddr]
}

func (s *SimpleStatement) isNullTransfer(tx *SimpleTransaction) bool {
	lotsOfLogs := len(tx.Receipt.Logs) > 10
	mayBeAirdrop := s.Sender.IsZero() || s.Sender == tx.To
	noBalanceChange := s.EndBal.Cmp(&s.BegBal) == 0 && s.MoneyMoved()
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
	logger.TestLog(true, "  MoneyMoved:       ", s.MoneyMoved())
	logger.TestLog(true, "    noBalanceChange: -->", noBalanceChange)

	if !ret {
		logger.TestLog(true, "  ---> Not a nullTransfer")
	}
	return ret
}

func (s *SimpleStatement) CorrectForNullTransfer(tx *SimpleTransaction) bool {
	if !s.IsEth() {
		if s.isNullTransfer(tx) {
			logger.TestLog(true, "Correcting token transfer for a null transfer")
			amt := s.TotalIn() // use totalIn since this is the amount that was faked
			s.AmountOut = *new(big.Int)
			s.AmountIn = *new(big.Int)
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

func (s *SimpleStatement) CorrectForSomethingElse(tx *SimpleTransaction) bool {
	if !s.IsEth() {
		logger.TestLog(true, "Correcting token transfer for unknown income or outflow")

		s.CorrectingIn.SetUint64(0)
		s.CorrectingOut.SetUint64(0)
		s.CorrectingReason = ""
		zero := new(big.Int).SetInt64(0)
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
			n := new(big.Int).Add(&s.CorrectingIn, s.EndBalDiff())
			s.CorrectingIn = *n
			s.CorrectingReason += "endbal"
		} else if cmpEndBal < 0 {
			n := new(big.Int).Add(&s.CorrectingOut, s.EndBalDiff())
			s.CorrectingOut = *n
			s.CorrectingReason += "endbal"
		}
		s.CorrectingReason = strings.Replace(s.CorrectingReason, "begbalendbal", "begbal-endbal", -1)
	} else {
		logger.TestLog(true, "Needs correction for eth")
	}

	return s.Reconciled()
}

// EXISTING_CODE
