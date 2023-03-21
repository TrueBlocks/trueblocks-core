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
	"github.com/bykof/gostradamus"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawEtherscan struct {
	BlockHash         string `json:"blockHash"`
	BlockNumber       string `json:"blockNumber"`
	ContractAddress   string `json:"contractAddress"`
	CumulativeGasUsed string `json:"cumulativeGasUsed"`
	From              string `json:"from"`
	FunctionName      string `json:"functionName"`
	Gas               string `json:"gas"`
	GasPrice          string `json:"gasPrice"`
	GasUsed           string `json:"gasUsed"`
	HasToken          string `json:"hasToken"`
	Hash              string `json:"hash"`
	Input             string `json:"input"`
	IsError           string `json:"isError"`
	MethodId          string `json:"methodId"`
	Nonce             string `json:"nonce"`
	Timestamp         string `json:"timestamp"`
	To                string `json:"to"`
	TransactionIndex  string `json:"transactionIndex"`
	TxReceiptStatus   string `json:"txreceipt_status"`
	Value             string `json:"value"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleEtherscan struct {
	BlockHash        base.Hash      `json:"blockHash"`
	BlockNumber      uint64         `json:"blockNumber"`
	ContractAddress  base.Address   `json:"contractAddress"`
	Date             string         `json:"date"`
	Ether            string         `json:"ether"`
	From             base.Address   `json:"from"`
	Gas              base.Gas       `json:"gas"`
	GasPrice         base.Gas       `json:"gasPrice"`
	GasUsed          base.Gas       `json:"gasUsed"`
	GasCost          base.Gas       `json:"gasCost"`
	HasToken         bool           `json:"hasToken"`
	Hash             base.Hash      `json:"hash"`
	Input            string         `json:"input"`
	IsError          bool           `json:"isError"`
	Timestamp        base.Timestamp `json:"timestamp"`
	To               base.Address   `json:"to"`
	TransactionIndex uint64         `json:"transactionIndex"`
	Value            base.Wei       `json:"value"`
	raw              *RawEtherscan  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleEtherscan) Raw() *RawEtherscan {
	return s.raw
}

func (s *SimpleEtherscan) SetRaw(raw *RawEtherscan) {
	s.raw = raw
}

func (s *SimpleEtherscan) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	date := gostradamus.FromUnixTimestamp(s.Timestamp)

	model = map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"date":        s.Date,
		"ether":       s.Ether,
		"from":        s.From,
		"timestamp":   s.Timestamp,
		"to":          s.To,
		"value":       s.Value.String(),
		// "hash":        s.Hash,
		// "gas":         s.Gas,
		// "gasCost":     s.GasCost,
		// "gasPrice":    s.GasPrice,
		// "gasUsed":     s.GasUsed,
		// "blockHash":        s.BlockHash,
		// "transactionIndex": s.TransactionIndex,
		// "input":            s.Input,
		// "hasToken":         s.HasToken,
		// "isError":          s.IsError,
		// "contractAddress":  s.ContractAddress,
		// "articulatedTx":    s.ArticulatedTx,
		// "compressedTx":     s.CompressedTx,
		// "etherGasPrice":    s.EtherGasPrice,
		// "extraValue1":      s.ExtraValue1,
		// "extraValue2":      s.ExtraValue2,
	}

	// var order []string

	model["date"] = date.Format("2006-01-02 15:04:05") + " UTC"
	if strings.Contains(s.Input, "Reward") {
		model["from"] = s.Input
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
			// "blockHash",
			"blockNumber",
			"transactionIndex",
			"timestamp",
			// "contractAddress",
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
			// "articulatedTx",
			// "compressedTx",
			// "etherGasPrice",
			// "extraValue1",
			// "extraValue2",
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
	model["ether"] = utils.WeiToEther(&s.Value).Text('f', 18)
	if s.BlockHash != base.HexToHash("0xdeadbeef") {
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
		if len(s.Input) > 0 && s.Input != "deprecated" {
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

func (s *SimpleEtherscan) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleEtherscan) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleEtherscan) SetGasCost() base.Gas {
	s.GasCost = s.GasPrice * s.GasUsed
	return s.GasCost
}

type EtherscanResponse struct {
	Message string         `json:"message"`
	Result  []RawEtherscan `json:"result"`
	Status  string         `json:"status"`
}

// EXISTING_CODE
