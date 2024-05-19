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
)

// EXISTING_CODE

type RawResult struct {
	Address          string `json:"address"`
	BlockNumber      string `json:"blockNumber"`
	EncodedArguments string `json:"encodedArguments"`
	Encoding         string `json:"encoding"`
	Name             string `json:"name"`
	Signature        string `json:"signature"`
	Timestamp        string `json:"timestamp"`
}

type Result struct {
	Address          base.Address   `json:"address"`
	ArticulatedOut   *Function      `json:"articulatedOut"`
	BlockNumber      base.Blknum    `json:"blockNumber"`
	EncodedArguments string         `json:"encodedArguments"`
	Encoding         string         `json:"encoding"`
	Name             string         `json:"name"`
	Signature        string         `json:"signature"`
	Timestamp        base.Timestamp `json:"timestamp"`
	raw              *RawResult     `json:"-"`
	// EXISTING_CODE
	Values        map[string]string `json:"values"`
	ReturnedBytes string
	// EXISTING_CODE
}

func (s Result) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Result) Raw() *RawResult {
	return s.raw
}

func (s *Result) SetRaw(raw *RawResult) {
	s.raw = raw
}

func (s *Result) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	callResult := map[string]any{
		"name":      s.Name,
		"signature": s.Signature,
		"encoding":  s.Encoding,
		"outputs":   s.Values,
	}
	model = map[string]any{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.Timestamp,
		"date":        s.Date(),
		"address":     s.Address.Hex(),
		"encoding":    s.Encoding,
		"bytes":       s.EncodedArguments,
		"callResult":  callResult,
	}

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
		delete(model, "timestamp")
		delete(model, "date")
	}

	isArticulated := extraOptions["articulate"] == true && s.ArticulatedOut != nil
	var articulatedOut map[string]interface{}
	if isArticulated {
		articulatedOut = map[string]interface{}{
			"name": s.ArticulatedOut.Name,
		}
		outputModels := parametersToMap(s.ArticulatedOut.Outputs)
		if outputModels != nil {
			articulatedOut["outputs"] = outputModels
		}
		if format == "json" {
			model["callResult"] = articulatedOut
		}
	}

	if format != "json" {
		model["signature"] = s.Signature
		model["compressedResult"] = makeCompressed(s.Values)
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Result) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *Result) CacheName() string {
	return "Result"
}

func (s *Result) CacheId() string {
	return fmt.Sprintf("%s-%s-%09d", s.Address.Hex()[2:], s.Encoding[2:], s.BlockNumber)
}

func (s *Result) CacheLocation() (directory string, extension string) {
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

func (s *Result) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// ArticulatedOut
	optArticulatedOut := &cache.Optional[Function]{
		Value: s.ArticulatedOut,
	}
	if err = cache.WriteValue(writer, optArticulatedOut); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// EncodedArguments
	if err = cache.WriteValue(writer, s.EncodedArguments); err != nil {
		return err
	}

	// Encoding
	if err = cache.WriteValue(writer, s.Encoding); err != nil {
		return err
	}

	// Name
	if err = cache.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// Signature
	if err = cache.WriteValue(writer, s.Signature); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	return nil
}

func (s *Result) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = cache.ReadValue(reader, &s.Address, vers); err != nil {
		return err
	}

	// ArticulatedOut
	optArticulatedOut := &cache.Optional[Function]{
		Value: s.ArticulatedOut,
	}
	if err = cache.ReadValue(reader, optArticulatedOut, vers); err != nil {
		return err
	}
	s.ArticulatedOut = optArticulatedOut.Get()

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// EncodedArguments
	if err = cache.ReadValue(reader, &s.EncodedArguments, vers); err != nil {
		return err
	}

	// Encoding
	if err = cache.ReadValue(reader, &s.Encoding, vers); err != nil {
		return err
	}

	// Name
	if err = cache.ReadValue(reader, &s.Name, vers); err != nil {
		return err
	}

	// Signature
	if err = cache.ReadValue(reader, &s.Signature, vers); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Result) FinishUnmarshal() {
	// EXISTING_CODE
	s.Values = make(map[string]string)
	for index, output := range s.ArticulatedOut.Outputs {
		s.Values[output.DisplayName(index)] = fmt.Sprint(output.Value)
	}
	// EXISTING_CODE
}

// EXISTING_CODE
// TODO: The above CacheId makes a very large number of files for a large contract.
// EXISTING_CODE
