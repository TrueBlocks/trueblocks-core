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
	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type BlocksOptions struct {
	BlockIds    []string    `json:"blocks,omitempty"`
	Hashes      bool        `json:"hashes,omitempty"`
	Uncles      bool        `json:"uncles,omitempty"`
	Traces      bool        `json:"traces,omitempty"`
	Uniq        bool        `json:"uniq,omitempty"`
	Flow        BlocksFlow  `json:"flow,omitempty"`
	Logs        bool        `json:"logs,omitempty"`
	Emitter     []string    `json:"emitter,omitempty"`
	Topic       []string    `json:"topic,omitempty"`
	Withdrawals bool        `json:"withdrawals,omitempty"`
	Articulate  bool        `json:"articulate,omitempty"`
	BigRange    uint64      `json:"bigRange,omitempty"`
	Count       bool        `json:"count,omitempty"`
	CacheTxs    bool        `json:"cacheTxs,omitempty"`
	CacheTraces bool        `json:"cacheTraces,omitempty"`
	List        base.Blknum `json:"list,omitempty"`
	ListCount   base.Blknum `json:"listCount,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *BlocksOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Blocks implements the chifra blocks command for the SDK.
func (opts *BlocksOptions) Blocks(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting blocks struct to URL values: %v", err)
	}

	return blocks.Blocks(w, values)
}

// blocksParseFunc handles specail cases such as structs and enums (if any).
func blocksParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*BlocksOptions)
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
func GetBlocksOptions(args []string) (*BlocksOptions, error) {
	var opts BlocksOptions
	if err := assignValuesFromArgs(args, blocksParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

func (opts *BlocksOptions) Query() ([]types.SimpleBlock[string], *rpc.MetaData, error) {
	buffer := bytes.Buffer{}
	if err := opts.Blocks(&buffer); err != nil {
		logger.Fatal(err)
	}

	var result Result[types.SimpleBlock[string]]
	if err := json.Unmarshal(buffer.Bytes(), &result); err != nil {
		return nil, nil, err
	} else {
		return result.Data, &result.Meta, nil
	}
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

func enumFromBlocksFlow(values []string) (BlocksFlow, error) {
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
// EXISTING_CODE
