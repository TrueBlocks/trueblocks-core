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
	initPkg "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type initOptionsInternal struct {
	All        bool         `json:"all,omitempty"`
	Example    string       `json:"example,omitempty"`
	DryRun     bool         `json:"dryRun,omitempty"`
	Publisher  base.Address `json:"publisher,omitempty"`
	FirstBlock base.Blknum  `json:"firstBlock,omitempty"`
	Sleep      float64      `json:"sleep,omitempty"`
	OrigOpts   *InitOptions `json:"-"`
	Globals
}

// String implements the stringer interface
func (opts *initOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// InitBytes implements the chifra init command for the SDK.
func (opts *initOptionsInternal) InitBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting init struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	if opts.OrigOpts != nil {
		opts.OrigOpts.RenderCtx = &rCtx
	}
	return initPkg.Init(rCtx, w, values)
}

// initParseFunc handles special cases such as structs and enums (if any).
func initParseFunc(target any, key, value string) (bool, error) {
	var found bool
	_, ok := target.(*initOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(init): target is not of correct type")
	}

	// No enums
	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetInitOptions returns a filled-in options instance given a string array of arguments.
func GetInitOptions(args []string) (*initOptionsInternal, error) {
	var opts initOptionsInternal
	if err := assignValuesFromArgs(args, initParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type initGeneric interface {
	types.Message
}

func queryInit[T initGeneric](opts *initOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.InitBytes(&buffer); err != nil {
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
func (opts *InitOptions) toInternal() *initOptionsInternal {
	return &initOptionsInternal{
		Publisher:  opts.Publisher,
		FirstBlock: opts.FirstBlock,
		Sleep:      opts.Sleep,
		OrigOpts:   opts,
		Globals:    opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
