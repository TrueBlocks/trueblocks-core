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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	traces "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type tracesOptionsInternal struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Filter         string   `json:"filter,omitempty"`
	Count          bool     `json:"count,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *tracesOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// TracesBytes implements the chifra traces command for the SDK.
func (opts *tracesOptionsInternal) TracesBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting traces struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	return traces.Traces(rCtx, w, values)
}

// tracesParseFunc handles special cases such as structs and enums (if any).
func tracesParseFunc(target any, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*tracesOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(traces): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetTracesOptions returns a filled-in options instance given a string array of arguments.
func GetTracesOptions(args []string) (*tracesOptionsInternal, error) {
	var opts tracesOptionsInternal
	if err := assignValuesFromArgs(args, tracesParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type tracesGeneric interface {
	types.Trace |
		types.TraceCount
}

func queryTraces[T tracesGeneric](opts *tracesOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.TracesBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	str = convertObjectToArray("inputs", str)
	str = convertObjectToArray("outputs", str)
	str = convertEmptyStrToZero("balance", str)
	str = convertEmptyStrToZero("value", str)
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
func (opts *TracesOptions) toInternal() *tracesOptionsInternal {
	return &tracesOptionsInternal{
		TransactionIds: opts.TransactionIds,
		Articulate:     opts.Articulate,
		Filter:         opts.Filter,
		Globals:        opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
