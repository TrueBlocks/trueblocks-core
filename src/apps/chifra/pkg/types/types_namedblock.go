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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawNamedBlock struct {
	BlockNumber string `json:"blockNumber"`
	Component   string `json:"component"`
	Description string `json:"description"`
	Name        string `json:"name"`
	Timestamp   string `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleNamedBlock struct {
	BlockNumber base.Blknum    `json:"blockNumber"`
	Component   string         `json:"component,omitempty"`
	Description string         `json:"description,omitempty"`
	Name        string         `json:"name,omitempty"`
	Timestamp   base.Timestamp `json:"timestamp"`
	raw         *RawNamedBlock `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleNamedBlock) Raw() *RawNamedBlock {
	return s.raw
}

func (s *SimpleNamedBlock) SetRaw(raw *RawNamedBlock) {
	s.raw = raw
}

func (s *SimpleNamedBlock) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"blockNumber": s.BlockNumber,
		"timestamp":   s.Timestamp,
		"date":        s.Date(),
	}

	order = []string{
		"blockNumber",
		"timestamp",
		"date",
	}

	if format == "json" {
		if len(s.Name) > 0 {
			model["name"] = s.Name
		}
		if verbose {
			if len(s.Component) > 0 {
				model["component"] = s.Component
			}
			if len(s.Description) > 0 {
				model["description"] = s.Description
			}
		}

	} else {
		model["name"] = s.Name
		order = append(order, "name")

		if verbose {
			model["component"] = s.Component
			order = append(order, "component")

			model["description"] = s.Description
			order = append(order, "description")
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleNamedBlock) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// EXISTING_CODE
// EXISTING_CODE
