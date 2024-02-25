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

	logs "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type LogsOptions struct {
	TransactionIds []string
	Emitter        []string // allow for ENS names and addresses
	Topic          []string // topics are strings
	Articulate     bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Logs implements the chifra logs command for the SDK.
func (opts *LogsOptions) Logs(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	//   transactions - a space-separated list of one or more transaction identifiers (required)
	//   -m, --emitter strings   filter logs to show only those logs emitted by the given address(es)
	//   -B, --topic strings     filter logs to show only those with this topic(s)
	//   -a, --articulate        articulate the retrieved data if ABIs can be found
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return logs.Logs(w, values)
}

// no enums

// EXISTING_CODE
// EXISTING_CODE

