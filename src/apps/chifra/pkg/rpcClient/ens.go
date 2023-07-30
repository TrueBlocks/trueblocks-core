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
func GetAddressesFromEns(chain string, addrsIn []string) (out []string, found bool) {
	// Note: we use ENS on mainnet always
	if ec, err := GetClient("mainnet"); err != nil {
		return
	} else {
		defer ec.Close()
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
		return
	}
}

// GetAddressFromEns converts a single string, if it contains .eth, into an address. Note, we take
// chain parameter, but ignore it choosing to look at mainnet ENS only
func GetAddressFromEns(chain string, in string) (string, bool) {
	if !strings.Contains(in, ".eth") {
		return lowerIfHex(in), false
	}

	// Note: we use ENS on mainnet always
	if ec, err := GetClient("mainnet"); err != nil {
		return "", false
	} else {
		defer ec.Close()
		val, err := ensGo.Resolve(ec, in)
		if err == nil && len(val) > 0 {
			return lowerIfHex(val.Hex()), true
		}
		return lowerIfHex(in), false
	}
}
