package types

import (
	"github.com/ethereum/go-ethereum/common"
)

type RawReceipt struct {
	BlockHash         string   `json:"blockHash"`
	BlockNumber       string   `json:"blockNumber"`
	ContractAddress   string   `json:"contractAddress"`
	CumulativeGasUsed string   `json:"cumulativeGasUsed"`
	EffectiveGasPrice string   `json:"effectiveGasPrice"`
	From              string   `json:"from"`
	GasUsed           string   `json:"gasUsed"`
	Logs              []RawLog `json:"logs"`
	LogsBloom         string   `json:"logsBloom"`
	Status            string   `json:"status"`
	To                string   `json:"to"`
	TransactionHash   string   `json:"transactionHash"`
	TransactionIndex  string   `json:"transactionIndex"`
	Type              string   `json:"type"`
}

type SimpleReceipt struct {
	BlockHash         common.Hash    `json:"blockHash"`
	BlockNumber       uint64         `json:"blockNumber"`
	ContractAddress   common.Address `json:"contractAddress,omitempty"`
	CumulativeGasUsed string         `json:"cumulativeGasUsed"`
	From              common.Address `json:"from"`
	GasUsed           uint64         `json:"gasUsed"`
	EffectiveGasPrice uint64         `json:"effectiveGasPrice"`
	Logs              []SimpleLog    `json:"logs,omitempty"`
	// LogsBloom         string         `json:"-"`
	// Root              string         `json:"-"`
	Status           uint32         `json:"status"`
	IsError          bool           `json:"isError,omitempty"`
	To               common.Address `json:"to,omitempty"`
	TransactionHash  common.Hash    `json:"hash"`
	TransactionIndex uint64         `json:"transactionIndex"`
	raw              *RawReceipt
}

func (r *SimpleReceipt) Raw() *RawReceipt {
	return r.raw
}

func (r *SimpleReceipt) SetRaw(rawReceipt RawReceipt) {
	r.raw = &rawReceipt
}

// To support custom format: just execute template on the output of Model
func (r *SimpleReceipt) Model(showHidden bool, format string) Model {
	model := map[string]interface{}{
		"blockNumber":      r.BlockNumber,
		"transactionIndex": r.TransactionIndex,
		"hash":             r.TransactionHash,
		"gasUsed":          r.GasUsed,
		"status":           r.Status,
	}
	order := []string{
		"blockNumber",
		"transactionIndex",
		"hash",
		"gasUsed",
		"status",
	}
	if r.IsError || format != "json" {
		model["isError"] = r.IsError
		order = append(order, "isError")
	}

	if showHidden && format == "json" {
		// TODO: The tests in this section of code are basically implementing `omitempty`
		model["blockHash"] = r.BlockHash
		if r.ContractAddress != common.HexToAddress("0x0") {
			model["contractAddress"] = r.ContractAddress
		}
		model["cumulativeGasUsed"] = r.CumulativeGasUsed
		if r.From != common.HexToAddress("0x0") {
			model["from"] = r.From
		}
		model["effectiveGasPrice"] = r.EffectiveGasPrice
		if len(r.Logs) > 0 {
			model["logs"] = r.Logs
		}
		if r.To != common.HexToAddress("0x0") {
			model["to"] = r.To
		}

		order = append(order, []string{
			"blockHash",
			"contractAddress",
			"cumulativeGasUsed",
			"from",
			"effectiveGasPrice",
			"logs",
			"to",
		}...)
	}

	if len(r.Logs) > 0 && format == "json" {
		model["logs"] = r.Logs
		order = append(order, []string{"logs"}...)
	}

	return Model{
		Data:  model,
		Order: order,
	}
}
