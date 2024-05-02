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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

// EXISTING_CODE

type RawState struct {
	AccountType string `json:"accountType"`
	Address     string `json:"address"`
	Balance     string `json:"balance"`
	BlockNumber string `json:"blockNumber"`
	Code        string `json:"code"`
	Deployed    string `json:"deployed"`
	Nonce       string `json:"nonce"`
	Proxy       string `json:"proxy"`
	Timestamp   string `json:"timestamp"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type State struct {
	AccountType string         `json:"accountType"`
	Address     base.Address   `json:"address"`
	Balance     base.Wei       `json:"balance"`
	BlockNumber base.Blknum    `json:"blockNumber"`
	Code        string         `json:"code"`
	Deployed    base.Blknum    `json:"deployed"`
	Nonce       uint64         `json:"nonce"`
	Proxy       base.Address   `json:"proxy"`
	Timestamp   base.Timestamp `json:"timestamp"`
	raw         *RawState      `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s State) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *State) Raw() *RawState {
	return s.raw
}

func (s *State) SetRaw(raw *RawState) {
	s.raw = raw
}

func (s *State) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model["blockNumber"] = s.BlockNumber
	model["address"] = s.Address

	order = []string{"blockNumber", "address"}
	if verbose {
		model["timestamp"] = s.Timestamp
		model["date"] = s.Date()
		order = []string{"blockNumber", "address", "timestamp", "date"}
	}

	if extraOptions != nil {
		if fields, ok := extraOptions["fields"]; ok {
			if fields, ok := fields.([]string); ok {
				for _, field := range fields {
					switch field {
					case "ether":
						model["ether"] = base.FormattedValue(&s.Balance, true, 18)
					case "balance":
						model["balance"] = base.FormattedValue(&s.Balance, false, 18)
					case "nonce":
						model["nonce"] = s.Nonce
					case "code":
						model["code"] = utils.FormattedCode(verbose, s.Code)
					case "proxy":
						model["proxy"] = s.Proxy
					case "deployed":
						if s.Deployed == utils.NOPOS {
							model["deployed"] = ""
						} else {
							model["deployed"] = s.Deployed
						}
					case "accttype":
						model["accttype"] = s.AccountType
					}
				}
				order = append(order, fields...)
			}
		}
	}
	if format == "json" {
		// In JSON format we display both balances
		if _, ok := model["ether"]; !ok {
			model["ether"] = base.FormattedValue(&s.Balance, true, 18)
		}
		if _, ok := model["balance"]; !ok {
			model["balance"] = base.FormattedValue(&s.Balance, false, 18)
		}
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *State) Date() string {
	return utils.FormattedDate(s.Timestamp)
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
// EXISTING_CODE
