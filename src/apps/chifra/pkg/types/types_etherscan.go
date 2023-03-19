// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import "github.com/ethereum/go-ethereum/common"

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
}

type SimpleEtherscan struct {
	ArticulatedTx    SimpleFunction `json:"articulatedTx"`
	BlockHash        common.Hash    `json:"blockHash"`
	BlockNumber      uint64         `json:"blockNumber"`
	CompressedTx     string         `json:"compressedTx"`
	Date             string         `json:"date"`
	Ether            string         `json:"ether"`
	EtherGasPrice    string         `json:"etherGasPrice"`
	ExtraValue1      Wei            `json:"extraValue1"`
	ExtraValue2      Wei            `json:"extraValue2"`
	From             Address        `json:"from"`
	Gas              Gas            `json:"gas"`
	GasPrice         Gas            `json:"gasPrice"`
	GasUsed          string         `json:"gasUsed"`
	HasToken         uint8          `json:"hasToken"`
	Hash             common.Hash    `json:"hash"`
	Input            string         `json:"input"`
	IsError          uint8          `json:"isError"`
	Timestamp        Timestamp      `json:"timestamp"`
	To               Address        `json:"to"`
	TransactionIndex uint64         `json:"transactionIndex"`
	Value            Wei            `json:"value"`
	raw              *RawEtherscan
}

func (s *SimpleEtherscan) Raw() *RawEtherscan {
	return s.raw
}

func (s *SimpleEtherscan) SetRaw(raw *RawEtherscan) {
	s.raw = raw
}

func (s *SimpleEtherscan) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"articulatedTx":    s.ArticulatedTx,
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"compressedTx":     s.CompressedTx,
		"date":             s.Date,
		"ether":            s.Ether,
		"etherGasPrice":    s.EtherGasPrice,
		"extraValue1":      s.ExtraValue1,
		"extraValue2":      s.ExtraValue2,
		"from":             s.From,
		"gas":              s.Gas,
		"gasPrice":         s.GasPrice,
		"gasUsed":          s.GasUsed,
		"hasToken":         s.HasToken,
		"hash":             s.Hash,
		"input":            s.Input,
		"isError":          s.IsError,
		"timestamp":        s.Timestamp,
		"to":               s.To,
		"transactionIndex": s.TransactionIndex,
		"value":            s.Value,
	}

	order := []string{
		"articulatedTx",
		"blockHash",
		"blockNumber",
		"compressedTx",
		"date",
		"ether",
		"etherGasPrice",
		"extraValue1",
		"extraValue2",
		"from",
		"gas",
		"gasPrice",
		"gasUsed",
		"hash",
		"hasToken",
		"input",
		"isError",
		"timestamp",
		"to",
		"transactionIndex",
		"value",
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
type EtherscanResponse struct {
	Message string         `json:"message"`
	Result  []RawEtherscan `json:"result"`
	Status  string         `json:"status"`
}

// EXISTING_CODE
