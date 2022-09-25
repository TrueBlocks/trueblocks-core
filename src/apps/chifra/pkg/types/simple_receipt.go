package types

import (
	"github.com/ethereum/go-ethereum/common"
)

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
	Status           uint64         `json:"status"`
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
	if r.IsError || (format != "json" && format != "api") {
		model["isError"] = r.IsError
		order = append(order, "isError")
	}

	if showHidden && format == "json" {
		model["blockHash"] = r.BlockHash
		model["contractAddress"] = r.ContractAddress
		model["cumulativeGasUsed"] = r.CumulativeGasUsed
		// TODO: This is commented out becuase r.From is empty, so incorrect
		// model["from"] = r.From
		model["effectiveGasPrice"] = r.EffectiveGasPrice
		model["logs"] = r.Logs
		// TODO: This is commented out becuase r.From is empty, so incorrect
		// model["to"] = r.To

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

	if len(r.Logs) > 0 && (format == "json" || format == "api") {
		model["logs"] = r.Logs
		order = append(order, []string{"logs"}...)
	}

	return Model{
		Data:  model,
		Order: order,
	}
}
