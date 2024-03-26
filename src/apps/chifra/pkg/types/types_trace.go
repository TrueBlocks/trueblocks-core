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
	"fmt"
	"io"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type RawTrace struct {
	Action           RawTraceAction  `json:"action"`
	BlockHash        string          `json:"blockHash"`
	BlockNumber      base.Blknum     `json:"blockNumber"`
	Error            string          `json:"error"`
	Result           *RawTraceResult `json:"result"`
	Subtraces        uint64          `json:"subtraces"`
	TraceAddress     []uint64        `json:"traceAddress"`
	TransactionHash  string          `json:"transactionHash"`
	TransactionIndex uint64          `json:"transactionPosition"`
	TraceType        string          `json:"type"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTrace struct {
	Action           *SimpleTraceAction `json:"action"`
	ArticulatedTrace *SimpleFunction    `json:"articulatedTrace,omitempty"`
	BlockHash        base.Hash          `json:"blockHash"`
	BlockNumber      base.Blknum        `json:"blockNumber"`
	CompressedTrace  string             `json:"compressedTrace,omitempty"`
	Error            string             `json:"error,omitempty"`
	Result           *SimpleTraceResult `json:"result"`
	Subtraces        uint64             `json:"subtraces"`
	Timestamp        base.Timestamp     `json:"timestamp"`
	TraceAddress     []uint64           `json:"traceAddress"`
	TransactionHash  base.Hash          `json:"transactionHash"`
	TransactionIndex uint64             `json:"transactionIndex"`
	TraceType        string             `json:"type,omitempty"`
	raw              *RawTrace          `json:"-"`
	// EXISTING_CODE
	TraceIndex base.Blknum `json:"-"`
	sortString string      `json:"-"`
	// EXISTING_CODE
}

func (s *SimpleTrace) Raw() *RawTrace {
	return s.raw
}

func (s *SimpleTrace) SetRaw(raw *RawTrace) {
	s.raw = raw
}

func (s *SimpleTrace) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"result":           s.Result,
		"subtraces":        s.Subtraces,
		"timestamp":        s.Timestamp,
		"date":             s.Date(),
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		"timestamp",
		"date",
		"error",
		"action::callType",
		"action::from",
		"action::to",
		"action::value",
		"action::ether",
		"action::gas",
		"result::gasUsed",
		"action::input",
		"result::output",
	}

	var articulatedTrace map[string]interface{}
	isArticulated := extraOptions["articulate"] == true && s.ArticulatedTrace != nil
	if isArticulated {
		articulatedTrace = map[string]interface{}{
			"name": s.ArticulatedTrace.Name,
		}
		inputModels := parametersToMap(s.ArticulatedTrace.Inputs)
		if inputModels != nil {
			articulatedTrace["inputs"] = inputModels
		}
		outputModels := parametersToMap(s.ArticulatedTrace.Outputs)
		if outputModels != nil {
			articulatedTrace["outputs"] = outputModels
		}
		sm := s.ArticulatedTrace.StateMutability
		if sm != "" && sm != "nonpayable" && sm != "view" {
			articulatedTrace["stateMutability"] = sm
		}
	}

	if format == "json" {
		if s.TraceAddress == nil {
			model["traceAddress"] = []uint64{}
		} else {
			model["traceAddress"] = s.TraceAddress
		}
		if len(s.Error) > 0 {
			model["error"] = s.Error
		}
		if len(s.TraceType) > 0 {
			model["type"] = s.TraceType
		}
		if s.Action != nil {
			model["action"] = s.Action.Model(chain, format, verbose, extraOptions).Data
		}
		if s.Result != nil {
			model["result"] = s.Result.Model(chain, format, verbose, extraOptions).Data
		}

		if isArticulated {
			model["articulatedTrace"] = articulatedTrace
		}

	} else {
		to := hexutil.Encode(s.Action.To.Bytes())
		if to == "0x0000000000000000000000000000000000000000" {
			to = "0x0"
		}

		model["blockNumber"] = s.BlockNumber
		model["transactionIndex"] = s.TransactionIndex
		model["error"] = s.Error
		model["timestamp"] = s.Timestamp
		if s.Action != nil {
			model["action::callType"] = s.Action.CallType
			model["action::gas"] = s.Action.Gas
			model["action::input"] = s.Action.Input
			if !s.Action.RefundAddress.IsZero() {
				model["action::from"] = hexutil.Encode(s.Action.From.Bytes())
				model["action::to"] = hexutil.Encode(s.Action.RefundAddress.Bytes())
				model["action::value"] = s.Action.Balance.String()
				model["action::ether"] = base.FormattedValue(&s.Action.Balance, true, 18)
				model["action::input"] = "0x"
				model["action::callType"] = "self-destruct"
			} else {
				model["action::from"] = hexutil.Encode(s.Action.From.Bytes())
				model["action::to"] = to
				model["action::value"] = s.Action.Value.String()
				model["action::ether"] = base.FormattedValue(&s.Action.Value, true, 18)
			}
		}
		if s.Result != nil {
			model["result::gasUsed"] = s.Result.GasUsed
			model["result::output"] = s.Result.Output
		} else {
			model["result::gasUsed"] = "0"
			model["result::output"] = ""
		}

		if isArticulated {
			model["compressedTrace"] = makeCompressed(articulatedTrace)
			order = append(order, "compressedTrace")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTrace) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// --> cacheable by tx as group
type SimpleTraceGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Traces           []SimpleTrace
}

func (s *SimpleTraceGroup) CacheName() string {
	return "Trace"
}

func (s *SimpleTraceGroup) CacheId() string {
	return fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
}

func (s *SimpleTraceGroup) CacheLocation() (directory string, extension string) {
	paddedId := s.CacheId()
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]

	subFolder := strings.ToLower(s.CacheName()) + "s"
	directory = filepath.Join(subFolder, filepath.Join(parts...))
	extension = "bin"

	return
}

func (s *SimpleTraceGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Traces)
}

func (s *SimpleTraceGroup) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Traces, version)
}

func (s *SimpleTrace) MarshalCache(writer io.Writer) (err error) {
	// Action
	optAction := &cache.Optional[SimpleTraceAction]{
		Value: s.Action,
	}
	if err = cache.WriteValue(writer, optAction); err != nil {
		return err
	}

	// ArticulatedTrace
	optArticulatedTrace := &cache.Optional[SimpleFunction]{
		Value: s.ArticulatedTrace,
	}
	if err = cache.WriteValue(writer, optArticulatedTrace); err != nil {
		return err
	}

	// BlockHash
	if err = cache.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// CompressedTrace
	if err = cache.WriteValue(writer, s.CompressedTrace); err != nil {
		return err
	}

	// Error
	if err = cache.WriteValue(writer, s.Error); err != nil {
		return err
	}

	// Result
	optResult := &cache.Optional[SimpleTraceResult]{
		Value: s.Result,
	}
	if err = cache.WriteValue(writer, optResult); err != nil {
		return err
	}

	// Subtraces
	if err = cache.WriteValue(writer, s.Subtraces); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// TraceAddress
	if err = cache.WriteValue(writer, s.TraceAddress); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}

	// TraceType
	if err = cache.WriteValue(writer, s.TraceType); err != nil {
		return err
	}

	return nil
}

func (s *SimpleTrace) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// Action
	optAction := &cache.Optional[SimpleTraceAction]{
		Value: s.Action,
	}
	if err = cache.ReadValue(reader, optAction, version); err != nil {
		return err
	}
	s.Action = optAction.Get()

	// ArticulatedTrace
	optArticulatedTrace := &cache.Optional[SimpleFunction]{
		Value: s.ArticulatedTrace,
	}
	if err = cache.ReadValue(reader, optArticulatedTrace, version); err != nil {
		return err
	}
	s.ArticulatedTrace = optArticulatedTrace.Get()

	// BlockHash
	if err = cache.ReadValue(reader, &s.BlockHash, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}

	// CompressedTrace
	if err = cache.ReadValue(reader, &s.CompressedTrace, version); err != nil {
		return err
	}

	// Error
	if err = cache.ReadValue(reader, &s.Error, version); err != nil {
		return err
	}

	// Result
	optResult := &cache.Optional[SimpleTraceResult]{
		Value: s.Result,
	}
	if err = cache.ReadValue(reader, optResult, version); err != nil {
		return err
	}
	s.Result = optResult.Get()

	// Subtraces
	if err = cache.ReadValue(reader, &s.Subtraces, version); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}

	// TraceAddress
	s.TraceAddress = make([]uint64, 0)
	if err = cache.ReadValue(reader, &s.TraceAddress, version); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.ReadValue(reader, &s.TransactionHash, version); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.ReadValue(reader, &s.TransactionIndex, version); err != nil {
		return err
	}

	// TraceType
	if err = cache.ReadValue(reader, &s.TraceType, version); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

func (s *SimpleTrace) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *SimpleTrace) GetSortString() string {
	if len(s.sortString) > 0 {
		return s.sortString
	}

	s.sortString = ""
	s.sortString += fmt.Sprintf("%03d", s.TraceIndex)
	if s.TraceAddress != nil {
		for _, v := range s.TraceAddress {
			s.sortString += fmt.Sprintf("%03d", v)
		}
	}

	return s.sortString
}

func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

// EXISTING_CODE
