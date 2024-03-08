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
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	abis "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type AbisOptions struct {
	Addrs    []string // allow for ENS names and addresses
	Known    bool
	ProxyFor base.Address
	Find     []string
	Hint     []string
	Encode   string
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Abis implements the chifra abis command for the SDK.
func (opts *AbisOptions) Abis(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, addr := range opts.Addrs {
		values.Add("addrs", addr)
	}
	if opts.Known {
		values.Set("known", "true")
	}
	if !opts.ProxyFor.IsZero() {
		values.Set("proxy_for", opts.ProxyFor.Hex())
	}
	for _, find := range opts.Find {
		values.Add("find", find)
	}
	for _, hint := range opts.Hint {
		values.Add("hint", hint)
	}
	if opts.Encode != "" {
		values.Set("encode", opts.Encode)
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return abis.Abis(w, values)
}

// GetAbisOptions returns an options instance given a string array of arguments.
func GetAbisOptions(args []string) (*AbisOptions, error) {
	var opts AbisOptions
	err := assignValuesFromArgs(&opts, &opts.Globals, args)
	logger.Info("Args:", args)
	logger.Info("Opts:", opts.String())
	if err != nil {
		return nil, err
	}

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE
