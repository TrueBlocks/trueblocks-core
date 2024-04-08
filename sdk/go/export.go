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
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
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
	Load        string      `json:"load,omitempty"`
	Reversed    bool        `json:"reversed,omitempty"`
	NoZero      bool        `json:"noZero,omitempty"`
	FirstBlock  base.Blknum `json:"firstBlock,omitempty"`
	LastBlock   base.Blknum `json:"lastBlock,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *ExportOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ExportBytes implements the chifra export command for the SDK.
func (opts *ExportOptions) ExportBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting export struct to URL values: %v", err)
	}

	return export.Export(w, values)
}

// exportParseFunc handles special cases such as structs and enums (if any).
func exportParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*ExportOptions)
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
func GetExportOptions(args []string) (*ExportOptions, error) {
	var opts ExportOptions
	if err := assignValuesFromArgs(args, exportParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type exportGeneric interface {
	types.SimpleTransaction |
		types.SimpleAppearance |
		types.SimpleReceipt |
		types.SimpleLog |
		types.SimpleTrace |
		types.SimpleStatement |
		types.SimpleState |
		types.SimpleWithdrawal |
		types.SimpleAppearanceCount
}

func queryExport[T exportGeneric](opts *ExportOptions) ([]T, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.ExportBytes(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[T]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// Export implements the chifra export command.
func (opts *ExportOptions) Export() ([]types.SimpleTransaction, *rpc.MetaData, error) {
	return queryExport[types.SimpleTransaction](opts)
}

// ExportAppearances implements the chifra export --appearances command.
func (opts *ExportOptions) ExportAppearances() ([]types.SimpleAppearance, *rpc.MetaData, error) {
	return queryExport[types.SimpleAppearance](opts)
}

// ExportReceipts implements the chifra export --receipts command.
func (opts *ExportOptions) ExportReceipts() ([]types.SimpleReceipt, *rpc.MetaData, error) {
	return queryExport[types.SimpleReceipt](opts)
}

// ExportLogs implements the chifra export --logs command.
func (opts *ExportOptions) ExportLogs() ([]types.SimpleLog, *rpc.MetaData, error) {
	return queryExport[types.SimpleLog](opts)
}

// ExportTraces implements the chifra export --traces command.
func (opts *ExportOptions) ExportTraces() ([]types.SimpleTrace, *rpc.MetaData, error) {
	return queryExport[types.SimpleTrace](opts)
}

// ExportStatements implements the chifra export --statements command.
func (opts *ExportOptions) ExportStatements() ([]types.SimpleStatement, *rpc.MetaData, error) {
	return queryExport[types.SimpleStatement](opts)
}

// ExportBalances implements the chifra export --balances command.
func (opts *ExportOptions) ExportBalances() ([]types.SimpleState, *rpc.MetaData, error) {
	return queryExport[types.SimpleState](opts)
}

// ExportWithdrawals implements the chifra export --withdrawals command.
func (opts *ExportOptions) ExportWithdrawals() ([]types.SimpleWithdrawal, *rpc.MetaData, error) {
	return queryExport[types.SimpleWithdrawal](opts)
}

// ExportCount implements the chifra export --count command.
func (opts *ExportOptions) ExportCount() ([]types.SimpleAppearanceCount, *rpc.MetaData, error) {
	return queryExport[types.SimpleAppearanceCount](opts)
}

type ExportFlow int

const (
	NoEF ExportFlow = 0
	EFIn            = 1 << iota
	EFOut
	EFZero
)

func (v ExportFlow) String() string {
	switch v {
	case NoEF:
		return "none"
	}

	var m = map[ExportFlow]string{
		EFIn:   "in",
		EFOut:  "out",
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

func enumFromExportFlow(values []string) (ExportFlow, error) {
	if len(values) == 0 {
		return NoEF, fmt.Errorf("no value provided for flow option")
	}

	var result ExportFlow
	for _, val := range values {
		switch val {
		case "in":
			result |= EFIn
		case "out":
			result |= EFOut
		case "zero":
			result |= EFZero
		default:
			return NoEF, fmt.Errorf("unknown flow: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
// EXISTING_CODE
