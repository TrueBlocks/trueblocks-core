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
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *AbisOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Abis implements the chifra abis command for the SDK.
func (opts *AbisOptions) Abis(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, addr := range opts.Addrs {
		items := strings.Split(addr, " ")
		for _, item := range items {
			values.Add("addrs", item)
		}
	}
	if opts.Known {
		values.Set("known", "true")
	}
	if !opts.ProxyFor.IsZero() {
		values.Set("proxyFor", opts.ProxyFor.Hex())
	}
	for _, find := range opts.Find {
		items := strings.Split(find, " ")
		for _, item := range items {
			values.Add("find", item)
		}
	}
	for _, hint := range opts.Hint {
		items := strings.Split(hint, " ")
		for _, item := range items {
			values.Add("hint", item)
		}
	}
	if opts.Encode != "" {
		values.Set("encode", opts.Encode)
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return abis.Abis(w, values)
}

// GetAbisOptions returns a filled-in options instance given a string array of arguments.
func GetAbisOptions(args []string) (*AbisOptions, error) {
	parseFunc := func(target interface{}, key, value string) (bool, error) {
		opts, ok := target.(*AbisOptions)
		if !ok {
			return false, fmt.Errorf("parseFunc(abis): target is not of correct type")
		}

		var found bool
		switch key {
		case "proxyFor":
			opts.ProxyFor = base.HexToAddress(value)
			return base.IsValidAddress(value), nil
		}

		return found, nil
	}

	var opts AbisOptions
	if err := assignValuesFromArgs(args, parseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE
