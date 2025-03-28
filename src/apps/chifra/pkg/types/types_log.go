// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package types

// EXISTING_CODE
import (
	"encoding/json"
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type Log struct {
	Address          base.Address   `json:"address"`
	ArticulatedLog   *Function      `json:"articulatedLog,omitempty"`
	BlockHash        base.Hash      `json:"blockHash"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Data             string         `json:"data,omitempty"`
	LogIndex         base.Lognum    `json:"logIndex"`
	Timestamp        base.Timestamp `json:"timestamp,omitempty"`
	Topics           []base.Hash    `json:"topics,omitempty"`
	TransactionHash  base.Hash      `json:"transactionHash"`
	TransactionIndex base.Txnum     `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Log) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Log) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":          s.Address,
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"logIndex":         s.LogIndex,
		"timestamp":        s.Timestamp,
		"date":             s.Date(),
		"transactionIndex": s.TransactionIndex,
		"transactionHash":  s.TransactionHash,
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		"logIndex",
		"blockHash",
		"transactionHash",
		"timestamp",
		"date",
		"address",
		"topic0",
		"topic1",
		"topic2",
		"topic3",
		"data",
	}

	isArticulated := extraOpts["articulate"] == true && s.ArticulatedLog != nil
	var articulatedLog = make(map[string]any)
	if isArticulated {
		articulatedLog["name"] = s.ArticulatedLog.Name
		inputModels := parametersToMap(s.ArticulatedLog.Inputs)
		if inputModels != nil {
			articulatedLog["inputs"] = inputModels
		}
	}

	if format == "json" {
		if len(s.Data) > 2 {
			model["data"] = s.Data
		}
		if isArticulated {
			model["articulatedLog"] = articulatedLog
		}

		model["topics"] = s.Topics

	} else {
		if len(s.Data) > 2 {
			model["data"] = s.Data
		} else {
			model["data"] = ""
		}

		if isArticulated {
			model["compressedLog"] = MakeCompressed(articulatedLog)
			order = append(order, "compressedLog")
		}

		model["topic0"] = ""
		if len(s.Topics) > 0 {
			model["topic0"] = s.Topics[0]
		}
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

	if name, loaded, found := nameAddress(extraOpts, s.Address); found {
		model["addressName"] = name.Name
		order = append(order, "addressName")
	} else if loaded && format != "json" {
		model["addressName"] = ""
		order = append(order, "addressName")
	}
	order = reorderOrdering(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Log) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *LogGroup) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d", s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Log") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

type LogGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Logs             []Log
}

func (s *LogGroup) MarshalCache(writer io.Writer) (err error) {
	return base.WriteValue(writer, s.Logs)
}

func (s *LogGroup) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	return base.ReadValue(reader, &s.Logs, fileVersion)
}

func (s *Log) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// ArticulatedLog
	optArticulatedLog := &cache.Optional[Function]{
		Value: s.ArticulatedLog,
	}
	if err = base.WriteValue(writer, optArticulatedLog); err != nil {
		return err
	}

	// BlockHash
	if err = base.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Data
	if err = base.WriteValue(writer, s.Data); err != nil {
		return err
	}

	// LogIndex
	if err = base.WriteValue(writer, s.LogIndex); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// Topics
	if err = base.WriteValue(writer, s.Topics); err != nil {
		return err
	}

	// TransactionHash
	if err = base.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	return nil
}

func (s *Log) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// ArticulatedLog
	optArticulatedLog := &cache.Optional[Function]{
		Value: s.ArticulatedLog,
	}
	if err = base.ReadValue(reader, optArticulatedLog, fileVersion); err != nil {
		return err
	}
	s.ArticulatedLog = optArticulatedLog.Get()

	// BlockHash
	if err = base.ReadValue(reader, &s.BlockHash, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// Used to be CompressedLog, since removed
	vCompressedLog := version.NewVersion("2.5.10")
	if fileVersion <= vCompressedLog.Uint64() {
		var val string
		if err = base.ReadValue(reader, &val, fileVersion); err != nil {
			return err
		}
	}

	// Data
	if err = base.ReadValue(reader, &s.Data, fileVersion); err != nil {
		return err
	}

	// LogIndex
	if err = base.ReadValue(reader, &s.LogIndex, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// Topics
	s.Topics = make([]base.Hash, 0)
	if err = base.ReadValue(reader, &s.Topics, fileVersion); err != nil {
		return err
	}

	// TransactionHash
	if err = base.ReadValue(reader, &s.TransactionHash, fileVersion); err != nil {
		return err
	}

	// TransactionIndex
	if err = base.ReadValue(reader, &s.TransactionIndex, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Log) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
