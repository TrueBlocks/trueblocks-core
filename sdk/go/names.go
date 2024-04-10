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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	names "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type NamesOptions struct {
	Terms     []string     `json:"terms,omitempty"`
	Expand    bool         `json:"expand,omitempty"`
	MatchCase bool         `json:"matchCase,omitempty"`
	All       bool         `json:"all,omitempty"`
	Custom    bool         `json:"custom,omitempty"`
	Prefund   bool         `json:"prefund,omitempty"`
	Addr      bool         `json:"addr,omitempty"`
	Tags      bool         `json:"tags,omitempty"`
	Clean     bool         `json:"clean,omitempty"`
	Regular   bool         `json:"regular,omitempty"`
	DryRun    bool         `json:"dryRun,omitempty"`
	Autoname  base.Address `json:"autoname,omitempty"`
	Create    bool         `json:"create,omitempty"`
	Update    bool         `json:"update,omitempty"`
	Delete    bool         `json:"delete,omitempty"`
	Undelete  bool         `json:"undelete,omitempty"`
	Remove    bool         `json:"remove,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *NamesOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// NamesBytes implements the chifra names command for the SDK.
func (opts *NamesOptions) NamesBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting names struct to URL values: %v", err)
	}

	return names.Names(w, values)
}

// namesParseFunc handles special cases such as structs and enums (if any).
func namesParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*NamesOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(names): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetNamesOptions returns a filled-in options instance given a string array of arguments.
func GetNamesOptions(args []string) (*NamesOptions, error) {
	var opts NamesOptions
	if err := assignValuesFromArgs(args, namesParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type namesGeneric interface {
	types.SimpleName |
		base.Address |
		string
}

func queryNames[T namesGeneric](opts *NamesOptions) ([]T, *types.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.NamesBytes(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[T]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
}

// Names implements the chifra names command.
func (opts *NamesOptions) Names() ([]types.SimpleName, *types.MetaData, error) {
	return queryNames[types.SimpleName](opts)
}

// NamesAddr implements the chifra names --addr command.
func (opts *NamesOptions) NamesAddr() ([]base.Address, *types.MetaData, error) {
	return queryNames[base.Address](opts)
}

// NamesTags implements the chifra names --tags command.
func (opts *NamesOptions) NamesTags() ([]string, *types.MetaData, error) {
	return queryNames[string](opts)
}

// No enums
// EXISTING_CODE
// EXISTING_CODE
