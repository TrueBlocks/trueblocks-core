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
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

const (
	// below are stored in cache, do not change
	BlockReward     base.Txnum = 99999 // the largest five-digit decimal number
	UncleReward     base.Txnum = 99998
	MisconfigReward base.Txnum = 99997
	ExternalReward  base.Txnum = 99996
	WithdrawalAmt   base.Txnum = 99995
	// above are stored in cache, do not change
	NephewReward base.Txnum = 99994
	TxFeeReward  base.Txnum = 99993
)

type ChunkAppearance = Appearance

// EXISTING_CODE

type Appearance struct {
	Address          base.Address   `json:"address"`
	BlockNumber      uint32         `json:"blockNumber"`
	Reason           string         `json:"reason,omitempty"`
	Timestamp        base.Timestamp `json:"timestamp"`
	TraceIndex       uint32         `json:"traceIndex,omitempty"`
	TransactionIndex uint32         `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Appearance) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Appearance) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	if extraOpts["appearances"] == true {
		model = map[string]any{
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

	model = map[string]any{
		"address":          s.Address,
		"blockNumber":      s.BlockNumber,
		"transactionIndex": s.TransactionIndex,
	}
	order = []string{
		"address",
		"blockNumber",
		"transactionIndex",
	}

	if name, loaded, found := nameAddress(extraOpts, s.Address); found {
		model["addressName"] = name.Name
		order = append(order, "addressName")
	} else if loaded && format != "json" {
		model["addressName"] = ""
		order = append(order, "addressName")
	}
	order = reorderOrdering(order)

	if extraOpts["uniq"] == true {
		if s.TraceIndex > 0 {
			model["traceIndex"] = s.TraceIndex
		} else if format != "json" {
			model["traceIndex"] = ""
		}
		order = append(order, "traceIndex")
		model["reason"] = s.Reason
		order = append(order, "reason")
		if verbose {
			model["timestamp"] = s.Timestamp
			order = append(order, "timestamp")
			model["date"] = s.Date()
			order = append(order, "date")
		}
	} else if extraOpts["export"] == true && format == "json" {
		if verbose {
			if s.Timestamp != base.NOPOSI {
				model["timestamp"] = s.Timestamp
			}
			model["date"] = s.Date()
		}
	} else {
		if verbose {
			if s.TraceIndex > 0 {
				model["traceIndex"] = s.TraceIndex
			} else if format != "json" {
				model["traceIndex"] = ""
			}
			order = append(order, "traceIndex")
			model["reason"] = s.Reason
			order = append(order, "reason")
			model["timestamp"] = s.Timestamp
			order = append(order, "timestamp")
			model["date"] = s.Date()
			order = append(order, "date")
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Appearance) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Appearance) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *Appearance) GetKey() string {
	return fmt.Sprintf("%s\t%09d\t%05d", s.Address, s.BlockNumber, s.TransactionIndex)
}

func (s *Appearance) Orig() string {
	return s.Reason // when converted from an Identifier, this is the original string
}

// EXISTING_CODE
