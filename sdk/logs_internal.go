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
	logs "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type logsOptionsInternal struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Emitter        []string `json:"emitter,omitempty"`
	Topic          []string `json:"topic,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *logsOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// LogsBytes implements the chifra logs command for the SDK.
func (opts *logsOptionsInternal) LogsBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting logs struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	opts.Globals.RenderCtx = &rCtx
	return logs.Logs(rCtx, w, values)
}

// logsParseFunc handles special cases such as structs and enums (if any).
func logsParseFunc(target any, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*logsOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(logs): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetLogsOptions returns a filled-in options instance given a string array of arguments.
func GetLogsOptions(args []string) (*logsOptionsInternal, error) {
	var opts logsOptionsInternal
	if err := assignValuesFromArgs(args, logsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type logsGeneric interface {
	types.Log
}

func queryLogs[T logsGeneric](opts *logsOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.LogsBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	str = convertObjectToArray("inputs", str)
	str = convertObjectToArray("outputs", str)
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
func (opts *LogsOptions) toInternal() *logsOptionsInternal {
	return &logsOptionsInternal{
		TransactionIds: opts.TransactionIds,
		Emitter:        opts.Emitter,
		Topic:          opts.Topic,
		Articulate:     opts.Articulate,
		Globals:        opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
