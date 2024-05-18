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
	"bytes"
	"encoding/json"
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	export "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type exportOptionsInternal struct {
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
	Load        string      `json:"load,omitempty"`
	Reversed    bool        `json:"reversed,omitempty"`
	NoZero      bool        `json:"noZero,omitempty"`
	FirstBlock  base.Blknum `json:"firstBlock,omitempty"`
	LastBlock   base.Blknum `json:"lastBlock,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *exportOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ExportBytes implements the chifra export command for the SDK.
func (opts *exportOptionsInternal) ExportBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting export struct to URL values: %v", err)
	}

	return export.Export(w, values)
}

// exportParseFunc handles special cases such as structs and enums (if any).
func exportParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*exportOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(export): target is not of correct type")
	}

	if key == "flow" {
		var err error
		values := strings.Split(value, ",")
		if opts.Flow, err = enumFromExportFlow(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetExportOptions returns a filled-in options instance given a string array of arguments.
func GetExportOptions(args []string) (*exportOptionsInternal, error) {
	var opts exportOptionsInternal
	if err := assignValuesFromArgs(args, exportParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type exportGeneric interface {
	types.Transaction |
		types.Appearance |
		types.Receipt |
		types.Log |
		types.Trace |
		types.Message |
		types.Statement |
		types.State |
		types.Withdrawal |
		types.AppearanceCount
}

func queryExport[T exportGeneric](opts *exportOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	if opts.Statements {
		opts.Accounting = true
	}
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.ExportBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
	str = convertObjectToArray("inputs", str)
	str = convertObjectToArray("outputs", str)
	// EXISTING_CODE

	var result Result[T]
	if err := json.Unmarshal([]byte(str), &result); err != nil {
		debugPrint(str, result, err)
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// toInternal converts the SDK options to the internal options format.
func (opts *ExportOptions) toInternal() *exportOptionsInternal {
	return &exportOptionsInternal{
		Addrs:       opts.Addrs,
		Topics:      opts.Topics,
		Fourbytes:   opts.Fourbytes,
		Accounting:  opts.Accounting,
		Articulate:  opts.Articulate,
		CacheTraces: opts.CacheTraces,
		FirstRecord: opts.FirstRecord,
		MaxRecords:  opts.MaxRecords,
		Relevant:    opts.Relevant,
		Emitter:     opts.Emitter,
		Topic:       opts.Topic,
		Reverted:    opts.Reverted,
		Asset:       opts.Asset,
		Flow:        opts.Flow,
		Factory:     opts.Factory,
		Unripe:      opts.Unripe,
		Load:        opts.Load,
		Reversed:    opts.Reversed,
		NoZero:      opts.NoZero,
		FirstBlock:  opts.FirstBlock,
		LastBlock:   opts.LastBlock,
		Globals:     opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
