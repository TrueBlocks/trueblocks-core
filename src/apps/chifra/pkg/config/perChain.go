// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

// GetRpcProvider returns the RPC provider for a chain
func GetRpcProvider(chain string) string {
	// TODO: BOGUS-RPC PROVIDER
	provider := GetRootConfig().Settings.RpcProvider
	return provider
}

func GetPinGateway(chain string) string {
	gateway := GetRootConfig().Settings.PinGateway
	return gateway
}
