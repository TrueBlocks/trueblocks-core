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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
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

func (s *SimpleEthState) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model["blockNumber"] = s.BlockNumber
	model["address"] = s.Address

	order = []string{"blockNumber", "address"}

	// TODO: fixme
	if extraOptions != nil {
		if fields, ok := extraOptions["fields"]; ok {
			if fields, ok := fields.([]string); ok {
				for _, field := range fields {
					switch field {
					case "balance":
						model["balance"] = s.Balance.String()
					case "nonce":
						model["nonce"] = s.Nonce
					case "code":
						if !verbose {
							model["code"] = s.CodeShort()
						} else {
							model["code"] = s.Code
						}
					case "proxy":
						model["proxy"] = s.Proxy
					case "deployed":
						if s.Deployed == utils.NOPOS {
							model["deployed"] = ""
						} else {
							model["deployed"] = s.Deployed
						}
					case "type":
						model["type"] = s.Accttype
					}
				}
				order = append(order, fields...)
			}
		}
	}
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
func (s *SimpleEthState) CodeShort() string {
	codeLen := len(s.Code)
	if codeLen <= 250 {
		return s.Code
	}

	return strings.Join(
		[]string{
			s.Code[:20],
			s.Code[codeLen-20:],
		},
		"...",
	)
}

// EXISTING_CODE
