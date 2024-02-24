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
	// EXISTING_CODE
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
	Dry_Run   bool
	Autoname  base.Address
	Create    bool
	Update    bool
	Delete    bool
	Undelete  bool
	Remove    bool
	Globals

	// EXISTING_CODE
}

// Names implements the chifra names command for the SDK.
func (opts *NamesOptions) Names(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return names.Names(w, values)
}

// EXISTING_CODE
// EXISTING_CODE

