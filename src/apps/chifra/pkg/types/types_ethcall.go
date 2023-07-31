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

type RawEthCall struct {
	Abi_spec    string `json:"abi_spec"`
	Address     string `json:"address"`
	BlockNumber string `json:"blockNumber"`
	Bytes       string `json:"bytes"`
	Deployed    string `json:"deployed"`
	Encoding    string `json:"encoding"`
	Signature   string `json:"signature"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleEthCall struct {
	Address          base.Address    `json:"address"`
	BlockNumber      base.Blknum     `json:"blockNumber"`
	Bytes            string          `json:"bytes"`
	CallResult       *SimpleFunction `json:"callResult"`
	CompressedResult string          `json:"compressedResult"`
	Deployed         base.Blknum     `json:"deployed"`
	Encoding         string          `json:"encoding"`
	Signature        string          `json:"signature"`
	raw              *RawEthCall     `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleEthCall) Raw() *RawEthCall {
	return s.raw
}

func (s *SimpleEthCall) SetRaw(raw *RawEthCall) {
	s.raw = raw
}

func (s *SimpleEthCall) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
// EXISTING_CODE
