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

	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type BlocksOptions struct {
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
	BigRange    uint64     `json:"bigRange,omitempty"`
	Count       bool       `json:"count,omitempty"`
	Globals

	// EXISTING_CODE
	List        uint64 `json:"list,omitempty"`
	ListCount   uint64 `json:"listCount,omitempty"`
	CacheTxs    bool   `json:"cacheTxs,omitempty"`
	CacheTraces bool   `json:"cacheTraces,omitempty"`
	// EXISTING_CODE
}

// String implements the stringer interface
func (opts *BlocksOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Blocks implements the chifra blocks command for the SDK.
func (opts *BlocksOptions) Blocks(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, blockId := range opts.BlockIds {
		items := strings.Split(blockId, " ")
		for _, item := range items {
			values.Add("blocks", item)
		}
	}
	if opts.Hashes {
		values.Set("hashes", "true")
	}
	if opts.Uncles {
		values.Set("uncles", "true")
	}
	if opts.Traces {
		values.Set("traces", "true")
	}
	if opts.Uniq {
		values.Set("uniq", "true")
	}
	if opts.Flow != NoBF {
		values.Set("flow", opts.Flow.String())
	}
	if opts.Logs {
		values.Set("logs", "true")
	}
	for _, emitter := range opts.Emitter {
		items := strings.Split(emitter, " ")
		for _, item := range items {
			values.Add("emitter", item)
		}
	}
	for _, topic := range opts.Topic {
		values.Add("topic", topic)
	}
	if opts.Withdrawals {
		values.Set("withdrawals", "true")
	}
	if opts.Articulate {
		values.Set("articulate", "true")
	}
	if opts.BigRange > 0 {
		values.Set("bigRange", fmt.Sprintf("%d", opts.BigRange))
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.List > 0 {
		values.Set("list", fmt.Sprintf("%d", opts.List))
	}
	if opts.ListCount > 0 {
		values.Set("listCount", fmt.Sprintf("%d", opts.ListCount))
	}
	if opts.CacheTxs {
		values.Set("cacheTxs", "true")
	}
	if opts.CacheTraces {
		values.Set("cacheTraces", "true")
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return blocks.Blocks(w, values)
}

// blocksParseFunc handles specail cases such as structs and enums (if any).
func blocksParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	// EXISTING_CODE
	opts, ok := target.(*BlocksOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(blocks): target is not of correct type")
	}

	switch key {
	case "flow":
		var err error
		values := strings.Split(value, ",")
		if opts.Flow, err = enumsFromStrsBlocks(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	return found, nil
}

// GetBlocksOptions returns a filled-in options instance given a string array of arguments.
func GetBlocksOptions(args []string) (*BlocksOptions, error) {
	var opts BlocksOptions
	if err := assignValuesFromArgs(args, blocksParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	// EXISTING_CODE
	// EXISTING_CODE

	return &opts, nil
}

type BlocksFlow int

const (
	NoBF   BlocksFlow = 0
	BFFrom            = 1 << iota
	BFTo
	BFReward
)

func (v BlocksFlow) String() string {
	switch v {
	case NoBF:
		return "none"
	}

	var m = map[BlocksFlow]string{
		BFFrom:   "from",
		BFTo:     "to",
		BFReward: "reward",
	}

	var ret []string
	for _, val := range []BlocksFlow{BFFrom, BFTo, BFReward} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

// EXISTING_CODE
func enumsFromStrsBlocks(values []string) (BlocksFlow, error) {
	if len(values) == 0 {
		return NoBF, fmt.Errorf("no value provided for flow option")
	}

	var result BlocksFlow
	for _, val := range values {
		switch val {
		case "from":
			result |= BFFrom
		case "to":
			result |= BFTo
		case "reward":
			result |= BFReward
		default:
			return NoBF, fmt.Errorf("unknown flow: %s", val)
		}
	}

	return result, nil
}

// EXISTING_CODE
