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
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawEthState struct {
	Accttype    string `json:"accttype"`
	Address     string `json:"address"`
	Balance     string `json:"balance"`
	BlockNumber string `json:"blockNumber"`
	Code        string `json:"code"`
	Deployed    string `json:"deployed"`
	Nonce       string `json:"nonce"`
	Proxy       string `json:"proxy"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleEthState struct {
	Accttype    string       `json:"accttype"`
	Address     base.Address `json:"address"`
	Balance     base.Wei     `json:"balance"`
	BlockNumber base.Blknum  `json:"blockNumber"`
	Code        string       `json:"code"`
	Deployed    base.Blknum  `json:"deployed"`
	Nonce       uint64       `json:"nonce"`
	Proxy       base.Address `json:"proxy"`
	raw         *RawEthState `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleEthState) Raw() *RawEthState {
	return s.raw
}

func (s *SimpleEthState) SetRaw(raw *RawEthState) {
	s.raw = raw
}

func (s *SimpleEthState) Model(showHidden bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleEthState) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleEthState) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
// EXISTING_CODE
