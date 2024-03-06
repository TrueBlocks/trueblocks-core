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
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	state "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type StateOptions struct {
	Addrs      []string // allow for ENS names and addresses
	BlockIds   []string // allow for block ranges and steps
	Parts      StateParts
	Changes    bool
	NoZero     bool
	Call       string
	Articulate bool
	ProxyFor   base.Address
	Globals

	// EXISTING_CODE
	// EXISTING_CODE
}

// State implements the chifra state command for the SDK.
func (opts *StateOptions) State(w io.Writer) error {
	values := make(url.Values)

	// EXISTING_CODE
	for _, v := range opts.Addrs {
		values.Add("addrs", v)
	}
	for _, v := range opts.BlockIds {
		values.Add("blocks", v)
	}
	if opts.Parts != NoSP {
		values.Set("parts", opts.Parts.String())
	}
	if opts.Changes {
		values.Set("changes", "true")
	}
	if opts.NoZero {
		values.Set("no_zero", "true")
	}
	if opts.Call != "" {
		values.Set("call", opts.Call)
	}
	if opts.Articulate {
		values.Set("articulate", "true")
	}
	if opts.ProxyFor.IsZero() {
		values.Set("proxy_for", opts.ProxyFor.String())
	}
	// EXISTING_CODE
	opts.Globals.mapGlobals(values)

	return state.State(w, values)
}

// GetStateOptions returns an options instance given a string array of arguments.
func GetStateOptions(args []string) (*StateOptions, error) {
	var opts StateOptions

	for i, arg := range args {
		// EXISTING_CODE
		logger.Info(fmt.Sprintf("\t%d: %s", i, arg))
		// EXISTING_CODE
	}

	return &opts, nil
}

type StateParts int

const (
	NoSP StateParts = iota
	SPNone
	SPSome
	SPAll
	SPBalance
	SPNonce
	SPCode
	SPProxy
	SPDeployed
	SPAccttype
)

func (v StateParts) String() string {
	return []string{
		"nosp",
		"none",
		"some",
		"all",
		"balance",
		"nonce",
		"code",
		"proxy",
		"deployed",
		"accttype",
	}[v]
}

// EXISTING_CODE
// EXISTING_CODE

