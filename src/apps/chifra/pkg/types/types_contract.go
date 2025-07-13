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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type Contract struct {
	Abi         *Abi           `json:"abi"`
	Address     base.Address   `json:"address"`
	Date        string         `json:"date"`
	ErrorCount  int64          `json:"errorCount"`
	LastError   string         `json:"lastError"`
	LastUpdated base.Timestamp `json:"lastUpdated"`
	Name        string         `json:"name"`
	// EXISTING_CODE
	ReadResults map[string]interface{} `json:"readResults"`
	// EXISTING_CODE
}

func (s Contract) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Contract) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Contract) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
