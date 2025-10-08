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
	"io"
	"path/filepath"
	"sort"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type State struct {
	AccountType string         `json:"accountType"`
	Address     base.Address   `json:"address"`
	Balance     base.Wei       `json:"balance"`
	BlockNumber base.Blknum    `json:"blockNumber"`
	Code        string         `json:"code"`
	Deployed    base.Blknum    `json:"deployed"`
	Nonce       base.Value     `json:"nonce"`
	Parts       StatePart      `json:"parts"`
	Proxy       base.Address   `json:"proxy"`
	Timestamp   base.Timestamp `json:"timestamp"`
	Calcs       *StateCalcs    `json:"calcs,omitempty"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s State) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *State) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	props := NewModelProps(chain, format, verbose, extraOpts)

	rawNames := []Labeler{
		NewLabeler(s.Address, "address"),
		NewLabeler(s.Proxy, "proxy"),
	}
	model := s.RawMap(props, &rawNames)
	for k, v := range s.CalcMap(props) {
		model[k] = v
	}

	var order = []string{}
	// EXISTING_CODE
	order = []string{"blockNumber", "address"}
	if verbose {
		if s.Timestamp > 0 {
			order = []string{"blockNumber", "address", "timestamp", "date"}
		}
	}

	if extraOpts != nil {
		if fields, ok := extraOpts["outFields"]; ok {
			if fields, ok := fields.([]string); ok {
				order = append(order, fields...)
			}
		}
	}

	if _, ok := model["balance"]; ok {
		asEther := extraOpts["ether"] == true
		if asEther {
			order = append(order, "ether")
		}
	}
	// EXISTING_CODE

	for _, item := range rawNames {
		key := item.name + "Name"
		if _, exists := model[key]; exists {
			order = append(order, key)
		}
	}
	order = reorderFields(order)

	return Model{
		Data:  model,
		Order: order,
	}
}

// RawMap returns a map containing only the raw/base fields for this State.
func (s *State) RawMap(p *ModelProps, needed *[]Labeler) map[string]any {
	model := map[string]any{
		// EXISTING_CODE
		"blockNumber": s.BlockNumber,
		"address":     s.Address,
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		if s.Timestamp > 0 {
			model["timestamp"] = s.Timestamp
		}
	}
	// EXISTING_CODE

	return labelAddresses(p, model, needed)
}

// CalcMap returns a map containing the calculated/derived fields for this type.
func (s *State) CalcMap(p *ModelProps) map[string]any {
	_ = p // delint
	model := map[string]any{
		// EXISTING_CODE
		// EXISTING_CODE
	}

	// EXISTING_CODE
	if p.Verbose {
		if s.Timestamp > 0 {
			model["date"] = s.Date()
		}
	}

	if p.ExtraOpts != nil {
		if fields, ok := p.ExtraOpts["outFields"]; ok {
			if fields, ok := fields.([]string); ok {
				for _, field := range fields {
					switch field {
					case "balance":
						model["balance"] = s.Balance.String()
					case "nonce":
						model["nonce"] = s.Nonce
					case "code":
						model["code"] = utils.FormattedCode(p.Verbose, s.Code)
					case "proxy":
						model["proxy"] = s.Proxy
					case "deployed":
						if s.Deployed == base.NOPOSN {
							model["deployed"] = ""
						} else {
							model["deployed"] = s.Deployed
						}
					case "accttype":
						model["accttype"] = s.AccountType
					}
				}
			}
		}
	}

	if _, ok := model["balance"]; ok {
		model["ether"] = s.Balance.ToFloatString(18)
	}
	// EXISTING_CODE

	return model
}

func (s *State) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *State) CacheLocations() (string, string, string) {
	paddedId := fmt.Sprintf("%s-%09d", s.Address.Hex()[2:], s.BlockNumber)
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("State") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *State) MarshalCache(writer io.Writer) (err error) {
	// AccountType
	if err = base.WriteValue(writer, s.AccountType); err != nil {
		return err
	}

	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Balance
	if err = base.WriteValue(writer, &s.Balance); err != nil {
		return err
	}

	// BlockNumber
	if err = base.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Code
	if err = base.WriteValue(writer, s.Code); err != nil {
		return err
	}

	// Deployed
	if err = base.WriteValue(writer, s.Deployed); err != nil {
		return err
	}

	// Nonce
	if err = base.WriteValue(writer, s.Nonce); err != nil {
		return err
	}

	// Parts
	if err = base.WriteValue(writer, uint64(s.Parts)); err != nil {
		return err
	}

	// Proxy
	if err = base.WriteValue(writer, s.Proxy); err != nil {
		return err
	}

	// Timestamp
	if err = base.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	return nil
}

func (s *State) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// AccountType
	if err = base.ReadValue(reader, &s.AccountType, fileVersion); err != nil {
		return err
	}

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// Balance
	if err = base.ReadValue(reader, &s.Balance, fileVersion); err != nil {
		return err
	}

	// BlockNumber
	if err = base.ReadValue(reader, &s.BlockNumber, fileVersion); err != nil {
		return err
	}

	// Code
	if err = base.ReadValue(reader, &s.Code, fileVersion); err != nil {
		return err
	}

	// Deployed
	if err = base.ReadValue(reader, &s.Deployed, fileVersion); err != nil {
		return err
	}

	// Nonce
	if err = base.ReadValue(reader, &s.Nonce, fileVersion); err != nil {
		return err
	}

	// Parts
	var parts uint64
	if err = base.ReadValue(reader, &parts, fileVersion); err != nil {
		return err
	}
	s.Parts = StatePart(parts)

	// Proxy
	if err = base.ReadValue(reader, &s.Proxy, fileVersion); err != nil {
		return err
	}

	// Timestamp
	if err = base.ReadValue(reader, &s.Timestamp, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *State) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	s.Calcs = nil
	// EXISTING_CODE
	// EXISTING_CODE
}

// StateCalcs holds lazy-loaded calculated fields for State
type StateCalcs struct {
	// EXISTING_CODE
	Date string `json:"date,omitempty"`
	// EXISTING_CODE
}

func (s *State) EnsureCalcs(p *ModelProps, fieldFilter []string) error {
	_ = fieldFilter // delint
	if s.Calcs != nil {
		return nil
	}

	calcMap := s.CalcMap(p)
	if len(calcMap) == 0 {
		return nil
	}

	jsonBytes, err := json.Marshal(calcMap)
	if err != nil {
		return err
	}

	s.Calcs = &StateCalcs{}
	return json.Unmarshal(jsonBytes, s.Calcs)
}

// EXISTING_CODE
// StatePart is a bit mask for querying various parts an address's state
type StatePart int

const (
	Balance StatePart = 1 << iota
	Nonce
	Code
	Deployed
	Proxy
	Type
)

func (s StatePart) String() string {
	m := map[StatePart]string{
		Balance:  "balance",
		Nonce:    "nonce",
		Code:     "code",
		Deployed: "deployed",
		Proxy:    "proxy",
		Type:     "accttype",
	}
	ret := []string{}
	for k, v := range m {
		if (s & k) != 0 {
			ret = append(ret, v)
		}
	}
	sort.Strings(ret)
	return strings.Join(ret, ",")
}

// SliceToStateParts converts a string array of part names to a bit mask of parts and returns the corresponding output field names or none if no valid parts are present
func SliceToStateParts(parts []string) (stateFields StatePart, outFields []string, none bool) {
	if len(parts) == 0 {
		stateFields = Balance
		outFields = []string{"balance"}
		return
	}

	for _, part := range parts {
		switch part {
		case "none":
			none = true
			outFields = nil
			return
		case "some":
			stateFields |= Balance | Nonce | Code | Type
		case "all":
			stateFields |= Balance | Nonce | Code | Proxy | Deployed | Type
		case "balance":
			stateFields |= Balance
		case "nonce":
			stateFields |= Nonce
		case "code":
			stateFields |= Code
		case "proxy":
			stateFields |= Proxy
		case "deployed":
			stateFields |= Deployed
		case "accttype":
			stateFields |= Type
		}
	}

	outFields = make([]string, 0, 6)
	if (stateFields & Proxy) != 0 {
		outFields = append(outFields, "proxy")
	}

	// Always show balance for non-none parts
	stateFields |= Balance
	outFields = append(outFields, "balance")

	if (stateFields & Nonce) != 0 {
		outFields = append(outFields, "nonce")
	}
	if (stateFields & Code) != 0 {
		outFields = append(outFields, "code")
	}
	if (stateFields & Deployed) != 0 {
		outFields = append(outFields, "deployed")
	}
	if (stateFields & Type) != 0 {
		outFields = append(outFields, "accttype")
	}

	return
}

// EXISTING_CODE
