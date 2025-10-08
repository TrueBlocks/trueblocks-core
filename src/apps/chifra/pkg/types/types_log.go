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
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/topics"
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
	Calcs            *LogCalcs      `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Log) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Log) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
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
		"isNFT",
	}

	isArticulated := extraOpts["articulate"] == true && s.ArticulatedLog != nil
	if isArticulated && format != "json" {
		order = append(order, "compressedLog")
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this Log.
func (s *Log) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"address":          s.Address,
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"logIndex":         s.LogIndex,
		"timestamp":        s.Timestamp,
		"transactionIndex": s.TransactionIndex,
		"transactionHash":  s.TransactionHash,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Log) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"date": s.Date(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	isArticulated := p.ExtraOpts["articulate"] == true && s.ArticulatedLog != nil
	var articulatedLog = make(map[string]any)
	if isArticulated {
		articulatedLog["name"] = s.ArticulatedLog.Name
		inputModels := parametersToMap(s.ArticulatedLog.Inputs)
		if inputModels != nil {
			articulatedLog["inputs"] = inputModels
		}
	}

	if p.Format == "json" {
		if s.IsNFT() {
			model["isNFT"] = s.IsNFT()
		}

		if len(s.Data) > 2 {
			model["data"] = s.Data
		}

		if isArticulated {
			model["articulatedLog"] = articulatedLog
		}

		model["topics"] = s.Topics

	} else {
		model["isNFT"] = s.IsNFT()

		if len(s.Data) > 2 {
			model["data"] = s.Data
		} else {
			model["data"] = ""
		}

		if isArticulated {
			model["compressedLog"] = MakeCompressed(articulatedLog)
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
	// EXISTING_CODE

	return model
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
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// LogCalcs holds lazy-loaded calculated fields for Log
type LogCalcs struct {
	// EXISTING_CODE
	Date           string                 `json:"date"`
	IsNFT          bool                   `json:"isNFT,omitempty"`
	Data           string                 `json:"data,omitempty"`
	ArticulatedLog map[string]interface{} `json:"articulatedLog,omitempty"`
	Topics         []string               `json:"topics,omitempty"`
	CompressedLog  string                 `json:"compressedLog,omitempty"`
	Topic0         string                 `json:"topic0,omitempty"`
	Topic1         string                 `json:"topic1,omitempty"`
	Topic2         string                 `json:"topic2,omitempty"`
	Topic3         string                 `json:"topic3,omitempty"`
	// EXISTING_CODE
}

func (s *Log) EnsureCalcs(p *ModelProps, requestedFields []string) error {
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &LogCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
func (l *Log) IsRelevant(addr base.Address, checkAddress bool) bool {
	target := addr.Bytes()

	if checkAddress {
		if bytes.Equal(l.Address.Bytes(), target) {
			return true
		}
	}

	if bytes.Contains([]byte(l.Data), target) {
		return true
	}

	for _, topic := range l.Topics {
		if bytes.Contains(topic.Bytes(), target) {
			return true
		}
	}

	return false
}

func (log *Log) IsNFT() bool {
	return len(log.Topics) == 4 && log.Topics[0] == topics.TransferTopic
}

func (log *Log) CompressedLog() string {
	if log.ArticulatedLog == nil {
		return ""
	}
	return MakeCompressed(log.ArticulatedLog)
}

func (log *Log) Log2Haystack() string {
	haystack := make([]byte, 66*len(log.Topics)+len(log.Data))
	haystack = append(haystack, log.Address.Hex()[2:]...)
	for _, topic := range log.Topics {
		haystack = append(haystack, topic.Hex()[2:]...)
	}
	haystack = append(haystack, log.Data[2:]...)
	return string(haystack)
}

// EXISTING_CODE
