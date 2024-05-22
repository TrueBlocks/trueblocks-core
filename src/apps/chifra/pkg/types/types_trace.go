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
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// EXISTING_CODE

type Trace struct {
	Action           *TraceAction   `json:"action"`
	ArticulatedTrace *Function      `json:"articulatedTrace,omitempty"`
	BlockHash        base.Hash      `json:"blockHash"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	Error            string         `json:"error,omitempty"`
	Result           *TraceResult   `json:"result"`
	Subtraces        uint64         `json:"subtraces"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TraceAddress     []uint64       `json:"traceAddress"`
	TransactionHash  base.Hash      `json:"transactionHash"`
	TransactionIndex base.Txnum     `json:"transactionIndex"`
	TraceType        string         `json:"type,omitempty"`
	// EXISTING_CODE
	TraceIndex          base.Tracenum `json:"-"`
	sortString          string        `json:"-"`
	TransactionPosition base.Txnum    `json:"transactionPosition,omitempty"`
	// EXISTING_CODE
}

func (s Trace) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Trace) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
	isArticulated := extraOpts["articulate"] == true && s.ArticulatedTrace != nil
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
			model["action"] = s.Action.Model(chain, format, verbose, extraOpts).Data
		}
		if s.Result != nil {
			model["result"] = s.Result.Model(chain, format, verbose, extraOpts).Data
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
				model["action::ether"] = s.Action.Balance.ToEtherStr(18)
				model["action::input"] = "0x"
				model["action::callType"] = "self-destruct"
			} else {
				model["action::from"] = hexutil.Encode(s.Action.From.Bytes())
				model["action::to"] = to
				model["action::value"] = s.Action.Value.String()
				model["action::ether"] = s.Action.Value.ToEtherStr(18)
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

func (s *Trace) Date() string {
	return base.FormattedDate(s.Timestamp)
}

type TraceGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Traces           []Trace
}

func (s *TraceGroup) CacheName() string {
	return "Trace"
}

func (s *TraceGroup) CacheId() string {
	return fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
}

func (s *TraceGroup) CacheLocation() (directory string, extension string) {
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

func (s *TraceGroup) MarshalCache(writer io.Writer) (err error) {
	return cache.WriteValue(writer, s.Traces)
}

func (s *TraceGroup) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	return cache.ReadValue(reader, &s.Traces, vers)
}

func (s *Trace) MarshalCache(writer io.Writer) (err error) {
	// Action
	optAction := &cache.Optional[TraceAction]{
		Value: s.Action,
	}
	if err = cache.WriteValue(writer, optAction); err != nil {
		return err
	}

	// ArticulatedTrace
	optArticulatedTrace := &cache.Optional[Function]{
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

	// Error
	if err = cache.WriteValue(writer, s.Error); err != nil {
		return err
	}

	// Result
	optResult := &cache.Optional[TraceResult]{
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

func (s *Trace) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Action
	optAction := &cache.Optional[TraceAction]{
		Value: s.Action,
	}
	if err = cache.ReadValue(reader, optAction, vers); err != nil {
		return err
	}
	s.Action = optAction.Get()

	// ArticulatedTrace
	optArticulatedTrace := &cache.Optional[Function]{
		Value: s.ArticulatedTrace,
	}
	if err = cache.ReadValue(reader, optArticulatedTrace, vers); err != nil {
		return err
	}
	s.ArticulatedTrace = optArticulatedTrace.Get()

	// BlockHash
	if err = cache.ReadValue(reader, &s.BlockHash, vers); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// Used to be CompressedTrace, since removed
	vCompressedTrace := version.NewVersion("2.5.10")
	if vers <= vCompressedTrace.Uint64() {
		var val string
		if err = cache.ReadValue(reader, &val, vers); err != nil {
			return err
		}
	}

	// Error
	if err = cache.ReadValue(reader, &s.Error, vers); err != nil {
		return err
	}

	// Result
	optResult := &cache.Optional[TraceResult]{
		Value: s.Result,
	}
	if err = cache.ReadValue(reader, optResult, vers); err != nil {
		return err
	}
	s.Result = optResult.Get()

	// Subtraces
	if err = cache.ReadValue(reader, &s.Subtraces, vers); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, vers); err != nil {
		return err
	}

	// TraceAddress
	s.TraceAddress = make([]uint64, 0)
	if err = cache.ReadValue(reader, &s.TraceAddress, vers); err != nil {
		return err
	}

	// TransactionHash
	if err = cache.ReadValue(reader, &s.TransactionHash, vers); err != nil {
		return err
	}

	// TransactionIndex
	if err = cache.ReadValue(reader, &s.TransactionIndex, vers); err != nil {
		return err
	}

	// TraceType
	if err = cache.ReadValue(reader, &s.TraceType, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Trace) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *Trace) GetSortString() string {
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

// EXISTING_CODE
