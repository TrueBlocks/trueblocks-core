// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package chunksPkg

// EXISTING_CODE
import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

type simpleIpfsPin struct {
	Cid        base.IpfsHash `json:"cid"`
	DatePinned string        `json:"datePinned"`
	FileName   string        `json:"fileName"`
	Size       int64         `json:"size"`
	Status     string        `json:"status"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s *simpleIpfsPin) Raw() *types.RawModeler {
	return nil
}

func (s *simpleIpfsPin) Model(chain, format string, verbose bool, extraOptions map[string]any) types.Model {
	var model = map[string]interface{}{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]interface{}{
		"cid":        s.Cid,
		"datePinned": cleanDate(s.DatePinned),
		"status":     s.Status,
		"size":       s.Size,
		"fileName":   s.FileName,
	}
	order = []string{
		"cid",
		"datePinned",
		"status",
		"size",
		"fileName",
	}
	// EXISTING_CODE

	return types.Model{
		Data:  model,
		Order: order,
	}
}

// EXISTING_CODE
func cleanDate(date string) string {
	parts := strings.Split(date, ".")
	return parts[0]
}

// EXISTING_CODE
