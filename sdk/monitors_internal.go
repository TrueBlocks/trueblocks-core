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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	monitors "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type monitorsOptionsInternal struct {
	Addrs     []string `json:"addrs,omitempty"`
	Delete    bool     `json:"delete,omitempty"`
	Undelete  bool     `json:"undelete,omitempty"`
	Remove    bool     `json:"remove,omitempty"`
	Clean     bool     `json:"clean,omitempty"`
	List      bool     `json:"list,omitempty"`
	Watch     bool     `json:"watch,omitempty"`
	Watchlist string   `json:"watchlist,omitempty"`
	Commands  string   `json:"commands,omitempty"`
	BatchSize uint64   `json:"batchSize,omitempty"`
	RunCount  uint64   `json:"runCount,omitempty"`
	Sleep     float64  `json:"sleep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *monitorsOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// MonitorsBytes implements the chifra monitors command for the SDK.
func (opts *monitorsOptionsInternal) MonitorsBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting monitors struct to URL values: %v", err)
	}

	return monitors.Monitors(w, values)
}

// monitorsParseFunc handles special cases such as structs and enums (if any).
func monitorsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*monitorsOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(monitors): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetMonitorsOptions returns a filled-in options instance given a string array of arguments.
func GetMonitorsOptions(args []string) (*monitorsOptionsInternal, error) {
	var opts monitorsOptionsInternal
	if err := assignValuesFromArgs(args, monitorsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type monitorsGeneric interface {
	types.Message |
		types.MonitorClean |
		types.Monitor
}

func queryMonitors[T monitorsGeneric](opts *monitorsOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.MonitorsBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *MonitorsOptions) toInternal() *monitorsOptionsInternal {
	return &monitorsOptionsInternal{
		Addrs:     opts.Addrs,
		Delete:    opts.Delete,
		Undelete:  opts.Undelete,
		Remove:    opts.Remove,
		Watch:     opts.Watch,
		Watchlist: opts.Watchlist,
		Commands:  opts.Commands,
		BatchSize: opts.BatchSize,
		RunCount:  opts.RunCount,
		Sleep:     opts.Sleep,
		Globals:   opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
