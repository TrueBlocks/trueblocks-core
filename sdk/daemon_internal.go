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
	"encoding/json"
	"fmt"
	"io"
	"strings"

	daemon "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type daemonOptionsInternal struct {
	Url     string       `json:"url,omitempty"`
	Api     DaemonApi    `json:"api,omitempty"`
	Scrape  DaemonScrape `json:"scrape,omitempty"`
	Monitor bool         `json:"monitor,omitempty"`
	Silent  bool         `json:"silent,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *daemonOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// DaemonBytes implements the chifra daemon command for the SDK.
func (opts *daemonOptionsInternal) DaemonBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting daemon struct to URL values: %v", err)
	}

	return daemon.Daemon(w, values)
}

// daemonParseFunc handles special cases such as structs and enums (if any).
func daemonParseFunc(target any, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*daemonOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(daemon): target is not of correct type")
	}

	if key == "api" {
		var err error
		values := strings.Split(value, ",")
		if opts.Api, err = enumFromDaemonApi(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}
	if key == "scrape" {
		var err error
		values := strings.Split(value, ",")
		if opts.Scrape, err = enumFromDaemonScrape(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetDaemonOptions returns a filled-in options instance given a string array of arguments.
func GetDaemonOptions(args []string) (*daemonOptionsInternal, error) {
	var opts daemonOptionsInternal
	if err := assignValuesFromArgs(args, daemonParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

// EXISTING_CODE
func (opts *DaemonOptions) toInternal() *daemonOptionsInternal {
	return &daemonOptionsInternal{
		Url:     opts.Url,
		Api:     opts.Api,
		Scrape:  opts.Scrape,
		Monitor: opts.Monitor,
		Silent:  opts.Silent,
		Globals: opts.Globals,
	}
}

// EXISTING_CODE
