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
	"github.com/ethereum/go-ethereum/common"
)

// EXISTING_CODE

type RawLog struct {
	Address          string   `json:"address"`
	BlockHash        string   `json:"blockHash"`
	BlockNumber      string   `json:"blockNumber"`
	Data             string   `json:"data"`
	LogIndex         string   `json:"logIndex"`
	Removed          bool     `json:"removed"`
	Topics           []string `json:"topics"`
	TransactionHash  string   `json:"transactionHash"`
	TransactionIndex string   `json:"transactionIndex"`
}

type SimpleLog struct {
	Address          Address         `json:"address"`
	ArticulatedLog   *SimpleFunction `json:"-"`
	BlockNumber      uint64          `json:"blockNumber"`
	CompressedLog    string          `json:"compressedLog,omitempty"`
	Data             string          `json:"data,omitempty"`
	LogIndex         uint64          `json:"logIndex"`
	Timestamp        Timestamp       `json:"timestamp,omitempty"`
	Topics           []common.Hash   `json:"topics,omitempty"`
	TransactionIndex uint32          `json:"transactionIndex"`
	raw              *RawLog         `json:"-"`
}

func (s *SimpleLog) Raw() *RawLog {
	return s.raw
}

func (s *SimpleLog) SetRaw(raw *RawLog) {
	s.raw = raw
}

func (s *SimpleLog) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	// EXISTING_CODE
	// EXISTING_CODE

	model := map[string]interface{}{
		"address":          s.Address,
		"articulatedLog":   s.ArticulatedLog,
		"blockNumber":      s.BlockNumber,
		"logIndex":         s.LogIndex,
		"timestamp":        s.Timestamp,
		"transactionIndex": s.TransactionIndex,
	}

	order := []string{
		"topic3",
		"blockHash",
		"transactionLogIndex",
		"topic2",
		"topic1",
		"topic0",
		"type",
		"blockNumber",
		"transactionIndex",
		"logIndex",
		"transactionHash",
		"timestamp",
		"address",
		"data",
		"articulatedLog",
	}

	// EXISTING_CODE
	if format != "json" {
		if len(s.Data) > 0 && s.Data != "0x" {
			model["data"] = s.Data
		}
		model["compressedLog"] = s.CompressedLog
		order = append(order, "compressedLog")
	} else {
		if len(s.Data) > 0 && s.Data != "0x" {
			model["data"] = s.Data
		} else {
			model["data"] = ""
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
