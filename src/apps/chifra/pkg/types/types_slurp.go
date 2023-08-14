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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawSlurp struct {
	BlockHash         string `json:"blockHash"`
	BlockNumber       string `json:"blockNumber"`
	ContractAddress   string `json:"contractAddress"`
	CumulativeGasUsed string `json:"cumulativeGasUsed"`
	Ether             string `json:"ether"`
	From              string `json:"from"`
	FunctionName      string `json:"functionName"`
	Gas               string `json:"gas"`
	GasCost           string `json:"gasCost"`
	GasPrice          string `json:"gasPrice"`
	GasUsed           string `json:"gasUsed"`
	HasToken          string `json:"hasToken"`
	Hash              string `json:"hash"`
	Input             string `json:"input"`
	MethodId          string `json:"methodId"`
	Nonce             string `json:"nonce"`
	Timestamp         string `json:"timestamp"`
	To                string `json:"to"`
	TransactionIndex  string `json:"transactionIndex"`
	TxReceiptStatus   string `json:"txReceiptStatus"`
	Value             string `json:"value"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleSlurp struct {
	ArticulatedTx     *SimpleFunction `json:"articulatedTx"`
	BlockHash         base.Hash       `json:"blockHash"`
	BlockNumber       base.Blknum     `json:"blockNumber"`
	CompressedTx      string          `json:"compressedTx"`
	ContractAddress   base.Address    `json:"contractAddress"`
	CumulativeGasUsed string          `json:"cumulativeGasUsed"`
	Ether             string          `json:"ether"`
	From              base.Address    `json:"from"`
	FunctionName      string          `json:"functionName"`
	Gas               base.Gas        `json:"gas"`
	GasCost           base.Gas        `json:"gasCost"`
	GasPrice          base.Gas        `json:"gasPrice"`
	GasUsed           base.Gas        `json:"gasUsed"`
	HasToken          bool            `json:"hasToken"`
	Hash              base.Hash       `json:"hash"`
	Input             string          `json:"input"`
	IsError           bool            `json:"isError"`
	MethodId          string          `json:"methodId"`
	Nonce             uint64          `json:"nonce"`
	Timestamp         base.Timestamp  `json:"timestamp"`
	To                base.Address    `json:"to"`
	TransactionIndex  base.Blknum     `json:"transactionIndex"`
	TxReceiptStatus   string          `json:"txReceiptStatus"`
	Value             base.Wei        `json:"value"`
	raw               *RawSlurp       `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleSlurp) Raw() *RawSlurp {
	return s.raw
}

func (s *SimpleSlurp) SetRaw(raw *RawSlurp) {
	s.raw = raw
}

func (s *SimpleSlurp) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleSlurp) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// cacheable_as_group

// EXISTING_CODE
// EXISTING_CODE
