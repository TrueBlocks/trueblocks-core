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
	logs "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type LogsOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Emitter        []string `json:"emitter,omitempty"`
	Topic          []string `json:"topic,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *LogsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Logs implements the chifra logs command for the SDK.
func (opts *LogsOptions) Logs(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting logs struct to URL values: %v", err)
	}

	return logs.Logs(w, values)
}

// logsParseFunc handles special cases such as structs and enums (if any).
func logsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*LogsOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(logs): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetLogsOptions returns a filled-in options instance given a string array of arguments.
func GetLogsOptions(args []string) (*LogsOptions, error) {
	var opts LogsOptions
	if err := assignValuesFromArgs(args, logsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

func (opts *LogsOptions) Query() ([]types.SimpleLog, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.Logs(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[types.SimpleLog]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
