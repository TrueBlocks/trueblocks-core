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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	config "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ConfigOptions struct {
	Mode  ConfigMode `json:"mode,omitempty"`
	Paths bool       `json:"paths,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *ConfigOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ConfigBytes implements the chifra config command for the SDK.
func (opts *ConfigOptions) ConfigBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting config struct to URL values: %v", err)
	}

	return config.Config(w, values)
}

// configParseFunc handles special cases such as structs and enums (if any).
func configParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*ConfigOptions)
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
func GetConfigOptions(args []string) (*ConfigOptions, error) {
	var opts ConfigOptions
	if err := assignValuesFromArgs(args, configParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type configGeneric interface {
	bool
}

func queryConfig[T configGeneric](opts *ConfigOptions) ([]T, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.ConfigBytes(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[T]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// Config implements the chifra config command for the SDK.
func (opts *ConfigOptions) Config() ([]bool, *rpc.MetaData, error) {
	return queryConfig[bool](opts)
}

type ConfigMode int

const (
	NoCOM  ConfigMode = 0
	CMShow            = 1 << iota
	CMEdit
)

func (v ConfigMode) String() string {
	switch v {
	case NoCOM:
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

func enumFromConfigMode(values []string) (ConfigMode, error) {
	if len(values) == 0 {
		return NoCOM, fmt.Errorf("no value provided for mode option")
	}

	var result ConfigMode
	for _, val := range values {
		switch val {
		case "show":
			result |= CMShow
		case "edit":
			result |= CMEdit
		default:
			return NoCOM, fmt.Errorf("unknown mode: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
