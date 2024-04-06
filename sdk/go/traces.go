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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type TracesOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Filter         string   `json:"filter,omitempty"`
	Count          bool     `json:"count,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *TracesOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Traces implements the chifra traces command for the SDK.
func (opts *TracesOptions) Traces(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting traces struct to URL values: %v", err)
	}

	return traces.Traces(w, values)
}

// tracesParseFunc handles special cases such as structs and enums (if any).
func tracesParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*TracesOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(traces): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetTracesOptions returns a filled-in options instance given a string array of arguments.
func GetTracesOptions(args []string) (*TracesOptions, error) {
	var opts TracesOptions
	if err := assignValuesFromArgs(args, tracesParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

func (opts *TracesOptions) Query() ([]types.SimpleTrace, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.Traces(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[types.SimpleTrace]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// traces-+trace|traces-count+tracecount

// No enums
// EXISTING_CODE
// EXISTING_CODE
