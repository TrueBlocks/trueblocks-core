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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/output"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	slurp "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type slurpOptionsInternal struct {
	Addrs       []string    `json:"addrs,omitempty"`
	BlockIds    []string    `json:"blocks,omitempty"`
	Parts       SlurpParts  `json:"parts,omitempty"`
	Appearances bool        `json:"appearances,omitempty"`
	Articulate  bool        `json:"articulate,omitempty"`
	Source      SlurpSource `json:"source,omitempty"`
	Count       bool        `json:"count,omitempty"`
	Page        uint64      `json:"page,omitempty"`
	PageId      string      `json:"pageId,omitempty"`
	PerPage     uint64      `json:"perPage,omitempty"`
	Sleep       float64     `json:"sleep,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *slurpOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// SlurpBytes implements the chifra slurp command for the SDK.
func (opts *slurpOptionsInternal) SlurpBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting slurp struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	opts.Globals.RenderCtx = &rCtx
	return slurp.Slurp(rCtx, w, values)
}

// slurpParseFunc handles special cases such as structs and enums (if any).
func slurpParseFunc(target any, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*slurpOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(slurp): target is not of correct type")
	}

	if key == "parts" {
		var err error
		values := strings.Split(value, ",")
		if opts.Parts, err = enumFromSlurpParts(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}
	if key == "source" {
		var err error
		values := strings.Split(value, ",")
		if opts.Source, err = enumFromSlurpSource(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetSlurpOptions returns a filled-in options instance given a string array of arguments.
func GetSlurpOptions(args []string) (*slurpOptionsInternal, error) {
	var opts slurpOptionsInternal
	if err := assignValuesFromArgs(args, slurpParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type slurpGeneric interface {
	types.Slurp |
		types.Appearance |
		types.Monitor
}

func querySlurp[T slurpGeneric](opts *slurpOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.SlurpBytes(&buffer); err != nil {
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
func (opts *SlurpOptions) toInternal() *slurpOptionsInternal {
	return &slurpOptionsInternal{
		Addrs:      opts.Addrs,
		BlockIds:   opts.BlockIds,
		Parts:      opts.Parts,
		Articulate: opts.Articulate,
		Source:     opts.Source,
		Page:       opts.Page,
		PageId:     opts.PageId,
		PerPage:    opts.PerPage,
		Sleep:      opts.Sleep,
		Globals:    opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
