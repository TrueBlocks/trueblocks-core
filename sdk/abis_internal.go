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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	abis "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type abisOptionsInternal struct {
	Addrs    []string     `json:"addrs,omitempty"`
	Known    bool         `json:"known,omitempty"`
	ProxyFor base.Address `json:"proxyFor,omitempty"`
	Find     []string     `json:"find,omitempty"`
	Hint     []string     `json:"hint,omitempty"`
	Encode   string       `json:"encode,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *abisOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// AbisBytes implements the chifra abis command for the SDK.
func (opts *abisOptionsInternal) AbisBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting abis struct to URL values: %v", err)
	}

	return abis.Abis(w, values)
}

// abisParseFunc handles special cases such as structs and enums (if any).
func abisParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*abisOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(abis): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	if key == "proxyFor" {
		opts, _ := target.(*abisOptionsInternal)
		opts.ProxyFor = base.HexToAddress(value)
		return base.IsValidAddress(value), nil
	}
	// EXISTING_CODE

	return found, nil
}

// GetAbisOptions returns a filled-in options instance given a string array of arguments.
func GetAbisOptions(args []string) (*abisOptionsInternal, error) {
	var opts abisOptionsInternal
	if err := assignValuesFromArgs(args, abisParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type abisGeneric interface {
	types.Function
}

func queryAbis[T abisGeneric](opts *abisOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.AbisBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *AbisOptions) toInternal() *abisOptionsInternal {
	return &abisOptionsInternal{
		Addrs:    opts.Addrs,
		Known:    opts.Known,
		ProxyFor: opts.ProxyFor,
		Find:     opts.Find,
		Hint:     opts.Hint,
		Encode:   opts.Encode,
		Globals:  opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
