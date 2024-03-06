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
	"fmt"
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type WhenOptions struct {
	BlockIds   []string
	List       bool
	Timestamps bool
	Count      bool
	Repair     bool
	Check      bool
	Update     bool
	Deep       bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// When implements the chifra when command for the SDK.
func (opts *WhenOptions) When(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, blockId := range opts.BlockIds {
		values.Add("blocks", blockId)
	}
	if opts.List {
		values.Set("list", "true")
	}
	if opts.Timestamps {
		values.Set("timestamps", "true")
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.Repair {
		values.Set("repair", "true")
	}
	if opts.Check {
		values.Set("check", "true")
	}
	if opts.Update {
		values.Set("update", "true")
	}
	if opts.Deep {
		values.Set("deep", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return when.When(w, values)
}

// GetWhenOptions returns an options instance given a string array of arguments.
func GetWhenOptions(args []string) (*WhenOptions, error) {
	var opts WhenOptions

	for i, arg := range args {
		// EXISTING_CODE
		logger.Info(fmt.Sprintf("\t%d: %s", i, arg))
		// EXISTING_CODE
	}

	return &opts, nil
}

// No enums

// EXISTING_CODE
// EXISTING_CODE

