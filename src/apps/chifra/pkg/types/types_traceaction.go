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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
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

func (s *SimpleTraceAction) Model(verbose bool, format string, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
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
			model["value"] = s.Value.String()
		}
		if !s.RefundAddress.IsZero() {
			model["refundAddress"] = s.RefundAddress
			model["balance"] = s.Balance.String()
			if s.Value.String() != "0" {
				model["value"] = s.Balance.String()
			}
		} else {
			if s.To.IsZero() {
				model["to"] = "0x0"
			} else {
				model["to"] = s.To
			}
			model["value"] = s.Value.String()
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

func (s *SimpleTraceAction) WriteTo(w io.Writer) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

func (s *SimpleTraceAction) ReadFrom(r io.Reader) (n int64, err error) {
	// EXISTING_CODE
	// EXISTING_CODE
	return 0, nil
}

// EXISTING_CODE
//

// MarshalCache writes the object to the cache.
func (s *SimpleTraceAction) MarshalCache(writer io.Writer) (err error) {
	if err = cacheNew.WriteValue(writer, s.Address); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Author); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, &s.Balance); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.CallType); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.From); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Gas); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Init); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.Input); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.RefundAddress); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.RewardType); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.SelfDestructed); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, s.To); err != nil {
		return err
	}
	if err = cacheNew.WriteValue(writer, &s.Value); err != nil {
		return err
	}

	return nil
}

func (s *SimpleTraceAction) UnmarshalCache(version uint64, reader io.Reader) (err error) {
	if err = cacheNew.ReadValue(reader, &s.Address, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Author, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Balance, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.CallType, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.From, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Gas, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Init, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Input, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.RefundAddress, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.RewardType, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.SelfDestructed, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.To, version); err != nil {
		return err
	}
	if err = cacheNew.ReadValue(reader, &s.Value, version); err != nil {
		return err
	}

	return
}

// EXISTING_CODE
