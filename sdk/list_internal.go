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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type listOptionsInternal struct {
	Addrs       []string     `json:"addrs,omitempty"`
	Count       bool         `json:"count,omitempty"`
	NoZero      bool         `json:"noZero,omitempty"`
	Bounds      bool         `json:"bounds,omitempty"`
	Unripe      bool         `json:"unripe,omitempty"`
	Silent      bool         `json:"silent,omitempty"`
	FirstRecord uint64       `json:"firstRecord,omitempty"`
	MaxRecords  uint64       `json:"maxRecords,omitempty"`
	Reversed    bool         `json:"reversed,omitempty"`
	Publisher   base.Address `json:"publisher,omitempty"`
	FirstBlock  base.Blknum  `json:"firstBlock,omitempty"`
	LastBlock   base.Blknum  `json:"lastBlock,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *listOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ListBytes implements the chifra list command for the SDK.
func (opts *listOptionsInternal) ListBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting list struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	opts.Globals.RenderCtx = &rCtx
	return list.List(rCtx, w, values)
}

// listParseFunc handles special cases such as structs and enums (if any).
func listParseFunc(target any, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*listOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(list): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetListOptions returns a filled-in options instance given a string array of arguments.
func GetListOptions(args []string) (*listOptionsInternal, error) {
	var opts listOptionsInternal
	if err := assignValuesFromArgs(args, listParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type listGeneric interface {
	types.Appearance |
		types.Monitor |
		types.Bounds
}

func queryList[T listGeneric](opts *listOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.ListBytes(&buffer); err != nil {
		return nil, nil, err
	}

	str := buffer.String()
	// EXISTING_CODE
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
func (opts *ListOptions) toInternal() *listOptionsInternal {
	return &listOptionsInternal{
		Addrs:       opts.Addrs,
		NoZero:      opts.NoZero,
		Unripe:      opts.Unripe,
		Silent:      opts.Silent,
		FirstRecord: opts.FirstRecord,
		MaxRecords:  opts.MaxRecords,
		Reversed:    opts.Reversed,
		Publisher:   opts.Publisher,
		FirstBlock:  opts.FirstBlock,
		LastBlock:   opts.LastBlock,
		Globals:     opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
