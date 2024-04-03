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
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type WhenOptions struct {
	BlockIds   []string    `json:"blocks,omitempty"`
	List       bool        `json:"list,omitempty"`
	Timestamps bool        `json:"timestamps,omitempty"`
	Count      bool        `json:"count,omitempty"`
	Truncate   base.Blknum `json:"truncate,omitempty"`
	Repair     bool        `json:"repair,omitempty"`
	Check      bool        `json:"check,omitempty"`
	Update     bool        `json:"update,omitempty"`
	Deep       bool        `json:"deep,omitempty"`
	Globals
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

	return &opts, nil
}

func (opts *WhenOptions) Query() ([]types.SimpleNamedBlock, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.When(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[types.SimpleNamedBlock]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
