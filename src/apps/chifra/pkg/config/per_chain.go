// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

import (
	"strings"
)

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider(chain string) string {
	ch := GetRootConfig().Chains[chain]
	return ch.RpcProvider
}

// GetPinGateway returns the pin gateway for a chain
func GetPinGateway(chain string) string {
	ch := GetRootConfig().Chains[chain]
	gateway := ch.PinGateway
	if !strings.HasPrefix(gateway, "http") {
		gateway = "https://" + gateway
	}
	return gateway
}
