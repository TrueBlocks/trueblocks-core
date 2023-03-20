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
	Address             base.Address    `json:"address"`
	ArticulatedLog      *SimpleFunction `json:"-"`
	BlockHash           common.Hash     `json:"blockHash"`
	BlockNumber         base.Blknum     `json:"blockNumber"`
	CompressedLog       string          `json:"compressedLog"`
	Data                string          `json:"data"`
	LogIndex            base.Blknum     `json:"logIndex"`
	Timestamp           base.Timestamp  `json:"timestamp"`
	Topic0              base.Topic      `json:"topic0"`
	Topic1              base.Topic      `json:"topic1"`
	Topic2              base.Topic      `json:"topic2"`
	Topic3              base.Topic      `json:"topic3"`
	Topics              []common.Hash   `json:"topics"`
	TransactionHash     common.Hash     `json:"transactionHash"`
	TransactionIndex    base.Blknum     `json:"transactionIndex"`
	TransactionLogIndex base.Blknum     `json:"transactionLogIndex"`
	LogType             string          `json:"type"`
	raw                 *RawLog
}

func (s *SimpleLog) Raw() *RawLog {
	return s.raw
}

func (s *SimpleLog) SetRaw(raw *RawLog) {
	s.raw = raw
}

func (s *SimpleLog) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"address":          s.Address,
		"blockNumber":      s.BlockNumber,
		"logIndex":         s.LogIndex,
		"timestamp":        s.Timestamp,
		"transactionIndex": s.TransactionIndex,
		"transactionHash":  s.TransactionHash,
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		"logIndex",
		"transactionHash",
		"timestamp",
		"address",
		"topic0",
		"topic1",
		"topic2",
		"topic3",
		"data",
	}

	isArticulated := extraOptions["articulate"] == true && s.ArticulatedLog != nil
	var articulatedLog = make(map[string]any)
	if isArticulated {
		articulatedLog["name"] = s.ArticulatedLog.Name
		inputModels := ParametersToMap(s.ArticulatedLog.Inputs)
		if inputModels != nil {
			articulatedLog["inputs"] = inputModels
		}
	}

	if format == "json" {
		if len(s.Data) > 0 && s.Data != "0x" {
			model["data"] = s.Data
		} else {
			model["data"] = ""
		}
		if isArticulated {
			model["articulatedLog"] = articulatedLog
		}

		model["topics"] = s.Topics

	} else {
		if len(s.Data) > 0 && s.Data != "0x" {
			model["data"] = s.Data
		}

		if isArticulated {
			model["compressedLog"] = MakeCompressed(articulatedLog)
			order = append(order, "compressedLog")
		}

		model["topic0"] = s.Topics[0]
		model["topic1"] = ""
		if len(s.Topics) > 1 {
			model["topic1"] = s.Topics[1]
		}
		model["topic2"] = ""
		if len(s.Topics) > 2 {
			model["topic2"] = s.Topics[2]
		}
		model["topic3"] = ""
		if len(s.Topics) > 3 {
			model["topic3"] = s.Topics[3]
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleLog) Write(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleLog) Read(p []byte) (n int, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
