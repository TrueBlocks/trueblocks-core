// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"strings"

	ensGo "github.com/wealdtech/go-ens/v3"
)

func lowerIfHex(addr string) string {
	if !strings.HasPrefix(addr, "0x") {
		return addr
	}
	return strings.ToLower(addr)
}

// GetAddressesFromEns converts an array of strings, if they contains .eth, into addresses. Note, we take
// chain parameter, but ignore it choosing to look at mainnet ENS only
func (conn *Connection) GetAddressesFromEns(addrs []string) (out []string, found bool) {
	// Note: we use ENS on mainnet always
	if ec, err := getClient("mainnet"); err != nil {
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
			out = append(out, lowerIfHex(term))
		}
		return
	}
}

// GetAddressFromEns converts a single string, if it contains .eth, into an address. Note, we take
// chain parameter, but ignore it choosing to look at mainnet ENS only
func (conn *Connection) GetAddressFromEns(addr string) (string, bool) {
	if !strings.Contains(addr, ".eth") {
		return lowerIfHex(addr), false
	}

	// Note: we use ENS on mainnet always
	if ec, err := getClient("mainnet"); err != nil {
		return "", false
	} else {
		defer ec.Close()
		if val, err := ensGo.Resolve(ec, addr); err == nil && len(val) > 0 {
			return lowerIfHex(val.Hex()), true
		}
		return lowerIfHex(addr), false
	}
}
