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
	"io"
	"net/url"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
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
	// EXISTING_CODE

	return state.State(w, values)
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

