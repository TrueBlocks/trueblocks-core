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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawEtherscan struct {
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

type SimpleEtherscan struct {
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
	raw               *RawEtherscan   `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleEtherscan) Raw() *RawEtherscan {
	return s.raw
}

func (s *SimpleEtherscan) SetRaw(raw *RawEtherscan) {
	s.raw = raw
}

func (s *SimpleEtherscan) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	model = map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"date":        utils.FormattedDate(s.Timestamp),
		"ether":       s.Ether,
		"from":        s.From,
		"timestamp":   s.Timestamp,
		"to":          s.To,
		"value":       s.Value.String(),
	}

	if s.From == base.BlockRewardSender || s.From == base.UncleRewardSender {
		model["from"] = s.From.Hex()
		s.Input = ""
		order = []string{
			"blockNumber",
			"timestamp",
			"date",
			"from",
			"to",
			"value",
			"ether",
		}

	} else {
		order = []string{
			"blockNumber",
			"transactionIndex",
			"timestamp",
			"date",
			"from",
			"to",
			"hasToken",
			"isError",
			"hash",
			"gasPrice",
			"gasUsed",
			"gasCost",
			"value",
			"ether",
			"input",
		}

		model["gas"] = s.Gas
		model["gasCost"] = s.SetGasCost()
		model["gasPrice"] = s.GasPrice
		model["gasUsed"] = s.GasUsed
		model["hash"] = s.Hash
	}

	if s.HasToken {
		model["hasToken"] = s.HasToken
	}
	if s.IsError {
		model["isError"] = s.IsError
	}
	model["ether"] = utils.FormattedValue(s.Value, true, 18)
	if s.BlockHash != base.HexToHash("0xdeadbeef") && !s.BlockHash.IsZero() {
		model["blockHash"] = s.BlockHash
	}
	if s.TransactionIndex != 80809 {
		model["transactionIndex"] = s.TransactionIndex
	}

	if format == "json" {
		a := s.ContractAddress.Hex()
		if strings.HasPrefix(a, "0x") && len(a) == 42 {
			model["contractAddress"] = a
		}
		if len(s.Input) > 2 && s.Input != "deprecated" {
			model["input"] = s.Input
		}
	} else {
		model["hasToken"] = s.HasToken
		model["isError"] = s.IsError
		if s.Input == "deprecated" {
			s.Input = "0x"
		}
		model["input"] = s.Input
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// object,array

// EXISTING_CODE
func (s *SimpleEtherscan) SetGasCost() base.Gas {
	s.GasCost = s.GasPrice * s.GasUsed
	return s.GasCost
}

// EXISTING_CODE
