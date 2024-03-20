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
	// EXISTING_CODE
)

type {{.Proper}}Options struct {
{{.SdkFields}}}

// String implements the stringer interface
func (opts *{{.Proper}}Options) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// {{.Proper}} implements the chifra {{.Lower}} command for the SDK.
func (opts *{{.Proper}}Options) {{.Proper}}(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting {{.Lower}} struct to URL values: %v", err)
	}

	return {{.Pkg}}.{{.Proper}}(w, values)
}

// {{.Lower}}ParseFunc handles specail cases such as structs and enums (if any).
func {{.Lower}}ParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	{{.Enums3}}, ok := target.(*{{.Proper}}Options)
	if !ok {
		return false, fmt.Errorf("parseFunc({{.Lower}}): target is not of correct type")
	}

{{.Enums1}}
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// Get{{.Proper}}Options returns a filled-in options instance given a string array of arguments.
func Get{{.Proper}}Options(args []string) (*{{.Proper}}Options, error) {
	var opts {{.Proper}}Options
	if err := assignValuesFromArgs(args, {{.Lower}}ParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

{{.Enums2}}
// EXISTING_CODE
// EXISTING_CODE
