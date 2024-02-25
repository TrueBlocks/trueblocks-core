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
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	names "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type NamesOptions struct {
	Terms     []string
	Expand    bool
	MatchCase bool
	All       bool
	Custom    bool
	Prefund   bool
	Addr      bool
	Tags      bool
	Clean     bool
	Regular   bool
	DryRun    bool
	Autoname  base.Address
	Create    bool
	Update    bool
	Delete    bool
	Undelete  bool
	Remove    bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Names implements the chifra names command for the SDK.
func (opts *NamesOptions) Names(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Terms {
		values.Add("terms", v)
	}
	if opts.Expand {
		values.Set("expand", "true")
	}
	if opts.MatchCase {
		values.Set("match_case", "true")
	}
	if opts.All {
		values.Set("all", "true")
	}
	if opts.Custom {
		values.Set("custom", "true")
	}
	if opts.Prefund {
		values.Set("prefund", "true")
	}
	if opts.Addr {
		values.Set("addr", "true")
	}
	if opts.Tags {
		values.Set("tags", "true")
	}
	if opts.Clean {
		values.Set("clean", "true")
	}
	if opts.Regular {
		values.Set("regular", "true")
	}
	if opts.DryRun {
		values.Set("dry_run", "true")
	}
	if !opts.Autoname.IsZero() {
		values.Set("autoname", opts.Autoname.String())
	}
	if opts.Create {
		values.Set("create", "true")
	}
	if opts.Update {
		values.Set("update", "true")
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
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return names.Names(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE
