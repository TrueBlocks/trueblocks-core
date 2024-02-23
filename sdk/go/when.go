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
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

// When does chifra when
func WhenCmd(w io.Writer, options map[string]string) error {
	return when.When(w, options)
}

// EXISTING_CODE
type WhenOptions struct {
	Blocks     []base.Blknum
	List       bool
	Timestamps bool
	Count      bool
	Repair     bool
	Check      bool
	Update     bool
	Deep       bool
	Globals    Globals
}

func When(w io.Writer, opts WhenOptions) error {
	options := map[string]string{}

	blkIds := make([]string, 0, len(opts.Blocks))
	for _, bn := range opts.Blocks {
		blkIds = append(blkIds, fmt.Sprintf("%d", bn))
	}
	options["blocks"] = strings.Join(blkIds, " ")
	if opts.List {
		options["list"] = "true"
	}
	if opts.Timestamps {
		options["timestamps"] = "true"
	}
	if opts.Count {
		options["count"] = "true"
	}
	if opts.Repair {
		options["repair"] = "true"
	}
	if opts.Check {
		options["check"] = "true"
	}
	if opts.Update {
		options["update"] = "true"
	}
	if opts.Deep {
		options["deep"] = "true"
	}
	opts.Globals.mapGlobals(options)

	return when.When(w, options)
}

// EXISTING_CODE

