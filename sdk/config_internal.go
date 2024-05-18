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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type configOptionsInternal struct {
	Mode  ConfigMode `json:"mode,omitempty"`
	Paths bool       `json:"paths,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *configOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ConfigBytes implements the chifra config command for the SDK.
func (opts *configOptionsInternal) ConfigBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting config struct to URL values: %v", err)
	}

	return config.Config(w, values)
}

// configParseFunc handles special cases such as structs and enums (if any).
func configParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*configOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(config): target is not of correct type")
	}

	if key == "mode" {
		var err error
		values := strings.Split(value, ",")
		if opts.Mode, err = enumFromConfigMode(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetConfigOptions returns a filled-in options instance given a string array of arguments.
func GetConfigOptions(args []string) (*configOptionsInternal, error) {
	var opts configOptionsInternal
	if err := assignValuesFromArgs(args, configParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type configGeneric interface {
	types.CacheItem
}

func queryConfig[T configGeneric](opts *configOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.ConfigBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		debugPrint(str, result, err)
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *ConfigOptions) toInternal() *configOptionsInternal {
	return &configOptionsInternal{
		Mode:    opts.Mode,
		Globals: opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
