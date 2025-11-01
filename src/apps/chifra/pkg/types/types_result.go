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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/cache"
)

// EXISTING_CODE

type Result struct {
	Address          base.Address   `json:"address"`
	ArticulatedOut   *Function      `json:"articulatedOut"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	EncodedArguments string         `json:"encodedArguments"`
	Encoding         string         `json:"encoding"`
	Name             string         `json:"name"`
	Signature        string         `json:"signature"`
	Timestamp        base.Timestamp `json:"timestamp"`
	Calcs            *ResultCalcs   `json:"calcs,omitempty"`
	// EXISTING_CODE
	Values        map[string]string `json:"values"`
	ReturnedBytes string
	// EXISTING_CODE
}

func (s Result) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Result) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
	if verbose {
		order = []string{
			"blockNumber",
			"timestamp",
			"date",
			"address",
			"signature",
			"encoding",
			"bytes",
			"compressedResult",
		}
	} else {
		order = []string{
			"blockNumber",
			"address",
			"signature",
			"encoding",
			"bytes",
			"compressedResult",
		}
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

// RawMap returns a map containing only the raw/base fields for this Result.
func (s *Result) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"blockNumber": s.BlockNumber,
		"address":     s.Address.Hex(),
		"encoding":    s.Encoding,
		"bytes":       s.EncodedArguments,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		model["timestamp"] = s.Timestamp
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *Result) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		model["date"] = s.Date()
	}

	callResult := map[string]any{
		"name":      s.Name,
		"signature": s.Signature,
		"encoding":  s.Encoding,
		"outputs":   s.Values,
	}
	model["callResult"] = callResult

	isArticulated := p.ExtraOpts["articulate"] == true && s.ArticulatedOut != nil
	var articulatedOut map[string]any
	if isArticulated {
		articulatedOut = map[string]any{
			"name": s.ArticulatedOut.Name,
		}
		outputModels := parametersToMap(s.ArticulatedOut.Outputs)
		if outputModels != nil {
			articulatedOut["outputs"] = outputModels
		}
		if p.Format == "json" {
			model["callResult"] = articulatedOut
		}
	}

	if p.Format != "json" {
		model["signature"] = s.Signature
		model["compressedResult"] = MakeCompressed(s.Values)
	}
	// EXISTING_CODE

	return model
}

func (s *Result) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *Result) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%s-%s-%09d", s.Address.Hex()[2:], s.Encoding[2:], s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Result") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *Result) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// ArticulatedOut
	optArticulatedOut := &cache.Optional[Function]{
		Value: s.ArticulatedOut,
	}
	if err = base.WriteValue(writer, optArticulatedOut); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// EncodedArguments
	if err = base.WriteValue(writer, s.EncodedArguments); err != nil {
		return err
	}

	// Encoding
	if err = base.WriteValue(writer, s.Encoding); err != nil {
		return err
	}

	// Name
	if err = base.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// Signature
	if err = base.WriteValue(writer, s.Signature); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	return nil
}

func (s *Result) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// ArticulatedOut
	optArticulatedOut := &cache.Optional[Function]{
		Value: s.ArticulatedOut,
	}
	if err = base.ReadValue(reader, optArticulatedOut, fileVersion); err != nil {
		return err
	}
	s.ArticulatedOut = optArticulatedOut.Get()

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// EncodedArguments
	if err = base.ReadValue(reader, &s.EncodedArguments, fileVersion); err != nil {
		return err
	}

	// Encoding
	if err = base.ReadValue(reader, &s.Encoding, fileVersion); err != nil {
		return err
	}

	// Name
	if err = base.ReadValue(reader, &s.Name, fileVersion); err != nil {
		return err
	}

	// Signature
	if err = base.ReadValue(reader, &s.Signature, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Result) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	s.Values = make(map[string]string)
	for index, output := range s.ArticulatedOut.Outputs {
		s.Values[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}
	// EXISTING_CODE
}

// ResultCalcs holds lazy-loaded calculated fields for Result
type ResultCalcs struct {
	// EXISTING_CODE
	Date       string      `json:"date,omitempty"`
	CallResult interface{} `json:"callResult"`
	// EXISTING_CODE
}

func (s *Result) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
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

	s.Calcs = &ResultCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// TODO: The above CacheId makes a very large number of files for a large contract.
// EXISTING_CODE
