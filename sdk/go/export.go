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
	"encoding/json"
	"fmt"
	"io"
	"net/url"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	export "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ExportOptions struct {
	Addrs       []string    `json:"addrs,omitempty"`
	Topics      []string    `json:"topics,omitempty"`
	Fourbytes   []string    `json:"fourbytes,omitempty"`
	Appearances bool        `json:"appearances,omitempty"`
	Receipts    bool        `json:"receipts,omitempty"`
	Logs        bool        `json:"logs,omitempty"`
	Traces      bool        `json:"traces,omitempty"`
	Neighbors   bool        `json:"neighbors,omitempty"`
	Accounting  bool        `json:"accounting,omitempty"`
	Statements  bool        `json:"statements,omitempty"`
	Balances    bool        `json:"balances,omitempty"`
	Withdrawals bool        `json:"withdrawals,omitempty"`
	Articulate  bool        `json:"articulate,omitempty"`
	CacheTraces bool        `json:"cacheTraces,omitempty"`
	Count       bool        `json:"count,omitempty"`
	FirstRecord uint64      `json:"firstRecord,omitempty"`
	MaxRecords  uint64      `json:"maxRecords,omitempty"`
	Relevant    bool        `json:"relevant,omitempty"`
	Emitter     []string    `json:"emitter,omitempty"`
	Topic       []string    `json:"topic,omitempty"`
	Reverted    bool        `json:"reverted,omitempty"`
	Asset       []string    `json:"asset,omitempty"`
	Flow        ExportFlow  `json:"flow,omitempty"`
	Factory     bool        `json:"factory,omitempty"`
	Unripe      bool        `json:"unripe,omitempty"`
	Reversed    bool        `json:"reversed,omitempty"`
	NoZero      bool        `json:"noZero,omitempty"`
	FirstBlock  base.Blknum `json:"firstBlock,omitempty"`
	LastBlock   base.Blknum `json:"lastBlock,omitempty"`
	Globals

	// EXISTING_CODE
	// Load        string      `json:"load,omitempty"`
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *ExportOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Export implements the chifra export command for the SDK.
func (opts *ExportOptions) Export(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Addrs {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("addrs", item)
		}
	}
	for _, v := range opts.Topics {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("topics", item)
		}
	}
	for _, v := range opts.Fourbytes {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("fourbytes", item)
		}
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
		values.Set("firstRecord", fmt.Sprintf("%d", opts.FirstRecord))
	}
	if opts.MaxRecords > 0 {
		values.Set("maxRecords", fmt.Sprintf("%d", opts.MaxRecords))
	}
	if opts.Relevant {
		values.Set("relevant", "true")
	}
	for _, v := range opts.Emitter {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("emitter", item)
		}
	}
	for _, v := range opts.Topic {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("topic", item)
		}
	}
	if opts.Reverted {
		values.Set("reverted", "true")
	}
	for _, v := range opts.Asset {
		items := strings.Split(v, " ")
		for _, item := range items {
			values.Add("asset", item)
		}
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
		values.Set("noZero", "true")
	}
	if opts.FirstBlock > 0 {
		values.Set("firstBlock", fmt.Sprintf("%d", opts.FirstBlock))
	}
	if opts.LastBlock > 0 {
		values.Set("lastBlock", fmt.Sprintf("%d", opts.LastBlock))
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return export.Export(w, values)
}

// exportParseFunc handles specail cases such as structs and enums (if any).
func exportParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	// EXISTING_CODE
	// EXISTING_CODE
	return found, nil
}

// GetExportOptions returns a filled-in options instance given a string array of arguments.
func GetExportOptions(args []string) (*ExportOptions, error) {
	var opts ExportOptions
	if err := assignValuesFromArgs(args, exportParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

type ExportFlow int

const (
	NoEF ExportFlow = 0
	EFIn = 1 << iota
	EFOut
	EFZero
)

func (v ExportFlow) String() string {
	switch v {
	case NoEF:
		return "none"
	}

	var m = map[ExportFlow]string{
		EFIn: "in",
		EFOut: "out",
		EFZero: "zero",
	}

	var ret []string
	for _, val := range []ExportFlow{EFIn, EFOut, EFZero} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

// EXISTING_CODE
// EXISTING_CODE

