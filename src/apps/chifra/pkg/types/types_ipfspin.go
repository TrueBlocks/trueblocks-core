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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type RawIpfsPin struct {
	Cid        string `json:"cid"`
	DatePinned string `json:"datePinned"`
	FileName   string `json:"fileName"`
	Size       string `json:"size"`
	Status     string `json:"status"`
	// EXISTING_CODE
	// EXISTING_CODE
}

type IpfsPin struct {
	Cid        base.IpfsHash `json:"cid"`
	DatePinned string        `json:"datePinned"`
	FileName   string        `json:"fileName"`
	Size       int64         `json:"size"`
	Status     string        `json:"status"`
	raw        *RawIpfsPin   `json:"-"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s IpfsPin) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *IpfsPin) Raw() *RawIpfsPin {
	return s.raw
}

func (s *IpfsPin) SetRaw(raw *RawIpfsPin) {
	s.raw = raw
}

func (s *IpfsPin) Model(chain, format string, verbose bool, extraOptions map[string]any) Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"fileName":   s.FileName,
		"cid":        s.Cid,
		"datePinned": cleanDate(s.DatePinned),
		"status":     s.Status,
		"size":       s.Size,
	}
	order = []string{
		"fileName",
		"cid",
		"datePinned",
		"status",
		"size",
	}
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *IpfsPin) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func cleanDate(date string) string {
	parts := strings.Split(date, ".")
	return parts[0]
}

// EXISTING_CODE
