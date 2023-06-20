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
	fmt := func(in big.Int) string {
		if extraOptions["ether"] == true {
			return utils.WeiToEther(&in).Text('f', -1*int(s.Decimals))
		}
		return in.Text(10)
	}

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
		"begBal":              fmt(s.BegBal),
		"amountNet":           fmt(*s.AmountNet()),
		"endBal":              fmt(s.EndBal),
		"reconciliationType":  s.ReconciliationType,
		"reconciled":          s.Reconciled(),
		"totalIn":             fmt(*s.TotalIn()),
		"amountIn":            fmt(s.AmountIn),
		"internalIn":          fmt(s.InternalIn),
		"selfDestructIn":      fmt(s.SelfDestructIn),
		"minerBaseRewardIn":   fmt(s.MinerBaseRewardIn),
		"minerNephewRewardIn": fmt(s.MinerNephewRewardIn),
		"minerTxFeeIn":        fmt(s.MinerTxFeeIn),
		"minerUncleRewardIn":  fmt(s.MinerUncleRewardIn),
		"prefundIn":           fmt(s.PrefundIn),
		"totalOut":            fmt(*s.TotalOut()),
		"amountOut":           fmt(s.AmountOut),
		"internalOut":         fmt(s.InternalOut),
		"selfDestructOut":     fmt(s.SelfDestructOut),
		"gasOut":              fmt(s.GasOut),
		"totalOutLessGas":     fmt(*s.TotalOutLessGas()),
		"prevAppBlk":          s.PrevAppBlk,
		"prevBal":             fmt(s.PrevBal),
		"begBalDiff":          fmt(*s.BegBalDiff()),
		"endBalDiff":          fmt(*s.EndBalDiff()),
		"endBalCalc":          fmt(*s.EndBalCalc()),
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

var debugging = false

func (s *SimpleStatement) TotalIn() *big.Int {
	vals := []big.Int{
		s.AmountIn,
		s.InternalIn,
		s.SelfDestructIn,
		s.MinerBaseRewardIn,
		s.MinerNephewRewardIn,
		s.MinerTxFeeIn,
		s.MinerUncleRewardIn,
		s.PrefundIn,
	}

	sum := big.NewInt(0)
	for _, n := range vals {
		sum = sum.Add(sum, &n)
	}

	logger.TestLog(debugging, "-------- TotalIn ----------------")
	logger.TestLog(debugging, "AmountIn:            ", s.AmountIn.Text(10))
	logger.TestLog(debugging, "InternalIn:          ", s.InternalIn.Text(10))
	logger.TestLog(debugging, "SelfDestructIn:      ", s.SelfDestructIn.Text(10))
	logger.TestLog(debugging, "MinerBaseRewardIn:   ", s.MinerBaseRewardIn.Text(10))
	logger.TestLog(debugging, "MinerNephewRewardIn: ", s.MinerNephewRewardIn.Text(10))
	logger.TestLog(debugging, "MinerTxFeeIn:        ", s.MinerTxFeeIn.Text(10))
	logger.TestLog(debugging, "MinerUncleRewardIn:  ", s.MinerUncleRewardIn.Text(10))
	logger.TestLog(debugging, "PrefundIn:           ", s.PrefundIn.Text(10))
	logger.TestLog(debugging, "TotalIn:             ", sum.Text(10))

	return sum
}

func (s *SimpleStatement) TotalOut() *big.Int {
	vals := []big.Int{
		s.AmountOut,
		s.InternalOut,
		s.SelfDestructOut,
		s.GasOut,
	}

	sum := big.NewInt(0)
	for _, n := range vals {
		sum = sum.Add(sum, &n)
	}

	logger.TestLog(debugging, "-------- TotalOut ----------------")
	logger.TestLog(debugging, "AmountOut:      ", s.AmountOut.Text(10))
	logger.TestLog(debugging, "InternalOut:    ", s.InternalOut.Text(10))
	logger.TestLog(debugging, "SelfDestructOut:", s.SelfDestructOut.Text(10))
	logger.TestLog(debugging, "GasOut:         ", s.GasOut.Text(10))
	logger.TestLog(debugging, "TotalOut:       ", sum.Text(10))

	return sum
}

