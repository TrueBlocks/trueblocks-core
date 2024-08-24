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

type Abi struct {
	Address     base.Address `json:"address"`
	FileSize    int64        `json:"fileSize"`
	Functions   []Function   `json:"functions"`
	IsEmpty     bool         `json:"isEmpty"`
	IsKnown     bool         `json:"isKnown"`
	LastModDate string       `json:"lastModDate"`
	NEvents     int64        `json:"nEvents"`
	NFunctions  int64        `json:"nFunctions"`
	Name        string       `json:"name"`
	Path        string       `json:"path"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Abi) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Abi) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	if extraOpts["list"] == true {
		model = map[string]any{
			"address":     s.Address,
			"name":        s.Name,
			"lastModDate": s.LastModDate,
			"fileSize":    s.FileSize,
			"isKnown":     s.IsKnown,
		}
		order = []string{
			"address",
			"name",
			"lastModDate",
			"fileSize",
			"isKnown",
		}

		if s.IsKnown {
			model["address"] = ""
		} else {
			if name, loaded, found := nameAddress(extraOpts, s.Address); found {
				model["name"] = name.Name
			} else if loaded {
				model["name"] = ""
			}
		}
		if format == "json" && s.Address.IsZero() {
			delete(model, "address")
		}

		if verbose {
			if format == "json" {
				if s.IsEmpty {
					model["isEmpty"] = s.IsEmpty
				}
			} else {
				model["isEmpty"] = s.IsEmpty
				order = append(order, "isEmpty")
			}
			model["nFunctions"] = s.NFunctions
			order = append(order, "nFunctions")
			model["nEvents"] = s.NEvents
			order = append(order, "nEvents")
			model["path"] = s.Path
			order = append(order, "path")
		}

	} else {
		model[s.Address.Hex()] = s.Functions
		order = append(order, s.Address.Hex())
		if name, loaded, found := nameAddress(extraOpts, s.Address); found {
			model["addressName"] = name.Name
			order = append(order, "addressName")
		} else if loaded && format != "json" {
			model["addressName"] = ""
			order = append(order, "addressName")
		}
	}
	order = reorderOrdering(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Abi) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
