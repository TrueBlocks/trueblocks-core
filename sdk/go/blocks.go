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

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type BlocksOptions struct {
	BlockIds    []string
	Hashes      bool
	Uncles      bool
	Traces      bool
	Uniq        bool
	Flow        FlowEnum
	Logs        bool
	Emitter     []base.Address
	Topic       []base.Topic
	Withdrawals bool
	Articulate  bool
	BigRange    uint64
	Count       bool
	CacheTxs    bool
	CacheTraces bool
	List        base.Blknum
	ListCount   base.Blknum
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
	if opts.Logs {
		values.Set("logs", "true")
	}
	if opts.Withdrawals {
		values.Set("withdrawals", "true")
	}
	if opts.Articulate {
		values.Set("articulate", "true")
	}
	if opts.Count {
		values.Set("count", "true")
	}
	if opts.BigRange > 0 {
		values.Set("bigRange", fmt.Sprintf("%d", opts.BigRange))
	}
	if opts.Flow != NoFlow {
		values.Set("flow", opts.Flow.String())
	}
	if len(opts.Emitter) > 0 {
		for _, emitter := range opts.Emitter {
			values.Add("emitter", emitter.Hex())
		}
	}
	if len(opts.Topic) > 0 {
		for _, topic := range opts.Topic {
			values.Add("topic", topic)
		}
	}
	opts.Globals.mapGlobals(values)
	// EXISTING_CODE

	return blocks.Blocks(w, values)
}

// EXISTING_CODE
type FlowEnum int

const (
	NoFlow FlowEnum = iota
	From
	To
	Reward
)

func (e FlowEnum) String() string {
	switch e {
	case From:
		return "from"
	case To:
		return "to"
	case Reward:
		return "reward"
	}
	return ""
}

// EXISTING_CODE

