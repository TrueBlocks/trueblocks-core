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
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	// EXISTING_CODE
)

type BlocksOptions struct {
	BlockIds    []string    `json:"blocks,omitempty"`
	Flow        BlocksFlow  `json:"flow,omitempty"`
	Emitter     []string    `json:"emitter,omitempty"`
	Topic       []string    `json:"topic,omitempty"`
	Articulate  bool        `json:"articulate,omitempty"`
	BigRange    uint64      `json:"bigRange,omitempty"`
	CacheTxs    bool        `json:"cacheTxs,omitempty"`
	CacheTraces bool        `json:"cacheTraces,omitempty"`
	List        base.Blknum `json:"list,omitempty"`
	ListCount   base.Blknum `json:"listCount,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts BlocksOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// Blocks implements the chifra blocks command.
func (opts *BlocksOptions) Blocks() ([]types.Block[types.Transaction], *types.MetaData, error) {
	in := opts.toInternal()
	return queryBlocks[types.Block[types.Transaction]](in)
}

// BlocksHashes implements the chifra blocks --hashes command.
func (opts *BlocksOptions) BlocksHashes() ([]types.Block[string], *types.MetaData, error) {
	in := opts.toInternal()
	in.Hashes = true
	return queryBlocks[types.Block[string]](in)
}

// BlocksUncles implements the chifra blocks --uncles command.
func (opts *BlocksOptions) BlocksUncles() ([]types.Block[string], *types.MetaData, error) {
	in := opts.toInternal()
	in.Uncles = true
	return queryBlocks[types.Block[string]](in)
}

// BlocksTraces implements the chifra blocks --traces command.
func (opts *BlocksOptions) BlocksTraces() ([]types.Trace, *types.MetaData, error) {
	in := opts.toInternal()
	in.Traces = true
	return queryBlocks[types.Trace](in)
}

// BlocksUniq implements the chifra blocks --uniq command.
func (opts *BlocksOptions) BlocksUniq() ([]types.Appearance, *types.MetaData, error) {
	in := opts.toInternal()
	in.Uniq = true
	return queryBlocks[types.Appearance](in)
}

// BlocksLogs implements the chifra blocks --logs command.
func (opts *BlocksOptions) BlocksLogs() ([]types.Log, *types.MetaData, error) {
	in := opts.toInternal()
	in.Logs = true
	return queryBlocks[types.Log](in)
}

// BlocksWithdrawals implements the chifra blocks --withdrawals command.
func (opts *BlocksOptions) BlocksWithdrawals() ([]types.Withdrawal, *types.MetaData, error) {
	in := opts.toInternal()
	in.Withdrawals = true
	return queryBlocks[types.Withdrawal](in)
}

// BlocksCount implements the chifra blocks --count command.
func (opts *BlocksOptions) BlocksCount() ([]types.BlockCount, *types.MetaData, error) {
	in := opts.toInternal()
	in.Count = true
	return queryBlocks[types.BlockCount](in)
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
