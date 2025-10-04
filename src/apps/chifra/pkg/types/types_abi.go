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
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

// EXISTING_CODE

type Abi struct {
	Address        base.Address `json:"address"`
	FileSize       int64        `json:"fileSize"`
	Functions      []Function   `json:"functions"`
	HasConstructor bool         `json:"hasConstructor"`
	HasFallback    bool         `json:"hasFallback"`
	IsEmpty        bool         `json:"isEmpty"`
	IsKnown        bool         `json:"isKnown"`
	LastModDate    string       `json:"lastModDate"`
	NEvents        int64        `json:"nEvents"`
	NFunctions     int64        `json:"nFunctions"`
	Name           string       `json:"name"`
	Path           string       `json:"path"`
	// EXISTING_CODE
	// EXISTING_CODE
}

func (s Abi) String() string {
	bytes, _ := json.Marshal(s)
	return string(bytes)
}

func (s *Abi) Model(chain, format string, verbose bool, extraOpts map[string]any) Model {
	_ = chain
	_ = format
	_ = verbose
	_ = extraOpts
	var model = map[string]any{}
	var order = []string{}

	// EXISTING_CODE
	if extraOpts["list"] == true {
		model = map[string]any{
			"address":     s.Address,
			"name":        s.Name,
			"lastModDate": s.LastModDate,
			"fileSize":    s.FileSize,
			"isKnown":     s.IsKnown,
		}
		order = []string{
			"address",
			"name",
			"lastModDate",
			"fileSize",
			"isKnown",
		}

		if s.IsKnown {
			model["address"] = ""
		} else {
			if name, loaded, found := labelAddress(extraOpts, s.Address); found {
				model["name"] = name.Name
			} else if loaded {
				model["name"] = ""
			}
		}
		if format == "json" && s.Address.IsZero() {
			delete(model, "address")
		}

		if verbose {
			if format == "json" {
				if s.IsEmpty {
					model["isEmpty"] = s.IsEmpty
				}
			} else {
				model["isEmpty"] = s.IsEmpty
				order = append(order, "isEmpty")
			}
			model["nFunctions"] = s.NFunctions
			order = append(order, "nFunctions")
			model["nEvents"] = s.NEvents
			order = append(order, "nEvents")
			if format == "json" {
				if s.HasConstructor {
					model["hasConstructor"] = s.HasConstructor
				}
				if s.HasFallback {
					model["hasFallback"] = s.HasFallback
				}
			} else {
				model["hasConstructor"] = s.HasConstructor
				order = append(order, "hasConstructor")
				model["hasFallback"] = s.HasFallback
				order = append(order, "hasFallback")
			}
			model["path"] = s.Path
			order = append(order, "path")
		}

	} else {
		model[s.Address.Hex()] = s.Functions
		order = append(order, s.Address.Hex())
		if name, loaded, found := labelAddress(extraOpts, s.Address); found {
			model["addressName"] = name.Name
			order = append(order, "addressName")
		} else if loaded && format != "json" {
			model["addressName"] = ""
			order = append(order, "addressName")
		}
	}
	order = reorderFields(order)
	// EXISTING_CODE

	return Model{
		Data:  model,
		Order: order,
	}
}

func (s *Abi) CacheLocations() (string, string, string) {
	paddedId := s.Address.Hex()[2:]
	parts := make([]string, 3)
	parts[0] = paddedId[:2]
	parts[1] = paddedId[2:4]
	parts[2] = paddedId[4:6]
	subFolder := strings.ToLower("Abi") + "s"
	directory := filepath.Join(subFolder, filepath.Join(parts...))
	return directory, paddedId, "bin"
}

func (s *Abi) MarshalCache(writer io.Writer) (err error) {
	// Address
	if err = base.WriteValue(writer, s.Address); err != nil {
		return err
	}

	// FileSize
	if err = base.WriteValue(writer, s.FileSize); err != nil {
		return err
	}

	// Functions
	functions := make([]base.Marshaler, 0, len(s.Functions))
	for _, function := range s.Functions {
		functions = append(functions, &function)
	}
	if err = base.WriteValue(writer, functions); err != nil {
		return err
	}

	// HasConstructor
	if err = base.WriteValue(writer, s.HasConstructor); err != nil {
		return err
	}

	// HasFallback
	if err = base.WriteValue(writer, s.HasFallback); err != nil {
		return err
	}

	// IsEmpty
	if err = base.WriteValue(writer, s.IsEmpty); err != nil {
		return err
	}

	// IsKnown
	if err = base.WriteValue(writer, s.IsKnown); err != nil {
		return err
	}

	// LastModDate
	if err = base.WriteValue(writer, s.LastModDate); err != nil {
		return err
	}

	// NEvents
	if err = base.WriteValue(writer, s.NEvents); err != nil {
		return err
	}

	// NFunctions
	if err = base.WriteValue(writer, s.NFunctions); err != nil {
		return err
	}

	// Name
	if err = base.WriteValue(writer, s.Name); err != nil {
		return err
	}

	// Path
	if err = base.WriteValue(writer, s.Path); err != nil {
		return err
	}

	return nil
}

func (s *Abi) UnmarshalCache(fileVersion uint64, reader io.Reader) (err error) {
	// Check for compatibility and return cache.ErrIncompatibleVersion to invalidate this item (see #3638)
	// EXISTING_CODE
	// EXISTING_CODE

	// Address
	if err = base.ReadValue(reader, &s.Address, fileVersion); err != nil {
		return err
	}

	// FileSize
	if err = base.ReadValue(reader, &s.FileSize, fileVersion); err != nil {
		return err
	}

	// Functions
	s.Functions = make([]Function, 0)
	if err = base.ReadValue(reader, &s.Functions, fileVersion); err != nil {
		return err
	}

	// HasConstructor
	if err = base.ReadValue(reader, &s.HasConstructor, fileVersion); err != nil {
		return err
	}

	// HasFallback
	if err = base.ReadValue(reader, &s.HasFallback, fileVersion); err != nil {
		return err
	}

	// IsEmpty
	if err = base.ReadValue(reader, &s.IsEmpty, fileVersion); err != nil {
		return err
	}

	// IsKnown
	if err = base.ReadValue(reader, &s.IsKnown, fileVersion); err != nil {
		return err
	}

	// LastModDate
	if err = base.ReadValue(reader, &s.LastModDate, fileVersion); err != nil {
		return err
	}

	// NEvents
	if err = base.ReadValue(reader, &s.NEvents, fileVersion); err != nil {
		return err
	}

	// NFunctions
	if err = base.ReadValue(reader, &s.NFunctions, fileVersion); err != nil {
		return err
	}

	// Name
	if err = base.ReadValue(reader, &s.Name, fileVersion); err != nil {
		return err
	}

	// Path
	if err = base.ReadValue(reader, &s.Path, fileVersion); err != nil {
		return err
	}

	s.FinishUnmarshal(fileVersion)

	return nil
}

// FinishUnmarshal is used by the cache. It may be unused depending on auto-code-gen
func (s *Abi) FinishUnmarshal(fileVersion uint64) {
	_ = fileVersion
	// EXISTING_CODE
	// EXISTING_CODE
}

// EXISTING_CODE
func (s *Abi) ShallowCopy() Abi {
	return *s
}

// EXISTING_CODE
