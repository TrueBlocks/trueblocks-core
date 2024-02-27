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
	for _, v := range opts.Addrs {
		values.Add("addrs", v)
	}
	for _, v := range opts.Topics {
		values.Add("topics", v)
	}
	for _, v := range opts.Fourbytes {
		values.Add("fourbytes", v)
	}
	if opts.Appearances {
		values.Set("appearances", "true")
	}
	if opts.Receipts {
		values.Set("receipts", "true")
	}
	if opts.Logs {
		values.Set("logs", "true")
	}
	if opts.Traces {
		values.Set("traces", "true")
	}
	if opts.Neighbors {
		values.Set("neighbors", "true")
	}
	if opts.Accounting {
		values.Set("accounting", "true")
	}
	if opts.Statements {
		values.Set("statements", "true")
	}
	if opts.Balances {
		values.Set("balances", "true")
	}
	if opts.Withdrawals {
		values.Set("withdrawals", "true")
	}
	if opts.Articulate {
		values.Set("articulate", "true")
	}
	if opts.CacheTraces {
		values.Set("cache_traces", "true")
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.FirstRecord > 0 {
		values.Set("first_record", fmt.Sprintf("%d", opts.FirstRecord))
	}
	if opts.MaxRecords > 0 {
		values.Set("max_records", fmt.Sprintf("%d", opts.MaxRecords))
	}
	if opts.Relevant {
		values.Set("relevant", "true")
	}
	for _, v := range opts.Emitter {
		values.Add("emitter", v)
	}
	for _, v := range opts.Topic {
		values.Add("topic", v)
	}
	if opts.Reverted {
		values.Set("reverted", "true")
	}
	for _, v := range opts.Asset {
		values.Add("asset", v)
	}
	if opts.Flow != NoEF {
		values.Set("flow", opts.Flow.String())
	}
	if opts.Factory {
		values.Set("factory", "true")
	}
	if opts.Unripe {
		values.Set("unripe", "true")
	}
	if opts.Reversed {
		values.Set("reversed", "true")
	}
	if opts.NoZero {
		values.Set("no_zero", "true")
	}
	if opts.FirstBlock > 0 {
		values.Set("first_block", fmt.Sprintf("%d", opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("last_block", fmt.Sprintf("%d", opts.LastBlock))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return export.Export(w, values)
}

// GetExportOptions returns an options instance given a string array of arguments.
func GetExportOptions(args []string) (*ExportOptions, error) {
	var opts ExportOptions

	for _, arg := range args {
		// EXISTING_CODE
		fmt.Println("arg: ", arg)
		// EXISTING_CODE
	}

	return &opts, nil
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
