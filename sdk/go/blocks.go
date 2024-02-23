// Copyright 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were generated with makeClass --run. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */

package sdk

import (
	"fmt"
	"io"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	blocks "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
)

// Blocks does chifra blocks
func Blocks(w io.Writer, options map[string]string) error {
	return blocks.Blocks(w, options)
}

// EXISTING_CODE
type BlocksFlow int

const (
	NoFlow BlocksFlow = iota
	From
	To
	Reward
)

type Fmt int

const (
	NoFmt Fmt = iota
	Json
	Txt
	Csv
)

type BlocksOptions struct {
	Blocks      []base.Blknum
	Hashes      bool
	Uncles      bool
	Traces      bool
	Uniq        bool
	Logs        bool
	Withdrawals bool
	Articulate  bool
	Count       bool
	BigRange    uint
	Flow        BlocksFlow
	Emitter     []base.Address
	Topic       []base.Topic
	Ether       bool
	Raw         bool
	Cache       bool
	Decache     bool
	Fmt         Fmt
	Verbose     bool
}

func BlockCmd(w io.Writer, opts BlocksOptions) error {
	options := make(map[string]string, 0)
	if opts.Hashes {
		options["hashes"] = "true"
	}
	if opts.Uncles {
		options["uncles"] = "true"
	}
	if opts.Traces {
		options["traces"] = "true"
	}
	if opts.Uniq {
		options["uniq"] = "true"
	}
	if opts.Logs {
		options["logs"] = "true"
	}
	if opts.Withdrawals {
		options["withdrawals"] = "true"
	}
	if opts.Articulate {
		options["articulate"] = "true"
	}
	if opts.Count {
		options["count"] = "true"
	}
	if opts.BigRange > 0 {
		options["bigRange"] = fmt.Sprintf("%d", opts.BigRange)
	}
	if opts.Flow == From {
		options["flow"] = "from"
	}
	if opts.Flow == To {
		options["flow"] = "to"
	}
	if opts.Flow == Reward {
		options["flow"] = "reward"
	}
	if len(opts.Emitter) > 0 {
		emitters := make([]string, 0)
		for _, emitter := range opts.Emitter {
			emitters = append(emitters, emitter.Hex())
		}
		options["emitter"] = strings.Join(emitters, " ")
	}
	if len(opts.Topic) > 0 {
		options["topic"] = strings.Join(opts.Topic, " ")
	}
	if opts.Ether {
		options["ether"] = "true"
	}
	if opts.Raw {
		options["raw"] = "true"
	}
	if opts.Cache {
		options["cache"] = "true"
	}
	if opts.Decache {
		options["decache"] = "true"
	}
	if opts.Fmt == Json {
		options["fmt"] = "json"
	}
	if opts.Fmt == Txt {
		options["fmt"] = "txt"
	}
	if opts.Fmt == Csv {
		options["fmt"] = "csv"
	}
	if opts.Verbose {
		options["verbose"] = "true"
	}
	blocks := make([]string, 0, len(opts.Blocks))
	for _, bn := range opts.Blocks {
		blocks = append(blocks, fmt.Sprintf("%d", bn))
	}
	options["blocks"] = strings.Join(blocks, " ")

	return Blocks(w, options)
}

// EXISTING_CODE
