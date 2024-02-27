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
	"fmt"
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type BlocksOptions struct {
	BlockIds    []string // allow for block ranges and steps
	Hashes      bool
	Uncles      bool
	Traces      bool
	Uniq        bool
	Flow        BlocksFlow
	Logs        bool
	Emitter     []string // allow for ENS names and addresses
	Topic       []string // topics are strings
	Withdrawals bool
	Articulate  bool
	BigRange    uint64
	Count       bool
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// Blocks implements the chifra blocks command for the SDK.
func (opts *BlocksOptions) Blocks(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, blockId := range opts.BlockIds {
		values.Add("blocks", blockId)
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
		values.Add("emitter", emitter)
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
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return blocks.Blocks(w, values)
}

// GetBlocksOptions returns an options instance given a string array of arguments.
func GetBlocksOptions(args []string) (*BlocksOptions, error) {
	var opts BlocksOptions

	for i, arg := range args {
		// EXISTING_CODE
		logger.Info(fmt.Sprintf("\t%d: %s", i, arg))
		// EXISTING_CODE
	}

	return &opts, nil
}

type BlocksFlow int

const (
	NoBF BlocksFlow = iota
	BFFrom
	BFTo
	BFReward
)

func (v BlocksFlow) String() string {
	return []string{
		"nobf",
		"from",
		"to",
		"reward",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE
