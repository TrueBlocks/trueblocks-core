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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type LogsOptions struct {
	TransactionIds []string          `json:"transactions,omitempty"`
	Emitter        []string          `json:"emitter,omitempty"`
	Topic          []string          `json:"topic,omitempty"`
	Articulate     bool              `json:"articulate,omitempty"`
	RenderCtx      *output.RenderCtx `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts LogsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Logs implements the chifra logs command.
func (opts *LogsOptions) Logs() ([]types.Log, *types.MetaData, error) {
	in := opts.toInternal()
	return queryLogs[types.Log](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
