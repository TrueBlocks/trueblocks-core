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
	abis "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type AbisOptions struct {
	Addrs    []string     `json:"addrs,omitempty"`
	Known    bool         `json:"known,omitempty"`
	ProxyFor base.Address `json:"proxyFor,omitempty"`
	Find     []string     `json:"find,omitempty"`
	Hint     []string     `json:"hint,omitempty"`
	Encode   string       `json:"encode,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *AbisOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Abis implements the chifra abis command for the SDK.
func (opts *AbisOptions) Abis(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting abis struct to URL values: %v", err)
	}

	return abis.Abis(w, values)
}

// abisParseFunc handles specail cases such as structs and enums (if any).
func abisParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*AbisOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(abis): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	if key == "proxyFor" {
		opts, _ := target.(*AbisOptions)
		opts.ProxyFor = base.HexToAddress(value)
		return base.IsValidAddress(value), nil
	}
	// EXISTING_CODE

	return found, nil
}

// GetAbisOptions returns a filled-in options instance given a string array of arguments.
func GetAbisOptions(args []string) (*AbisOptions, error) {
	var opts AbisOptions
	if err := assignValuesFromArgs(args, abisParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

func (opts *AbisOptions) Query() ([]bool, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.Abis(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[bool]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// abis-+function

// No enums
// EXISTING_CODE
// EXISTING_CODE
