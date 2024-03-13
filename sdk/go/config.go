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

	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ConfigOptions struct {
	Mode  ConfigMode `json:"mode,omitempty"`
	Paths bool       `json:"paths,omitempty"`
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
	if opts.Mode.String() == "edit" {
		return fmt.Errorf("edit mode not implemented in sdk")
	}
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

// configParseFunc handles specail cases such as structs and enums (if any).
func configParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	// EXISTING_CODE
	// EXISTING_CODE
	return found, nil
}

// GetConfigOptions returns a filled-in options instance given a string array of arguments.
func GetConfigOptions(args []string) (*ConfigOptions, error) {
	var opts ConfigOptions
	if err := assignValuesFromArgs(args, configParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

type ConfigMode int

const (
	NoCM1  ConfigMode = 0
	CMShow            = 1 << iota
	CMEdit
)

func (v ConfigMode) String() string {
	switch v {
	case NoCM1:
		return "none"
	}

	var m = map[ConfigMode]string{
		CMShow: "show",
		CMEdit: "edit",
	}

	var ret []string
	for _, val := range []ConfigMode{CMShow, CMEdit} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

// EXISTING_CODE
// EXISTING_CODE
