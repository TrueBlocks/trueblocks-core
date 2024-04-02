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
	"fmt"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

const (
	// below are stored in cache, do not change
	BlockReward     base.Txnum = 99999 // the largest five-digit decimal number
	UncleReward     base.Txnum = 99998
	MisconfigReward base.Txnum = 99997
	ExternalReward  base.Txnum = 99996
	Withdrawal      base.Txnum = 99995
	// above are stored in cache, do not change
	NephewReward base.Txnum = 99994
	TxFeeReward  base.Txnum = 99993
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

func (s *SimpleAppearance) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
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

	if extraOptions["namesMap"] != nil {
		name := extraOptions["namesMap"].(map[base.Address]SimpleName)[s.Address]
		if name.Address.Hex() != "0x0" {
			model["name"] = name
			order = append(order, "name")
		}
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
			model["date"] = s.Date()
			order = append(order, []string{
				"timestamp",
				"date",
			}...)
		}
	} else if extraOptions["export"] == true && format == "json" {
		if verbose {
			if s.Timestamp != utils.NOPOSI {
				model["timestamp"] = s.Timestamp
			}
			model["date"] = s.Date()
		}
		if extraOptions["namesMap"] != nil {
			name := extraOptions["namesMap"].(map[base.Address]SimpleName)[s.Address]
			if name.Address.Hex() != "0x0" {
				model["name"] = name.Name
				order = append(order, "name")
			}
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
			model["date"] = s.Date()
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

func (s *SimpleAppearance) Date() string {
	return utils.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *SimpleAppearance) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
//

func (s *SimpleAppearance) GetKey() string {
	return fmt.Sprintf("%s\t%09d\t%05d", s.Address, s.BlockNumber, s.TransactionIndex)
}

func (s *SimpleAppearance) Orig() string {
	return s.Reason // when converted from an Identifier, this is the original string
}

type MappedType interface {
	SimpleTransaction |
		SimpleBlock[string] |
		SimpleBlock[SimpleTransaction] |
		SimpleAppearance |
		SimpleWithdrawal |
		SimpleResult |
		SimpleToken |
		bool
}

// TODO: Do we want this to be configurable? Maybe, maybe not
var AppMapSize int = 20

func AsSliceOfMaps[T MappedType](apps []SimpleAppearance, reversed bool) ([]map[SimpleAppearance]*T, int, error) {
	sort.Slice(apps, func(i, j int) bool {
		if reversed {
			i, j = j, i
		}
		if apps[i].BlockNumber == apps[j].BlockNumber {
			return apps[i].TransactionIndex < apps[j].TransactionIndex
		}
		return apps[i].BlockNumber < apps[j].BlockNumber
	})

	arrayOfMaps := make([]map[SimpleAppearance]*T, 0, len(apps))
	curMap := make(map[SimpleAppearance]*T)
	for i := 0; i < len(apps); i++ {
		// TODO: Do we want this to be configurable? Maybe, maybe not
		if len(curMap) == AppMapSize {
			arrayOfMaps = append(arrayOfMaps, curMap)
			curMap = make(map[SimpleAppearance]*T)
		}
		curMap[apps[i]] = nil
	}

	if len(curMap) > 0 {
		arrayOfMaps = append(arrayOfMaps, curMap)
	}

	return arrayOfMaps, len(apps), nil
}

// EXISTING_CODE
