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
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
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
			model["ether"] = s.Value.ToFloatStr(18)
		}

		if !s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if asEther {
				model["balanceEth"] = s.Balance.ToFloatStr(18)
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
		items := []namer{
			{addr: s.Address, name: "addressName"},
			{addr: s.Author, name: "authorName"},
			{addr: s.From, name: "fromName"},
			{addr: s.RefundAddress, name: "refundAddressName"},
			{addr: s.SelfDestructed, name: "selfDestructedName"},
			{addr: s.To, name: "toName"},
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

func (s *TraceAction) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Author
	if err = base.WriteValue(writer, s.Author); err != nil {
		return err
	}

	// Balance
	if err = base.WriteValue(writer, &s.Balance); err != nil {
		return err
	}

	// CallType
	if err = base.WriteValue(writer, s.CallType); err != nil {
		return err
	}

	// From
	if err = base.WriteValue(writer, s.From); err != nil {
		return err
	}

	// Gas
	if err = base.WriteValue(writer, s.Gas); err != nil {
		return err
	}

	// Init
	if err = base.WriteValue(writer, s.Init); err != nil {
		return err
	}

	// Input
	if err = base.WriteValue(writer, s.Input); err != nil {
		return err
	}

	// RefundAddress
	if err = base.WriteValue(writer, s.RefundAddress); err != nil {
		return err
	}

	// RewardType
	if err = base.WriteValue(writer, s.RewardType); err != nil {
		return err
	}

	// SelfDestructed
	if err = base.WriteValue(writer, s.SelfDestructed); err != nil {
		return err
	}

	// To
	if err = base.WriteValue(writer, s.To); err != nil {
		return err
	}

	// Value
	if err = base.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *TraceAction) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// Author
	if err = base.ReadValue(reader, &s.Author, fileVersion); err != nil {
		return err
	}

	// Balance
	if err = base.ReadValue(reader, &s.Balance, fileVersion); err != nil {
		return err
	}

	// CallType
	if err = base.ReadValue(reader, &s.CallType, fileVersion); err != nil {
		return err
	}

	// From
	if err = base.ReadValue(reader, &s.From, fileVersion); err != nil {
		return err
	}

	// Gas
	if err = base.ReadValue(reader, &s.Gas, fileVersion); err != nil {
		return err
	}

	// Init
	if err = base.ReadValue(reader, &s.Init, fileVersion); err != nil {
		return err
	}

	// Input
	if err = base.ReadValue(reader, &s.Input, fileVersion); err != nil {
		return err
	}

	// RefundAddress
	if err = base.ReadValue(reader, &s.RefundAddress, fileVersion); err != nil {
		return err
	}

	// RewardType
	if err = base.ReadValue(reader, &s.RewardType, fileVersion); err != nil {
		return err
	}

	// SelfDestructed
	if err = base.ReadValue(reader, &s.SelfDestructed, fileVersion); err != nil {
		return err
	}

	// To
	if err = base.ReadValue(reader, &s.To, fileVersion); err != nil {
		return err
	}

	// Value
	if err = base.ReadValue(reader, &s.Value, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *TraceAction) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
