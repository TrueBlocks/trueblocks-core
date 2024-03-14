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

	monitors "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type MonitorsOptions struct {
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
	Sleep     float64  `json:"sleep,omitempty"`
	Globals

	// EXISTING_CODE
	// RunCount  uint64   `json:"runCount,omitempty"`
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *MonitorsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Monitors implements the chifra monitors command for the SDK.
func (opts *MonitorsOptions) Monitors(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Addrs {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("addrs", item)
		}
	}
	if opts.Delete {
		values.Set("delete", "true")
	}
	if opts.Undelete {
		values.Set("undelete", "true")
	}
	if opts.Remove {
		values.Set("remove", "true")
	}
	if opts.Clean {
		values.Set("clean", "true")
	}
	if opts.List {
		values.Set("list", "true")
	}
	if opts.Watch {
		values.Set("watch", "true")
	}
	if opts.Watchlist != "" {
		values.Set("watchlist", opts.Watchlist)
	}
	if opts.Commands != "" {
		values.Set("commands", opts.Commands)
	}
	if opts.BatchSize > 0 {
		values.Set("batch_size", fmt.Sprint(opts.BatchSize))
	}
	if opts.Sleep > 0 {
		values.Set("sleep", fmt.Sprint(opts.Sleep))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return monitors.Monitors(w, values)
}

// monitorsParseFunc handles specail cases such as structs and enums (if any).
func monitorsParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*MonitorsOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(monitors): target is not of correct type")
	}

	// No enums

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetMonitorsOptions returns a filled-in options instance given a string array of arguments.
func GetMonitorsOptions(args []string) (*MonitorsOptions, error) {
	var opts MonitorsOptions
	if err := assignValuesFromArgs(args, monitorsParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

