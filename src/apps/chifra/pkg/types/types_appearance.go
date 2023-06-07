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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawAppearance struct {
	Address          string `json:"address"`
	BlockNumber      uint32 `json:"blockNumber"`
	TransactionIndex uint32 `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleAppearance struct {
	Address          base.Address   `json:"address"`
	BlockNumber      uint32         `json:"blockNumber"`
	Date             string         `json:"date"`
	Reason           string         `json:"reason,omitempty"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TraceIndex       uint32         `json:"traceIndex,omitempty"`
	TransactionIndex uint32         `json:"transactionIndex"`
	raw              *RawAppearance `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleAppearance) Raw() *RawAppearance {
	return s.raw
}

func (s *SimpleAppearance) SetRaw(raw *RawAppearance) {
	s.raw = raw
}

func (s *SimpleAppearance) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	if extraOptions["appearances"] == true {
		model = map[string]interface{}{
			"blockNumber":      s.BlockNumber,
			"transactionIndex": s.TransactionIndex,
		}
		order = []string{
			"blockNumber",
			"transactionIndex",
		}
		return Model{
			Data:  model,
			Order: order,
		}
	}
	model = map[string]interface{}{
		"address":          s.Address,
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
	}
	order = []string{
		"address",
		"blockNumber",
		"transactionIndex",
	}

	if extraOptions["uniq"] == true {
		if s.TraceIndex > 0 {
			model["traceIndex"] = s.TraceIndex
			order = append(order, "traceIndex")
		} else if format != "json" {
			model["traceIndex"] = ""
			order = append(order, "traceIndex")
		}
		model["reason"] = s.Reason
		order = append(order, []string{
			"reason",
		}...)
		if verbose {
			model["timestamp"] = s.Timestamp
			model["date"] = s.Date
			order = append(order, []string{
				"timestamp",
				"date",
			}...)
		}
	} else {
		if verbose {
			if s.TraceIndex > 0 {
				model["traceIndex"] = s.TraceIndex
				order = append(order, "traceIndex")
			} else if format != "json" {
				model["traceIndex"] = ""
				order = append(order, "traceIndex")
			}
			model["reason"] = s.Reason
			model["timestamp"] = s.Timestamp
			model["date"] = s.Date

			order = append(order, []string{
				"reason",
				"timestamp",
				"date",
			}...)
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *SimpleAppearance) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleAppearance) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
func (s *SimpleAppearance) GetKey() string {
	return fmt.Sprintf("%s\t%09d\t%05d", s.Address, s.BlockNumber, s.TransactionIndex)
}

// EXISTING_CODE
