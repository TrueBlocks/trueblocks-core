package types

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

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

// TODO: BOGUS2 - MUST DOCUMENT
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
}

func (s *SimpleEtherscan) Raw() *RawEtherscan {
	return s.raw
}

func (s *SimpleEtherscan) SetRaw(r *RawEtherscan) {
	s.raw = r
}

func (s *SimpleEtherscan) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	to := hexutil.Encode(s.To.Bytes())
	if to == "0x0000000000000000000000000000000000000000" {
		to = "0x0" // weird special case to preserve what RPC does
	}

	model = map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"date":        s.Date,
		"ether":       s.Ether,
		"from":        s.From,
		"timestamp":   s.Timestamp,
		"to":          s.To,
		"value":       s.Value.String(),
	}
	model["date"] = utils.FormattedDate(s.Timestamp)

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

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleEtherscan) SetGasCost() base.Gas {
	s.GasCost = s.GasPrice * s.GasUsed
	return s.GasCost
}
