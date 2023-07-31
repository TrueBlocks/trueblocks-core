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
)

// EXISTING_CODE

type RawCallResult struct {
	Address          string   `json:"address"`
	BlockNumber      string   `json:"blockNumber"`
	EncodedArguments string   `json:"encodedArguments"`
	Encoding         string   `json:"encoding"`
	Name             string   `json:"name"`
	Outputs          []string `json:"outputs"`
	Signature        string   `json:"signature"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleCallResult struct {
	Address          base.Address      `json:"address"`
	BlockNumber      base.Blknum       `json:"blockNumber"`
	EncodedArguments string            `json:"encodedArguments"`
	Encoding         string            `json:"encoding"`
	Name             string            `json:"name"`
	Outputs          map[string]string `json:"outputs"`
	Signature        string            `json:"signature"`
	raw              *RawCallResult    `json:"-"`
	// EXISTING_CODE
	RawReturn string
	// EXISTING_CODE
}

func (s *SimpleCallResult) Raw() *RawCallResult {
	return s.raw
}

func (s *SimpleCallResult) SetRaw(raw *RawCallResult) {
	s.raw = raw
}

func (s *SimpleCallResult) Model(verbose bool, format string, extraOptions map[string]any) Model {
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
	model["compressedResult"] = MakeCompressed(s.Outputs)
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

// EXISTING_CODE
// EXISTING_CODE
