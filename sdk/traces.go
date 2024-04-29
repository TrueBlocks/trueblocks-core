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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type TracesOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Filter         string   `json:"filter,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *TracesOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Traces implements the chifra traces command.
func (opts *TracesOptions) Traces() ([]types.Trace, *types.MetaData, error) {
	in := opts.toInternal()
	return queryTraces[types.Trace](in)
}

// TracesCount implements the chifra traces --count command.
func (opts *TracesOptions) TracesCount() ([]types.TraceCount, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return queryTraces[types.TraceCount](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
