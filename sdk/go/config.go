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
	"io"
	"net/url"

	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ConfigOptions struct {
	Mode  ConfigMode
	Paths bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *ConfigOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Config implements the chifra config command for the SDK.
func (opts *ConfigOptions) Config(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	if opts.Mode != NoCM1 {
		values.Set("mode", opts.Mode.String())
	}
	if opts.Paths {
		values.Set("paths", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return config.Config(w, values)
}

// GetConfigOptions returns a filled-in options instance given a string array of arguments.
func GetConfigOptions(args []string) (*ConfigOptions, error) {
	var opts ConfigOptions
	if err := assignValuesFromArgs(&opts, &opts.Globals, args); err != nil {
		return nil, err
	}
	return &opts, nil
}

type ConfigMode int

const (
	NoCM1 ConfigMode = iota
	CMShow
	CMEdit
)

func (v ConfigMode) String() string {
	return []string{
		"nocm1",
		"show",
		"edit",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

