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
	export "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ExportOptions struct {
	Addrs       []string // allow for ENS names and addresses
	Topics      []string // topics are strings
	Fourbytes   []string
	Appearances bool
	Receipts    bool
	Logs        bool
	Traces      bool
	Neighbors   bool
	Accounting  bool
	Statements  bool
	Balances    bool
	Withdrawals bool
	Articulate  bool
	CacheTraces bool
	Count       bool
	FirstRecord uint64
	MaxRecords  uint64
	Relevant    bool
	Emitter     []string // allow for ENS names and addresses
	Topic       []string // topics are strings
	Reverted    bool
	Asset       []string // allow for ENS names and addresses
	Flow        ExportFlow
	Factory     bool
	Unripe      bool
	Load        string
	Reversed    bool
	NoZero      bool
	FirstBlock  base.Blknum
	LastBlock   base.Blknum
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Export implements the chifra export command for the SDK.
func (opts *ExportOptions) Export(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	// EXISTING_CODE

	return export.Export(w, values)
}

type ExportFlow int

const (
	NoEF ExportFlow = iota
	EFIn
	EFOut
	EFZero
)

func (v ExportFlow) String() string {
	return []string{
		"noef",
		"in",
		"out",
		"zero",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

