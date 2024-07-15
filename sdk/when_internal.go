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
	when "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type whenOptionsInternal struct {
	BlockIds   []string          `json:"blocks,omitempty"`
	List       bool              `json:"list,omitempty"`
	Timestamps bool              `json:"timestamps,omitempty"`
	Count      bool              `json:"count,omitempty"`
	Truncate   base.Blknum       `json:"truncate,omitempty"`
	Repair     bool              `json:"repair,omitempty"`
	Check      bool              `json:"check,omitempty"`
	Update     bool              `json:"update,omitempty"`
	Deep       bool              `json:"deep,omitempty"`
	RenderCtx  *output.RenderCtx `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts *whenOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// WhenBytes implements the chifra when command for the SDK.
func (opts *whenOptionsInternal) WhenBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting when struct to URL values: %v", err)
	}

	if opts.RenderCtx == nil {
		opts.RenderCtx = output.NewRenderContext()
	}
	return when.When(opts.RenderCtx, w, values)
}

// whenParseFunc handles special cases such as structs and enums (if any).
func whenParseFunc(target any, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*whenOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(when): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetWhenOptions returns a filled-in options instance given a string array of arguments.
func GetWhenOptions(args []string) (*whenOptionsInternal, error) {
	var opts whenOptionsInternal
	if err := assignValuesFromArgs(args, whenParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type whenGeneric interface {
	types.NamedBlock |
		types.Timestamp |
		types.TimestampCount
}

func queryWhen[T whenGeneric](opts *whenOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.WhenBytes(&buffer); err != nil {
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
func (opts *WhenOptions) toInternal() *whenOptionsInternal {
	return &whenOptionsInternal{
		BlockIds:  opts.BlockIds,
		Truncate:  opts.Truncate,
		Repair:    opts.Repair,
		Check:     opts.Check,
		Update:    opts.Update,
		Deep:      opts.Deep,
		RenderCtx: opts.RenderCtx,
		Globals:   opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
