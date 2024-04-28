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

type ReceiptsOptions struct {
	TransactionIds []string `json:"transactions,omitempty"`
	Articulate     bool     `json:"articulate,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *ReceiptsOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Receipts implements the chifra receipts command.
func (opts *ReceiptsOptions) Receipts() ([]types.Receipt, *types.MetaData, error) {
	in := opts.toInternal()
	return queryReceipts[types.Receipt](in)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
