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

type Approval struct {
	Allowance    base.Wei       `json:"allowance"`
	BlockNumber  base.Blknum    `json:"blockNumber"`
	LastAppBlock base.Blknum    `json:"lastAppBlock"`
	LastAppLogID base.Lognum    `json:"lastAppLogID"`
	LastAppTs    base.Timestamp `json:"lastAppTs"`
	LastAppTxID  base.Txnum     `json:"lastAppTxID"`
	Owner        base.Address   `json:"owner"`
	Spender      base.Address   `json:"spender"`
	Timestamp    base.Timestamp `json:"timestamp"`
	Token        base.Address   `json:"token"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Approval) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Approval) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"blockNumber":  s.BlockNumber,
		"timestamp":    s.Timestamp,
		"date":         s.Date(),
		"owner":        s.Owner,
		"spender":      s.Spender,
		"token":        s.Token,
		"allowance":    s.Allowance.String(),
		"lastAppBlock": s.LastAppBlock,
		"lastAppLogID": s.LastAppLogID,
		"lastAppTxID":  s.LastAppTxID,
		"lastAppTs":    s.LastAppTs,
		"lastAppDate":  base.FormattedDate(s.LastAppTs),
	}

	order = []string{
		"blockNumber",
		"timestamp",
		"date",
		"owner",
		"spender",
		"token",
		"allowance",
		"lastAppBlock",
		"lastAppLogID",
		"lastAppTxID",
		"lastAppTs",
		"lastAppDate",
	}

	if verbose {
		items := []namer{
			{addr: s.Owner, name: "ownerName"},
			{addr: s.Spender, name: "spenderName"},
			{addr: s.Token, name: "tokenName"},
		}
		for _, item := range items {
			if name, loaded, found := nameAddress(extraOpts, item.addr); found {
				model[item.name] = name.Name
				order = append(order, item.name)
			} else if loaded && format != "json" {
				model[item.name] = ""
				order = append(order, item.name)
			}
		}
		order = reorderOrdering(order)
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Approval) Date() string {
	return base.FormattedDate(s.Timestamp)
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Approval) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
