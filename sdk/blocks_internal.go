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
	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type blocksOptionsInternal struct {
	BlockIds    []string   `json:"blocks,omitempty"`
	Hashes      bool       `json:"hashes,omitempty"`
	Uncles      bool       `json:"uncles,omitempty"`
	Traces      bool       `json:"traces,omitempty"`
	Uniq        bool       `json:"uniq,omitempty"`
	Flow        BlocksFlow `json:"flow,omitempty"`
	Logs        bool       `json:"logs,omitempty"`
	Emitter     []string   `json:"emitter,omitempty"`
	Topic       []string   `json:"topic,omitempty"`
	Withdrawals bool       `json:"withdrawals,omitempty"`
	Articulate  bool       `json:"articulate,omitempty"`
	Count       bool       `json:"count,omitempty"`
	CacheTxs    bool       `json:"cacheTxs,omitempty"`
	CacheTraces bool       `json:"cacheTraces,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *blocksOptionsInternal) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// BlocksBytes implements the chifra blocks command for the SDK.
func (opts *blocksOptionsInternal) BlocksBytes(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		return fmt.Errorf("error converting blocks struct to URL values: %v", err)
	}

	rCtx := output.NewRenderContext()
	return blocks.Blocks(rCtx, w, values)
}

// blocksParseFunc handles special cases such as structs and enums (if any).
func blocksParseFunc(target any, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*blocksOptionsInternal)
	if !ok {
		return false, fmt.Errorf("parseFunc(blocks): target is not of correct type")
	}

	if key == "flow" {
		var err error
		values := strings.Split(value, ",")
		if opts.Flow, err = enumFromBlocksFlow(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return found, nil
}

// GetBlocksOptions returns a filled-in options instance given a string array of arguments.
func GetBlocksOptions(args []string) (*blocksOptionsInternal, error) {
	var opts blocksOptionsInternal
	if err := assignValuesFromArgs(args, blocksParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type blocksGeneric interface {
	types.Block |
		types.LightBlock |
		types.Trace |
		types.Appearance |
		types.Log |
		types.Withdrawal |
		types.BlockCount
}

func queryBlocks[T blocksGeneric](opts *blocksOptionsInternal) ([]T, *types.MetaData, error) {
	// EXISTING_CODE
	// EXISTING_CODE

	buffer := bytes.Buffer{}
	if err := opts.BlocksBytes(&buffer); err != nil {
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
func (opts *BlocksOptions) toInternal() *blocksOptionsInternal {
	return &blocksOptionsInternal{
		BlockIds:    opts.BlockIds,
		Flow:        opts.Flow,
		Emitter:     opts.Emitter,
		Topic:       opts.Topic,
		Articulate:  opts.Articulate,
		CacheTxs:    opts.CacheTxs,
		CacheTraces: opts.CacheTraces,
		Globals:     opts.Globals,
	}
}

// EXISTING_CODE
// EXISTING_CODE
