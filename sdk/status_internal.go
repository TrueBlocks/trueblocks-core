// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE

	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	status "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type statusOptionsInternal struct {
	Modes       StatusModes `json:"modes,omitempty"`
	Diagnose    bool        `json:"diagnose,omitempty"`
	FirstRecord uint64      `json:"firstRecord,omitempty"`
	MaxRecords  uint64      `json:"maxRecords,omitempty"`
	Chains      bool        `json:"chains,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *statusOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// StatusBytes implements the chifra status command for the SDK.
func (opts *statusOptionsInternal) StatusBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting status struct to URL values: %v", err)
	}

	return status.Status(w, values)
}

// statusParseFunc handles special cases such as structs and enums (if any).
func statusParseFunc(target any, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*statusOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(status): target is not of correct type")
	}

	if key == "modes" {
		var err error
		values := strings.Split(value, ",")
		if opts.Modes, err = enumFromStatusModes(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetStatusOptions returns a filled-in options instance given a string array of arguments.
func GetStatusOptions(args []string) (*statusOptionsInternal, error) {
	var opts statusOptionsInternal
	if err := assignValuesFromArgs(args, statusParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type statusGeneric interface {
	types.Status
}

func queryStatus[T statusGeneric](opts *statusOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.StatusBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		debugPrint(str, result, err)
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *StatusOptions) toInternal() *statusOptionsInternal {
	return &statusOptionsInternal{
		FirstRecord: opts.FirstRecord,
		MaxRecords:  opts.MaxRecords,
		Chains:      opts.Chains,
		Globals:     opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
