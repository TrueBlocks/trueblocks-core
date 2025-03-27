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

type Transfer struct {
	AmountIn            base.Wei     `json:"amountIn,omitempty"`
	AmountOut           base.Wei     `json:"amountOut,omitempty"`
	Asset               base.Address `json:"asset"`
	BlockNumber         base.Blknum  `json:"blockNumber"`
	Decimals            uint64       `json:"decimals"`
	GasOut              base.Wei     `json:"gasOut,omitempty"`
	Holder              base.Address `json:"holder"`
	InternalIn          base.Wei     `json:"internalIn,omitempty"`
	InternalOut         base.Wei     `json:"internalOut,omitempty"`
	LogIndex            base.Lognum  `json:"logIndex"`
	MinerBaseRewardIn   base.Wei     `json:"minerBaseRewardIn,omitempty"`
	MinerNephewRewardIn base.Wei     `json:"minerNephewRewardIn,omitempty"`
	MinerTxFeeIn        base.Wei     `json:"minerTxFeeIn,omitempty"`
	MinerUncleRewardIn  base.Wei     `json:"minerUncleRewardIn,omitempty"`
	PrefundIn           base.Wei     `json:"prefundIn,omitempty"`
	Recipient           base.Address `json:"recipient"`
	SelfDestructIn      base.Wei     `json:"selfDestructIn,omitempty"`
	SelfDestructOut     base.Wei     `json:"selfDestructOut,omitempty"`
	Sender              base.Address `json:"sender"`
	TransactionIndex    base.Txnum   `json:"transactionIndex"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Transfer) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Transfer) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
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
func (s *Transfer) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
