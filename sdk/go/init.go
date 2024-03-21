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
	"encoding/json"
	"fmt"
	"io"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type InitOptions struct {
	All        bool         `json:"all,omitempty"`
	DryRun     bool         `json:"dryRun,omitempty"`
	Publisher  base.Address `json:"publisher,omitempty"`
	FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	Sleep      float64      `json:"sleep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *InitOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Init implements the chifra init command for the SDK.
func (opts *InitOptions) Init(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting init struct to URL values: %v", err)
	}

	return initPkg.Init(w, values)
}

// initParseFunc handles specail cases such as structs and enums (if any).
func initParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*InitOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(init): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetInitOptions returns a filled-in options instance given a string array of arguments.
func GetInitOptions(args []string) (*InitOptions, error) {
	var opts InitOptions
	if err := assignValuesFromArgs(args, initParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
