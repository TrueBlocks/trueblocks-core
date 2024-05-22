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
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type TraceAction struct {
	Address        base.Address `json:"address,omitempty"`
	Author         base.Address `json:"author,omitempty"`
	Balance        base.Wei     `json:"balance,omitempty"`
	CallType       string       `json:"callType"`
	From           base.Address `json:"from"`
	Gas            base.Gas     `json:"gas"`
	Init           string       `json:"init,omitempty"`
	Input          string       `json:"input,omitempty"`
	RefundAddress  base.Address `json:"refundAddress,omitempty"`
	RewardType     string       `json:"rewardType,omitempty"`
	SelfDestructed base.Address `json:"selfDestructed,omitempty"`
	To             base.Address `json:"to"`
	Value          base.Wei     `json:"value"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s TraceAction) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *TraceAction) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	if format == "json" {
		if extraOpts["traces"] != true && len(s.Init) > 0 {
			model["init"] = utils.FormattedCode(verbose, s.Init)
		}
		if !s.SelfDestructed.IsZero() {
			model["selfDestructed"] = s.SelfDestructed
		}
		if !s.From.IsZero() {
			model["from"] = s.From
		}
		if len(s.CallType) > 0 {
			model["callType"] = s.CallType
		}
		if s.Gas != 0 {
			model["gas"] = s.Gas
		}
		if len(s.Input) > 2 {
			model["input"] = s.Input
		}

		asEther := extraOpts["ether"] == true
		model["value"] = s.Value.String()
		if asEther {
			model["ether"] = s.Value.ToEtherStr(18)
		}

		if !s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if asEther {
				model["balanceEth"] = s.Balance.ToEtherStr(18)
			}

		} else {
			if s.To.IsZero() {
				model["to"] = "0x0"
			} else {
				model["to"] = s.To
			}
		}
		if len(s.Init) > 0 {
			model["init"] = utils.FormattedCode(verbose, s.Init)
		}
		if !s.Address.IsZero() {
			model["address"] = s.Address
		}
		if !s.Author.IsZero() {
			model["author"] = s.Author
		}
		if len(s.RewardType) > 0 {
			model["rewardType"] = s.RewardType
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *TraceAction) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Author
	if err = cache.WriteValue(writer, s.Author); err != nil {
		return err
	}

	// Balance
	if err = cache.WriteValue(writer, &s.Balance); err != nil {
		return err
	}

	// CallType
	if err = cache.WriteValue(writer, s.CallType); err != nil {
		return err
	}

	// From
	if err = cache.WriteValue(writer, s.From); err != nil {
		return err
	}

	// Gas
	if err = cache.WriteValue(writer, s.Gas); err != nil {
		return err
	}

	// Init
	if err = cache.WriteValue(writer, s.Init); err != nil {
		return err
	}

	// Input
	if err = cache.WriteValue(writer, s.Input); err != nil {
		return err
	}

	// RefundAddress
	if err = cache.WriteValue(writer, s.RefundAddress); err != nil {
		return err
	}

	// RewardType
	if err = cache.WriteValue(writer, s.RewardType); err != nil {
		return err
	}

	// SelfDestructed
	if err = cache.WriteValue(writer, s.SelfDestructed); err != nil {
		return err
	}

	// To
	if err = cache.WriteValue(writer, s.To); err != nil {
		return err
	}

	// Value
	if err = cache.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *TraceAction) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = cache.ReadValue(reader, &s.Address, vers); err != nil {
		return err
	}

	// Author
	if err = cache.ReadValue(reader, &s.Author, vers); err != nil {
		return err
	}

	// Balance
	if err = cache.ReadValue(reader, &s.Balance, vers); err != nil {
		return err
	}

	// CallType
	if err = cache.ReadValue(reader, &s.CallType, vers); err != nil {
		return err
	}

	// From
	if err = cache.ReadValue(reader, &s.From, vers); err != nil {
		return err
	}

	// Gas
	if err = cache.ReadValue(reader, &s.Gas, vers); err != nil {
		return err
	}

	// Init
	if err = cache.ReadValue(reader, &s.Init, vers); err != nil {
		return err
	}

	// Input
	if err = cache.ReadValue(reader, &s.Input, vers); err != nil {
		return err
	}

	// RefundAddress
	if err = cache.ReadValue(reader, &s.RefundAddress, vers); err != nil {
		return err
	}

	// RewardType
	if err = cache.ReadValue(reader, &s.RewardType, vers); err != nil {
		return err
	}

	// SelfDestructed
	if err = cache.ReadValue(reader, &s.SelfDestructed, vers); err != nil {
		return err
	}

	// To
	if err = cache.ReadValue(reader, &s.To, vers); err != nil {
		return err
	}

	// Value
	if err = cache.ReadValue(reader, &s.Value, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *TraceAction) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
