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
	"fmt"
	"io"
	"path/filepath"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
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
	TraceIndex uint64 `json:"-"`
	// EXISTING_CODE
}

func (s *SimpleTrace) Raw() *RawTrace {
	return s.raw
}

func (s *SimpleTrace) SetRaw(raw *RawTrace) {
	s.raw = raw
}

func (s *SimpleTrace) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockHash":        s.BlockHash,
		"blockNumber":      s.BlockNumber,
		"result":           s.Result,
		"subtraces":        s.Subtraces,
		"timestamp":        s.Timestamp,
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
		// "traceIndex":       s.TraceIndex,
	}

	order = []string{
		"blockNumber",
		"transactionIndex",
		// "traceIndex",
		"timestamp",
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
		inputModels := ParametersToMap(s.ArticulatedTrace.Inputs)
		if inputModels != nil {
			articulatedTrace["inputs"] = inputModels
		}
		outputModels := ParametersToMap(s.ArticulatedTrace.Outputs)
		if outputModels != nil {
			articulatedTrace["outputs"] = outputModels
		}
		sm := s.ArticulatedTrace.StateMutability
		if sm != "" && sm != "nonpayable" && sm != "view" {
			articulatedTrace["stateMutability"] = sm
		}
	}

	if format == "json" {
		model["traceAddress"] = s.TraceAddress
		if len(s.Error) > 0 {
			model["error"] = s.Error
		}
		if len(s.TraceType) > 0 {
			model["type"] = s.TraceType
		}
		if s.Action != nil {
			model["action"] = s.Action.Model(verbose, format, extraOptions).Data
		}
		if s.Result != nil {
			model["result"] = s.Result.Model(verbose, format, extraOptions).Data
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
				model["action::ether"] = utils.FormattedValue(s.Action.Balance, true, 18)
				model["action::input"] = "0x"
				model["action::callType"] = "self-destruct"
			} else {
				model["action::from"] = hexutil.Encode(s.Action.From.Bytes())
				model["action::to"] = to
				model["action::value"] = s.Action.Value.String()
				model["action::ether"] = utils.FormattedValue(s.Action.Value, true, 18)
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
			model["compressedTrace"] = MakeCompressed(articulatedTrace)
			order = append(order, "compressedTrace")
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleTrace) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTrace) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func mustParseUint(input any) (result uint64) {
	result, _ = strconv.ParseUint(fmt.Sprint(input), 0, 64)
	return
}

func (s *SimpleTrace) MarshalCache(writer io.Writer) (err error) {
	if err = cacheNew.WriteValue(writer, s.Action); err != nil {
		return err
	}

	optArticulatedTrace := &cacheNew.Optional[SimpleFunction]{
		Value: s.ArticulatedTrace,
	}
	if err = cacheNew.WriteValue(writer, optArticulatedTrace); err != nil {
		return err
	}

	if err = cacheNew.WriteValue(writer, &s.BlockHash); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.CompressedTrace); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Error); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Result); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Subtraces); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.TraceAddress); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, &s.TransactionHash); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.TransactionIndex); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.TraceType); err != nil {
		return err
	}

	return nil
}

func (s *SimpleTrace) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	if s.Action == nil {
		s.Action = new(SimpleTraceAction)
	}
	if err = cacheNew.ReadValue(reader, s.Action, version); err != nil {
		return err
	}

	// ArticulatedTrace can be missing
	optArticulatedTrace := &cacheNew.Optional[SimpleFunction]{
		Value: s.ArticulatedTrace,
	}
	if err = cacheNew.ReadValue(reader, optArticulatedTrace, version); err != nil {
		return err
	}
	s.ArticulatedTrace = optArticulatedTrace.Get()

	if err = cacheNew.ReadValue(reader, &s.BlockHash, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.CompressedTrace, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Error, version); err != nil {
		return err
	}

	if s.Result == nil {
		s.Result = new(SimpleTraceResult)
	}
	if err = cacheNew.ReadValue(reader, s.Result, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Subtraces, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}

	if err = cacheNew.ReadValue(reader, &s.TraceAddress, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.TransactionHash, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.TransactionIndex, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.TraceType, version); err != nil {
		return err
	}

	return nil
}

// We store traces in the cache as an array, so we need
// a data type reflecting this.
type SimpleTraceGroup struct {
	// The actual traces being cached
	Traces []SimpleTrace
	// Details for cache locator
	BlockNumber      base.Blknum
	TransactionIndex int
}

func NewSimpleTraceGroup(tx *SimpleTransaction) *SimpleTraceGroup {
	return &SimpleTraceGroup{
		Traces:           make([]SimpleTrace, 0, len(tx.Traces)),
		BlockNumber:      tx.Receipt.BlockNumber,
		TransactionIndex: int(tx.TransactionIndex),
	}
}

func (s *SimpleTraceGroup) CacheName() string {
	return "Trace"
}

func (s *SimpleTraceGroup) CacheId() string {
	return fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
}

func (s *SimpleTraceGroup) CacheLocation() (directory string, extension string) {
	extension = "bin"

	id := s.CacheId()

	parts := make([]string, 3)
	parts[0] = id[:2]
	parts[1] = id[2:4]
	parts[2] = id[4:6]
	directory = filepath.Join("traces", filepath.Join(parts...))
	return
}

func (s *SimpleTraceGroup) MarshalCache(writer io.Writer) (err error) {
	return cacheNew.WriteValue(writer, s.Traces)
}

func (s *SimpleTraceGroup) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	return cacheNew.ReadValue(reader, &s.Traces, version)
}

// EXISTING_CODE