func (s *SimpleStatement) AmountNet() *big.Int {
	val := new(big.Int).Sub(s.TotalIn(), s.TotalOut())

	logger.TestLog(debugging, "-------- AmountNet ----------------")
	logger.TestLog(debugging, "TotalIn:        ", s.TotalIn().Text(10))
	logger.TestLog(debugging, "TotalOut:       ", s.TotalOut().Text(10))
	logger.TestLog(debugging, "AmountNet:      ", val.Text(10))

	return val
}

func (s *SimpleStatement) TotalOutLessGas() *big.Int {
	val := s.TotalOut()
	val = new(big.Int).Sub(val, &s.GasOut)

	logger.TestLog(debugging, "-------- TotalOutLessGas ----------------")
	logger.TestLog(debugging, "TotalOut:       ", s.TotalOut().Text(10))
	logger.TestLog(debugging, "GasOut:         ", s.GasOut.Text(10))
	logger.TestLog(debugging, "TotalOutLessGas:", val.Text(10))

	return val
}

func (s *SimpleStatement) BegBalDiff() *big.Int {
	val := &big.Int{}

	if s.BlockNumber == 0 {
		val = new(big.Int).SetInt64(0)
	} else {
		new(big.Int).Sub(&s.BegBal, &s.PrevBal)
	}

	logger.TestLog(debugging, "-------- BegBalDiff ----------------")
	logger.TestLog(debugging, "BegBal:         ", s.BegBal.Text(10))
	logger.TestLog(debugging, "PrevBal:        ", s.PrevBal.Text(10))
	logger.TestLog(debugging, "BegBalDiff:     ", val.Text(10))

	return val
}

func (s *SimpleStatement) EndBalCalc() *big.Int {
	val := new(big.Int).Add(&s.BegBal, s.AmountNet())

	logger.TestLog(debugging, "-------- EndBalCalc ----------------")
	logger.TestLog(debugging, "BegBal:         ", s.BegBal.Text(10))
	logger.TestLog(debugging, "AmountNet:      ", s.AmountNet().Text(10))
	logger.TestLog(debugging, "EndBalCalc:     ", val.Text(10))

	return val
}

func (s *SimpleStatement) EndBalDiff() *big.Int {
	val := new(big.Int).Sub(s.EndBalCalc(), &s.EndBal)

	logger.TestLog(debugging, "-------- EndBalDiff ----------------")
	logger.TestLog(debugging, "EndBalCalc:     ", s.EndBalCalc().Text(10))
	logger.TestLog(debugging, "EndBal:         ", s.EndBal.Text(10))
	logger.TestLog(debugging, "EndBalDiff:     ", val.Text(10))

	return val
}

func (s *SimpleStatement) Reconciled() bool {
	zero := new(big.Int).SetInt64(0)
	val := (s.EndBalDiff().Cmp(zero) == 0 && s.BegBalDiff().Cmp(zero) == 0)

	// debugging = true
	logger.TestLog(debugging, "-------- Reconciled ----------------")
	logger.TestLog(debugging, "zero:           ", zero.Text(10))
	logger.TestLog(debugging, "BegBalDiff:     ", s.BegBalDiff().Text(10))
	logger.TestLog(debugging, "EndBalDiff:     ", s.EndBalDiff().Text(10))
	logger.TestLog(debugging, "First:          ", s.EndBalDiff().Cmp(zero) == 0)
	logger.TestLog(debugging, "Second:         ", s.BegBalDiff().Cmp(zero) == 0)
	logger.TestLog(debugging, "Reconciled:     ", val)
	// debugging = false

	return val
}

func (s *SimpleStatement) ClearInternal() {
	// s.AmountIn.SetUint64(0)
	// s.AmountOut.SetUint64(0)
	// s.GasOut.SetUint64(0)
	s.InternalIn.SetUint64(0)
	s.InternalOut.SetUint64(0)
	s.MinerBaseRewardIn.SetUint64(0)
	s.MinerNephewRewardIn.SetUint64(0)
	s.MinerTxFeeIn.SetUint64(0)
	s.MinerUncleRewardIn.SetUint64(0)
	s.PrefundIn.SetUint64(0)
	s.SelfDestructIn.SetUint64(0)
	s.SelfDestructOut.SetUint64(0)
}

// EXISTING_CODE
