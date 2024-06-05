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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
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
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s State) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *State) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model["blockNumber"] = s.BlockNumber
	model["address"] = s.Address

	order = []string{"blockNumber", "address"}
	if verbose {
		model["timestamp"] = s.Timestamp
		model["date"] = s.Date()
		order = []string{"blockNumber", "address", "timestamp", "date"}
		model["parts"] = s.Parts.String()
		order = append(order, "parts")
	}

	if extraOpts != nil {
		if fields, ok := extraOpts["fields"]; ok {
			if fields, ok := fields.([]string); ok {
				for _, field := range fields {
					switch field {
					case "balance":
						model["balance"] = s.Balance.String()
					case "nonce":
						model["nonce"] = s.Nonce
					case "code":
						model["code"] = utils.FormattedCode(verbose, s.Code)
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
					order = append(order, field)
				}
			}
		}
	}

	if _, ok := model["balance"]; ok {
		model["ether"] = s.Balance.ToEtherStr(18)
		asEther := extraOpts["ether"] == true
		if asEther {
			order = append(order, "ether")
		}
	}

	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *State) Date() string {
	return base.FormattedDate(s.Timestamp)
}

func (s *State) CacheName() string {
	return "State"
}

func (s *State) CacheId() string {
	return fmt.Sprintf("%s-%09d", s.Address.Hex()[2:], s.BlockNumber)
}

func (s *State) CacheLocation() (directory string, extension string) {
	paddedId := s.CacheId()
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]

	subFolder := strings.ToLower(s.CacheName()) + "s"
	directory = filepath.Join(subFolder, filepath.Join(parts...))
	extension = "bin"

	return
}

func (s *State) MarshalCache(writer io.Writer) (err error) {
	// AccountType
	if err = cache.WriteValue(writer, s.AccountType); err != nil {
		return err
	}

	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Balance
	if err = cache.WriteValue(writer, &s.Balance); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.WriteValue(writer, s.BlockNumber); err != nil {
		return err
	}

	// Code
	if err = cache.WriteValue(writer, s.Code); err != nil {
		return err
	}

	// Deployed
	if err = cache.WriteValue(writer, s.Deployed); err != nil {
		return err
	}

	// Nonce
	if err = cache.WriteValue(writer, s.Nonce); err != nil {
		return err
	}

	// Parts
	if err = cache.WriteValue(writer, uint64(s.Parts)); err != nil {
		return err
	}

	// Proxy
	if err = cache.WriteValue(writer, s.Proxy); err != nil {
		return err
	}

	// Timestamp
	if err = cache.WriteValue(writer, s.Timestamp); err != nil {
		return err
	}

	return nil
}

func (s *State) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// AccountType
	if err = cache.ReadValue(reader, &s.AccountType, vers); err != nil {
		return err
	}

	// Address
	if err = cache.ReadValue(reader, &s.Address, vers); err != nil {
		return err
	}

	// Balance
	if err = cache.ReadValue(reader, &s.Balance, vers); err != nil {
		return err
	}

	// BlockNumber
	if err = cache.ReadValue(reader, &s.BlockNumber, vers); err != nil {
		return err
	}

	// Code
	if err = cache.ReadValue(reader, &s.Code, vers); err != nil {
		return err
	}

	// Deployed
	if err = cache.ReadValue(reader, &s.Deployed, vers); err != nil {
		return err
	}

	// Nonce
	if err = cache.ReadValue(reader, &s.Nonce, vers); err != nil {
		return err
	}

	// Parts
	var parts uint64
	if err = cache.ReadValue(reader, &parts, vers); err != nil {
		return err
	}
	s.Parts = StatePart(parts)

	// Proxy
	if err = cache.ReadValue(reader, &s.Proxy, vers); err != nil {
		return err
	}

	// Timestamp
	if err = cache.ReadValue(reader, &s.Timestamp, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *State) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
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
func SliceToStateParts(parts []string) (stateFields StatePart, outputFields []string, none bool) {
	if len(parts) == 0 {
		stateFields = Balance
		outputFields = []string{"balance"}
		return
	}

	for _, part := range parts {
		switch part {
		case "none":
			none = true
			outputFields = nil
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

	outputFields = make([]string, 0, 6)
	if (stateFields & Proxy) != 0 {
		outputFields = append(outputFields, "proxy")
	}

	// Always show balance for non-none parts
	stateFields |= Balance
	outputFields = append(outputFields, "balance")

	if (stateFields & Nonce) != 0 {
		outputFields = append(outputFields, "nonce")
	}
	if (stateFields & Code) != 0 {
		outputFields = append(outputFields, "code")
	}
	if (stateFields & Deployed) != 0 {
		outputFields = append(outputFields, "deployed")
	}
	if (stateFields & Type) != 0 {
		outputFields = append(outputFields, "accttype")
	}

	return
}

// EXISTING_CODE
