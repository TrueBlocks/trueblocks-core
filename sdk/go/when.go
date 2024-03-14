// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	// EXISTING_CODE
	"encoding/json"
	"fmt"
	"io"
	"log"

	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type WhenOptions struct {
	BlockIds   []string    `json:"blocks,omitempty"`
	List       bool        `json:"list,omitempty"`
	Timestamps bool        `json:"timestamps,omitempty"`
	Count      bool        `json:"count,omitempty"`
	Repair     bool        `json:"repair,omitempty"`
	Check      bool        `json:"check,omitempty"`
	Update     bool        `json:"update,omitempty"`
	Deep       bool        `json:"deep,omitempty"`
	Globals

	// EXISTING_CODE
	// Truncate   base.Blknum `json:"truncate,omitempty"`
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *WhenOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// When implements the chifra when command for the SDK.
func (opts *WhenOptions) When(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting when struct to URL values: %v", err)
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return when.When(w, values)
}

// whenParseFunc handles specail cases such as structs and enums (if any).
func whenParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*WhenOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(when): target is not of correct type")
	}

	// No enums

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetWhenOptions returns a filled-in options instance given a string array of arguments.
func GetWhenOptions(args []string) (*WhenOptions, error) {
	var opts WhenOptions
	if err := assignValuesFromArgs(args, whenParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

