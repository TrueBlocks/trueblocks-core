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

type RawMonitor struct {
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleMonitor struct {
	Address     base.Address `json:"address"`
	Deleted     bool         `json:"deleted"`
	FileSize    int64        `json:"fileSize"`
	LastScanned uint32       `json:"lastScanned"`
	NRecords    int64        `json:"nRecords"`
	raw         *RawMonitor  `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleMonitor) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *SimpleMonitor) Raw() *RawMonitor {
	return s.raw
}

func (s *SimpleMonitor) SetRaw(raw *RawMonitor) {
	s.raw = raw
}

func (s *SimpleMonitor) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":  s.Address,
		"nRecords": s.NRecords,
	}
	order = []string{
		"address",
		"nRecords",
	}

	if s.FileSize > 0 {
		model["fileSize"] = s.FileSize
		order = append(order, "fileSize")
	}

	if verbose {
		model["lastScanned"] = s.LastScanned
		model["deleted"] = s.Deleted
		if extraOptions["testMode"] == true {
			model["lastScanned"] = "--lastScanned--"
		}
		order = append(order, "lastScanned")
		order = append(order, "deleted")
		if extraOptions["namesMap"] != nil {
			name := extraOptions["namesMap"].(map[base.Address]SimpleName)[s.Address]
			if name.Address.Hex() != "0x0" {
				model["name"] = name
				order = append(order, "name")
			}
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleMonitor) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
