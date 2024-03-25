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
	"io"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawTraceAction struct {
	Address        string `json:"address"`
	Author         string `json:"author"`
	Balance        string `json:"balance"`
	CallType       string `json:"callType"`
	From           string `json:"from"`
	Gas            string `json:"gas"`
	Init           string `json:"init"`
	Input          string `json:"input"`
	RefundAddress  string `json:"refundAddress"`
	RewardType     string `json:"rewardType"`
	SelfDestructed string `json:"selfDestructed"`
	To             string `json:"to"`
	Value          string `json:"value"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type SimpleTraceAction struct {
	Address        base.Address    `json:"address,omitempty"`
	Author         base.Address    `json:"author,omitempty"`
	Balance        base.Wei        `json:"balance,omitempty"`
	CallType       string          `json:"callType"`
	From           base.Address    `json:"from"`
	Gas            base.Gas        `json:"gas"`
	Init           string          `json:"init,omitempty"`
	Input          string          `json:"input,omitempty"`
	RefundAddress  base.Address    `json:"refundAddress,omitempty"`
	RewardType     string          `json:"rewardType,omitempty"`
	SelfDestructed base.Address    `json:"selfDestructed,omitempty"`
	To             base.Address    `json:"to"`
	Value          base.Wei        `json:"value"`
	raw            *RawTraceAction `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *SimpleTraceAction) Raw() *RawTraceAction {
	return s.raw
}

func (s *SimpleTraceAction) SetRaw(raw *RawTraceAction) {
	s.raw = raw
}

func (s *SimpleTraceAction) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	asEther := extraOptions["ether"] == true
	if format == "json" {
		if extraOptions["traces"] != true && len(s.Init) > 0 {
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
		if s.Value.String() != "0" {
			model["value"] = base.FormattedValue(&s.Value, asEther, 18)
		}
		if !s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if s.Value.String() != "0" {
				model["value"] = base.FormattedValue(&s.Balance, asEther, 18)
			}
		} else {
			if s.To.IsZero() {
				model["to"] = "0x0"
			} else {
				model["to"] = s.To
			}
			model["value"] = base.FormattedValue(&s.Value, asEther, 18)
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

// --> marshal_only
func (s *SimpleTraceAction) MarshalCache(writer io.Writer) (err error) {
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

func (s *SimpleTraceAction) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	// Address
	if err = cache.ReadValue(reader, &s.Address, version); err != nil {
		return err
	}

	// Author
	if err = cache.ReadValue(reader, &s.Author, version); err != nil {
		return err
	}

	// Balance
	if err = cache.ReadValue(reader, &s.Balance, version); err != nil {
		return err
	}

	// CallType
	if err = cache.ReadValue(reader, &s.CallType, version); err != nil {
		return err
	}

	// From
	if err = cache.ReadValue(reader, &s.From, version); err != nil {
		return err
	}

	// Gas
	if err = cache.ReadValue(reader, &s.Gas, version); err != nil {
		return err
	}

	// Init
	if err = cache.ReadValue(reader, &s.Init, version); err != nil {
		return err
	}

	// Input
	if err = cache.ReadValue(reader, &s.Input, version); err != nil {
		return err
	}

	// RefundAddress
	if err = cache.ReadValue(reader, &s.RefundAddress, version); err != nil {
		return err
	}

	// RewardType
	if err = cache.ReadValue(reader, &s.RewardType, version); err != nil {
		return err
	}

	// SelfDestructed
	if err = cache.ReadValue(reader, &s.SelfDestructed, version); err != nil {
		return err
	}

	// To
	if err = cache.ReadValue(reader, &s.To, version); err != nil {
		return err
	}

	// Value
	if err = cache.ReadValue(reader, &s.Value, version); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

func (s *SimpleTraceAction) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE

