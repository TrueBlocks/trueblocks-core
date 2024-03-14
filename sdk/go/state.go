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
	"log"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	state "github.com/TrueBlocks/trueblocks-core/src/apps/chifra/sdk"
	// EXISTING_CODE
)

type StateOptions struct {
	Addrs      []string     `json:"addrs,omitempty"`
	BlockIds   []string     `json:"blocks,omitempty"`
	Parts      StateParts   `json:"parts,omitempty"`
	Changes    bool         `json:"changes,omitempty"`
	NoZero     bool         `json:"noZero,omitempty"`
	Call       string       `json:"call,omitempty"`
	Articulate bool         `json:"articulate,omitempty"`
	ProxyFor   base.Address `json:"proxyFor,omitempty"`
	Globals
}

// String implements the stringer interface
func (opts *StateOptions) String() string {
	bytes, _ := json.Marshal(opts)
	return string(bytes)
}

// State implements the chifra state command for the SDK.
func (opts *StateOptions) State(w io.Writer) error {
	values, err := structToValues(*opts)
	if err != nil {
		log.Fatalf("Error converting state struct to URL values: %v", err)
	}

	return state.State(w, values)
}

// stateParseFunc handles specail cases such as structs and enums (if any).
func stateParseFunc(target interface{}, key, value string) (bool, error) {
	var found bool
	opts, ok := target.(*StateOptions)
	if !ok {
		return false, fmt.Errorf("parseFunc(state): target is not of correct type")
	}

	switch key {
	case "parts":
		var err error
		values := strings.Split(value, ",")
		if opts.Parts, err = enumFromStateParts(values); err != nil {
			return false, err
		} else {
			found = true
		}
	}

	// EXISTING_CODE
	if key == "proxyFor" {
		opts.ProxyFor = base.HexToAddress(value)
		return base.IsValidAddress(value), nil
	}
	// EXISTING_CODE

	return found, nil
}

// GetStateOptions returns a filled-in options instance given a string array of arguments.
func GetStateOptions(args []string) (*StateOptions, error) {
	var opts StateOptions
	if err := assignValuesFromArgs(args, stateParseFunc, &opts, &opts.Globals); err != nil {
		return nil, err
	}

	return &opts, nil
}

type StateParts int

const (
	NoSP StateParts = 0
	SPBalance = 1 << iota
	SPNonce
	SPCode
	SPProxy
	SPDeployed
	SPAccttype
	SPSome = SPBalance | SPProxy | SPDeployed | SPAccttype
	SPAll = SPBalance | SPNonce | SPCode | SPProxy | SPDeployed | SPAccttype
)

func (v StateParts) String() string {
	switch v {
	case NoSP:
		return "none"
	case SPSome:
		return "some"
	case SPAll:
		return "all"
	}

	var m = map[StateParts]string{
		SPBalance: "balance",
		SPNonce: "nonce",
		SPCode: "code",
		SPProxy: "proxy",
		SPDeployed: "deployed",
		SPAccttype: "accttype",
	}

	var ret []string
	for _, val := range []StateParts{SPBalance, SPNonce, SPCode, SPProxy, SPDeployed, SPAccttype} {
		if v&val != 0 {
			ret = append(ret, m[val])
		}
	}

	return strings.Join(ret, ",")
}

func enumFromStateParts(values []string) (StateParts, error) {
	if len(values) == 0 {
		return NoSP, fmt.Errorf("no value provided for parts option")
	}

	if len(values) == 1 && values[0] == "all" {
		return SPAll, nil
	} else if len(values) == 1 && values[0] == "some" {
		return SPSome, nil
	}

	var result StateParts
	for _, val := range values {
		switch val {
		case "balance":
			result |= SPBalance
		case "nonce":
			result |= SPNonce
		case "code":
			result |= SPCode
		case "proxy":
			result |= SPProxy
		case "deployed":
			result |= SPDeployed
		case "accttype":
			result |= SPAccttype
		default:
			return NoSP, fmt.Errorf("unknown parts: %s", val)
		}
	}

	return result, nil
}

