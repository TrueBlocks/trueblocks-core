// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpc

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
	ensGo "github.com/wealdtech/go-ens/v3"
)

// GetEnsAddresses converts an array of strings, if they contains .eth, into addresses. Note, we take
// chain parameter, but ignore it choosing to look at mainnet ENS only
func (conn *Connection) GetEnsAddresses(addrs []string) (out []string, found bool) {
	has := false
	for _, addr := range addrs {
		if strings.Contains(addr, ".eth") {
			has = true
			break
		}
	}
	if !has {
		for _, term := range addrs {
			out = append(out, utils.LowerIfHex(term))
		}
		return out, false
	}

	// Note: we use ENS on mainnet always
	tc := TempConnection("mainnet")
	if ec, err := tc.getClient(); err != nil {
		return
	} else {
		defer ec.Close()
		for _, term := range addrs {
			if strings.Contains(term, ".eth") {
				if val, err := ensGo.Resolve(ec, term); err == nil && len(val) > 0 {
					term = val.Hex()
					found = true
				}
			}
			out = append(out, utils.LowerIfHex(term))
		}
		return
	}
}

// GetEnsAddress converts a single string, if it contains .eth, into an address. Note, we take
// chain parameter, but ignore it choosing to look at mainnet ENS only
func (conn *Connection) GetEnsAddress(addr string) (string, bool) {
	if !strings.Contains(addr, ".eth") {
		return utils.LowerIfHex(addr), false
	}

	// Note: we use ENS on mainnet always
	tc := TempConnection("mainnet")
	if ec, err := tc.getClient(); err != nil {
		return "", false
	} else {
		defer ec.Close()
		if val, err := ensGo.Resolve(ec, addr); err == nil && len(val) > 0 {
			return utils.LowerIfHex(val.Hex()), true
		}
		return utils.LowerIfHex(addr), false
	}
}

// IsSame returns true if the two strings are the same, ignoring case.
// If not equal, it also tried to interpret the strings as addresses using ENS.
func IsSame(a string, b string) bool {
	if strings.EqualFold(a, b) {
		return true
	}
	if !strings.HasSuffix(b, ".eth") {
		return false
	}
	unused := TempConnection("mainnet")
	ensAddr, _ := unused.GetEnsAddress(b)
	return ensAddr == a
}
