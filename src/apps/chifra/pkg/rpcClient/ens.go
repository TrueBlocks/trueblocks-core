// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
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
func GetAddressesFromEns(chain string, addrsIn []string) ([]string, bool) {
	provider := config.GetRpcProvider("mainnet")
	ec := GetClient(provider)
	defer ec.Close()

	found := false
	var out []string
	for i := 0; i < len(addrsIn); i++ {
		term := addrsIn[i]
		if strings.Contains(term, ".eth") {
			val, err := ensGo.Resolve(ec, term)
			if err == nil && len(val) > 0 {
				term = val.Hex()
				found = true
			}
		}
		out = append(out, lowerIfHex(term))
	}
	return out, found
}

// GetAddressFromEns converts a single string, if it contains .eth, into an address. Note, we take
// chain parameter, but ignore it choosing to look at mainnet ENS only
func GetAddressFromEns(chain string, in string) (string, bool) {
	if !strings.Contains(in, ".eth") {
		return lowerIfHex(in), false
	}

	provider := config.GetRpcProvider("mainnet")
	ec := GetClient(provider)
	defer ec.Close()

	val, err := ensGo.Resolve(ec, in)
	if err == nil && len(val) > 0 {
		return lowerIfHex(val.Hex()), true
	}

	return lowerIfHex(in), false
}
