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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	list "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type ListOptions struct {
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
func (opts *ListOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// ListBytes implements the chifra list command for the SDK.
func (opts *ListOptions) ListBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting list struct to URL values: %v", err)
	}

	return list.List(w, values)
}

// listParseFunc handles special cases such as structs and enums (if any).
func listParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*ListOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(list): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetListOptions returns a filled-in options instance given a string array of arguments.
func GetListOptions(args []string) (*ListOptions, error) {
	var opts ListOptions
	if err := assignValuesFromArgs(args, listParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type listGeneric interface {
	types.SimpleAppearance |
		types.SimpleAppearanceCount |
		types.SimpleBounds
}

func queryList[T listGeneric](opts *ListOptions) ([]T, *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.ListBytes(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[T]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// List implements the chifra list command.
func (opts *ListOptions) List() ([]types.SimpleAppearance, *rpc.MetaData, error) {
	return queryList[types.SimpleAppearance](opts)
}

// ListCount implements the chifra list --count command.
func (opts *ListOptions) ListCount() ([]types.SimpleAppearanceCount, *rpc.MetaData, error) {
	return queryList[types.SimpleAppearanceCount](opts)
}

// ListBounds implements the chifra list --bounds command.
func (opts *ListOptions) ListBounds() ([]types.SimpleBounds, *rpc.MetaData, error) {
	return queryList[types.SimpleBounds](opts)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
