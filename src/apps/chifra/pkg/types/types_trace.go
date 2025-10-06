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
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
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

	if format != "json" {
		for _, item := range rawNames {
			key := item.name + "Name"
			if _, exists := model[key]; exists {
				order = append(order, key)
			}
		}

		// Add compressedTrace if it exists
		if _, exists := model["compressedTrace"]; exists {
			order = append(order, "compressedTrace")
		}

		order = reorderFields(order)
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

// RawMap returns a map containing only the raw/base fields for this Trace.
func (s *Trace) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"blockHash":   s.BlockHash,
		"blockNumber": s.BlockNumber,
		"subtraces":   s.Subtraces,
		"timestamp":   s.Timestamp,
		// TODO: Do we want date here? We did not put it in `order`.
		// "date":             s.Date(),
		"transactionHash":  s.TransactionHash,
		"transactionIndex": s.TransactionIndex,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Format != "json" && s.Action != nil {
		*needed = append(*needed, NewLabeler(s.Action.From, "action::from"))
		if !s.Action.RefundAddress.IsZero() {
			*needed = append(*needed, NewLabeler(s.Action.RefundAddress, "action::to"))
		} else {
			*needed = append(*needed, NewLabeler(s.Action.To, "action::to"))
		}
	}

	if p.Format == "json" {
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
			model["action"] = s.Action.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data
		}
		if s.Result != nil {
			model["result"] = s.Result.Model(p.Chain, p.Format, p.Verbose, p.ExtraOpts).Data
		}
	} else {
		model["error"] = s.Error
		if s.Action != nil {
			model["action::callType"] = s.Action.CallType
			model["action::gas"] = s.Action.Gas
			model["action::input"] = s.Action.Input
		}
		if s.Result != nil {
			model["result::gasUsed"] = s.Result.GasUsed
			model["result::output"] = s.Result.Output
		} else {
			model["result::gasUsed"] = "0"
			model["result::output"] = ""
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this Trace.
func (s *Trace) CalcMap(p *ModelProps) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"date": s.Date(),
		// EXISTING_CODE
	}

	// EXISTING_CODE
	var articulatedTrace map[string]any
	isArticulated := p.ExtraOpts["articulate"] == true && s.ArticulatedTrace != nil
	if isArticulated {
		articulatedTrace = map[string]any{
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

	if p.Format == "json" {
		if isArticulated {
			model["articulatedTrace"] = articulatedTrace
		}
	} else {
		if s.Action != nil {
			to := hexutil.Encode(s.Action.To.Bytes())
			if to == "0x0000000000000000000000000000000000000000" {
				to = "0x0"
			}

			if !s.Action.RefundAddress.IsZero() {
				model["action::from"] = hexutil.Encode(s.Action.From.Bytes())
				model["action::to"] = hexutil.Encode(s.Action.RefundAddress.Bytes())
				model["action::value"] = s.Action.Balance.String()
				model["action::ether"] = s.Action.Balance.ToFloatString(18)
				model["action::input"] = "0x"
				model["action::callType"] = "self-destruct"
			} else {
				model["action::from"] = hexutil.Encode(s.Action.From.Bytes())
				model["action::to"] = to
				model["action::value"] = s.Action.Value.String()
				model["action::ether"] = s.Action.Value.ToFloatString(18)
			}
		}

		if isArticulated {
			model["compressedTrace"] = MakeCompressed(articulatedTrace)
		}
	}
	// EXISTING_CODE

	return model
}

func (s *Trace) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *TraceGroup) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%09d-%05d", s.BlockNumber, s.TransactionIndex)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Trace") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

type TraceGroup struct {
	BlockNumber      base.Blknum
	TransactionIndex base.Txnum
	Traces           []Trace
}

func (s *TraceGroup) MarshalCache(writer io.Writer) (err error) {
	return base.WriteValue(writer, s.Traces)
}

func (s *TraceGroup) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	return base.ReadValue(reader, &s.Traces, fileVersion)
}

func (s *Trace) MarshalCache(writer io.Writer) (err error) {
	// Action
	optAction := &cache.Optional[TraceAction]{
		Value: s.Action,
	}
	if err = base.WriteValue(writer, optAction); err != nil {
		return err
	}

	// ArticulatedTrace
	optArticulatedTrace := &cache.Optional[Function]{
		Value: s.ArticulatedTrace,
	}
	if err = base.WriteValue(writer, optArticulatedTrace); err != nil {
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

	// Error
	if err = base.WriteValue(writer, s.Error); err != nil {
		return err
	}

	// Result
	optResult := &cache.Optional[TraceResult]{
		Value: s.Result,
	}
	if err = base.WriteValue(writer, optResult); err != nil {
		return err
	}

	// Subtraces
	if err = base.WriteValue(writer, s.Subtraces); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	// TraceAddress
	if err = base.WriteValue(writer, s.TraceAddress); err != nil {
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

	// TraceType
	if err = base.WriteValue(writer, s.TraceType); err != nil {
		return err
	}

	return nil
}

func (s *Trace) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Action
	optAction := &cache.Optional[TraceAction]{
		Value: s.Action,
	}
	if err = base.ReadValue(reader, optAction, fileVersion); err != nil {
		return err
	}
	s.Action = optAction.Get()

	// ArticulatedTrace
	optArticulatedTrace := &cache.Optional[Function]{
		Value: s.ArticulatedTrace,
	}
	if err = base.ReadValue(reader, optArticulatedTrace, fileVersion); err != nil {
		return err
	}
	s.ArticulatedTrace = optArticulatedTrace.Get()

	// BlockHash
	if err = base.ReadValue(reader, &s.BlockHash, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// Used to be CompressedTrace, since removed
	vCompressedTrace := version.NewVersion("2.5.10")
	if fileVersion <= vCompressedTrace.Uint64() {
		var val string
		if err = base.ReadValue(reader, &val, fileVersion); err != nil {
			return err
		}
	}

	// Error
	if err = base.ReadValue(reader, &s.Error, fileVersion); err != nil {
		return err
	}

	// Result
	optResult := &cache.Optional[TraceResult]{
		Value: s.Result,
	}
	if err = base.ReadValue(reader, optResult, fileVersion); err != nil {
		return err
	}
	s.Result = optResult.Get()

	// Subtraces
	if err = base.ReadValue(reader, &s.Subtraces, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	// TraceAddress
	s.TraceAddress = make([]uint64, 0)
	if err = base.ReadValue(reader, &s.TraceAddress, fileVersion); err != nil {
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

	// TraceType
	if err = base.ReadValue(reader, &s.TraceType, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Trace) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
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
