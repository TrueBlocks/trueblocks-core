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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawResult struct {
	Address          string   `json:"address"`
	BlockNumber      string   `json:"blockNumber"`
	EncodedArguments string   `json:"encodedArguments"`
	Encoding         string   `json:"encoding"`
	Name             string   `json:"name"`
	Outputs          []string `json:"outputs"`
	Signature        string   `json:"signature"`
	Timestamp        string   `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleResult struct {
	Address          base.Address      `json:"address"`
	BlockNumber      base.Blknum       `json:"blockNumber"`
	EncodedArguments string            `json:"encodedArguments"`
	Encoding         string            `json:"encoding"`
	Name             string            `json:"name"`
	Outputs          map[string]string `json:"outputs"`
	Signature        string            `json:"signature"`
	Timestamp        base.Timestamp    `json:"timestamp"`
	raw              *RawResult        `json:"-"`
	// EXISTING_CODE
	RawReturn string
	// EXISTING_CODE
}

func (s *SimpleResult) Raw() *RawResult {
	return s.raw
}

func (s *SimpleResult) SetRaw(raw *RawResult) {
	s.raw = raw
}

func (s *SimpleResult) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	callResult := map[string]any{
		"name":      s.Name,
		"signature": s.Signature,
		"encoding":  s.Encoding,
		"outputs":   s.Outputs,
	}
	model = map[string]any{
		"blockNumber": s.BlockNumber,
		"address":     s.Address.Hex(),
		"encoding":    s.Encoding,
		"bytes":       s.EncodedArguments,
		"callResult":  callResult,
	}

	if format == "json" {
		return Model{
			Data: model,
		}
	}

	model["signature"] = s.Signature
	model["compressedResult"] = makeCompressed(s.Outputs)
	order = []string{
		"blockNumber",
		"address",
		"signature",
		"encoding",
		"bytes",
		"compressedResult",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleResult) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// --> cacheable by address,block,fourbyte
func (s *SimpleResult) CacheName() string {
	return "Result"
}

func (s *SimpleResult) CacheId() string {
	return fmt.Sprintf("%s-%s-%09d", s.Address.Hex()[2:], s.Encoding[2:], s.BlockNumber)
	// TODO: The above creates a very large number of files for a large contract.
}

func (s *SimpleResult) CacheLocation() (directory string, extension string) {
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

func (s *SimpleResult) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
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

	// Outputs
	str := make([]string, len(s.Outputs)*2)
	for i, v := range s.Outputs {
		str = append(str, i)
		str = append(str, v)
	}
	if err = cache.WriteValue(writer, strings.Join(str, "|")); err != nil {
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

func (s *SimpleResult) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// Address
	if err = cache.ReadValue(reader, &s.Address, version); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, version); err != nil {
		return err
	}

	// EncodedArguments
	if err = cache.ReadValue(reader, &s.EncodedArguments, version); err != nil {
		return err
	}

	// Encoding
	if err = cache.ReadValue(reader, &s.Encoding, version); err != nil {
		return err
	}

	// Name
	if err = cache.ReadValue(reader, &s.Name, version); err != nil {
		return err
	}

	// Outputs
	var str string
	if err = cache.ReadValue(reader, &str, version); err != nil {
		return err
	}
	arr := strings.Split(str, "|")
	s.Outputs = make(map[string]string)
	for i := 0; i < len(arr); i += 2 {
		if len(arr[i]) > 0 {
			s.Outputs[arr[i]] = arr[i+1]
		}
	}
	// if err = cache.ReadValue(reader, &s.Outputs, version); err != nil {
	// 	return err
	// }

	// Signature
	if err = cache.ReadValue(reader, &s.Signature, version); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, version); err != nil {
		return err
	}

	return nil
}

// EXISTING_CODE
// EXISTING_CODE
