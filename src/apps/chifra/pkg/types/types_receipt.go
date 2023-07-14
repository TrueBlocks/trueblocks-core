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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
)

// EXISTING_CODE

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
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleReceipt struct {
	BlockHash         base.Hash    `json:"blockHash,omitempty"`
	BlockNumber       base.Blknum  `json:"blockNumber"`
	ContractAddress   base.Address `json:"contractAddress,omitempty"`
	CumulativeGasUsed string       `json:"cumulativeGasUsed,omitempty"`
	EffectiveGasPrice base.Gas     `json:"effectiveGasPrice,omitempty"`
	From              base.Address `json:"from,omitempty"`
	GasUsed           base.Gas     `json:"gasUsed"`
	IsError           bool         `json:"isError,omitempty"`
	Logs              []SimpleLog  `json:"logs"`
	Status            uint32       `json:"status"`
	To                base.Address `json:"to,omitempty"`
	TransactionHash   base.Hash    `json:"transactionHash"`
	TransactionIndex  base.Blknum  `json:"transactionIndex"`
	raw               *RawReceipt  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleReceipt) Raw() *RawReceipt {
	return s.raw
}

func (s *SimpleReceipt) SetRaw(raw *RawReceipt) {
	s.raw = raw
}

func (s *SimpleReceipt) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockNumber":      s.BlockNumber,
		"gasUsed":          s.GasUsed,
		"status":           s.Status,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		"transactionHash",
		"status",
		"gasUsed",
	}

	if format == "json" {
		if !s.ContractAddress.IsZero() {
			model["contractAddress"] = s.ContractAddress
		}

		if s.IsError {
			model["isError"] = s.IsError
			order = append(order, "isError")
		}

		if extraOptions["articulate"] == true {
			logs := make([]map[string]any, 0, len(s.Logs))
			for _, log := range s.Logs {
				logModel := map[string]any{
					"address":  log.Address.Hex(),
					"logIndex": log.LogIndex,
					"topics":   log.Topics,
					"data":     log.Data,
				}
				if log.ArticulatedLog != nil {
					articulatedLog := map[string]any{
						"name": log.ArticulatedLog.Name,
					}
					inputModels := ParametersToMap(log.ArticulatedLog.Inputs)
					if inputModels != nil {
						articulatedLog["inputs"] = inputModels
					}
					logModel["articulatedLog"] = articulatedLog
				}
				logs = append(logs, logModel)
			}
			model["logs"] = logs
		} else {
			model["logs"] = s.Logs
			// order = append(order, "logs")
		}

		if verbose {
			model["blockHash"] = s.BlockHash
			order = append(order, "blockHash")

			model["cumulativeGasUsed"] = s.CumulativeGasUsed
			order = append(order, "cumulativeGasUsed")

			if !s.From.IsZero() {
				model["from"] = s.From
			}

			if !s.To.IsZero() {
				model["to"] = s.To
			}
		}
	} else {
		model["logsCnt"] = len(s.Logs)
		order = append(order, "logsCnt")

		model["isError"] = s.IsError
		order = append(order, "isError")

		if verbose {
			model["contractAddress"] = s.ContractAddress.Hex()
			order = append(order, "contractAddress")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleReceipt) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleReceipt) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleReceipt) MarshalCache(writer io.Writer) (err error) {
	if s == nil {
		zero := SimpleReceipt{}
		return zero.MarshalCache(writer)
	}

	if err = cacheNew.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.ContractAddress); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.CumulativeGasUsed); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.EffectiveGasPrice); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.From); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.GasUsed); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.IsError); err != nil {
		return err
	}

	logs := make([]cacheNew.Marshaler, 0, len(s.Logs))
	for _, log := range s.Logs {
		logs = append(logs, &log)
	}
	if err = cacheNew.WriteValue(writer, logs); err != nil {
		return err
	}

	if err = cacheNew.WriteValue(writer, s.Status); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.To); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	return
}

func (s *SimpleReceipt) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	if err = cacheNew.ReadValue(reader, &s.BlockHash, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.ContractAddress, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.CumulativeGasUsed, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.EffectiveGasPrice, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.From, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.GasUsed, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.IsError, version); err != nil {
		return err
	}

	s.Logs = make([]SimpleLog, 0)
	if err = cacheNew.ReadValue(reader, &s.Logs, version); err != nil {
		return err
	}

	if err = cacheNew.ReadValue(reader, &s.Status, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.To, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.TransactionHash, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.TransactionIndex, version); err != nil {
		return err
	}

	return
}

// EXISTING_CODE
