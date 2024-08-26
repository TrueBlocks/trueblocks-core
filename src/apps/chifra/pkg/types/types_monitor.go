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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/version"
)

// EXISTING_CODE

type Monitor struct {
	Address     base.Address `json:"address"`
	Deleted     bool         `json:"deleted"`
	FileSize    int64        `json:"fileSize"`
	IsEmpty     bool         `json:"isEmpty"`
	IsStaged    bool         `json:"isStaged"`
	LastScanned uint32       `json:"lastScanned"`
	NRecords    int64        `json:"nRecords"`
	Name        string       `json:"name"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Monitor) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Monitor) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	model = map[string]any{
		"address":  s.Address,
		"nRecords": s.NRecords,
		"fileSize": s.FileSize,
	}
	order = []string{
		"address",
		"nRecords",
		"fileSize",
	}

	if extraOpts["list"] == true {
		model["isEmpty"] = s.IsEmpty
		model["isStaged"] = s.IsStaged
		order = append(order, "isEmpty")
		order = append(order, "isStaged")
	}

	if verbose {
		model["lastScanned"] = s.LastScanned
		model["deleted"] = s.Deleted
		if extraOpts["testMode"] == true {
			model["lastScanned"] = "--lastScanned--"
		}
		order = append(order, "lastScanned")
		order = append(order, "deleted")
	}

	if name, loaded, found := nameAddress(extraOpts, s.Address); found {
		model["addressName"] = name.Name
		order = append(order, "addressName")
	} else if loaded && format != "json" {
		model["addressName"] = ""
		order = append(order, "addressName")
	}
	order = reorderOrdering(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Monitor) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = cache.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// Deleted
	if err = cache.WriteValue(writer, s.Deleted); err != nil {
		return err
	}

	// FileSize
	if err = cache.WriteValue(writer, s.FileSize); err != nil {
		return err
	}

	// IsEmpty
	if err = cache.WriteValue(writer, s.IsEmpty); err != nil {
		return err
	}

	// IsStaged
	if err = cache.WriteValue(writer, s.IsStaged); err != nil {
		return err
	}

	// LastScanned
	if err = cache.WriteValue(writer, s.LastScanned); err != nil {
		return err
	}

	// NRecords
	if err = cache.WriteValue(writer, s.NRecords); err != nil {
		return err
	}

	// Name
	if err = cache.WriteValue(writer, s.Name); err != nil {
		return err
	}

	return nil
}

func (s *Monitor) UnmarshalCache(vers uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = cache.ReadValue(reader, &s.Address, vers); err != nil {
		return err
	}

	// Deleted
	if err = cache.ReadValue(reader, &s.Deleted, vers); err != nil {
		return err
	}

	// FileSize
	if err = cache.ReadValue(reader, &s.FileSize, vers); err != nil {
		return err
	}

	// IsEmpty
	vIsEmpty := version.NewVersion("3.1.2")
	if vers > vIsEmpty.Uint64() {
		// IsEmpty
		if err = cache.ReadValue(reader, &s.IsEmpty, vers); err != nil {
			return err
		}
	}

	// IsStaged
	vIsStaged := version.NewVersion("3.1.2")
	if vers > vIsStaged.Uint64() {
		// IsStaged
		if err = cache.ReadValue(reader, &s.IsStaged, vers); err != nil {
			return err
		}
	}

	// LastScanned
	if err = cache.ReadValue(reader, &s.LastScanned, vers); err != nil {
		return err
	}

	// NRecords
	if err = cache.ReadValue(reader, &s.NRecords, vers); err != nil {
		return err
	}

	// Name
	if err = cache.ReadValue(reader, &s.Name, vers); err != nil {
		return err
	}

	s.FinishUnmarshal()

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Monitor) FinishUnmarshal() {
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
// EXISTING_CODE
