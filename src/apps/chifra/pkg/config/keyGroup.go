// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

type keyGroup struct {
	License string `toml:"license,omitempty"`
	ApiKey  string `toml:"apiKey"`
	Secret  string `toml:"secret,omitempty"`
	Jwt     string `toml:"jwt,omitempty"`
}

func GetPinningKeys(chain string) (string, string, string) {
	keys := GetRootConfig().Keys
	a := keys["pinata"].ApiKey
	b := keys["pinata"].Secret
	c := keys["estuary"].ApiKey
	return a, b, c
}

func HasPinningKeys(chain string) bool {
	a, b, c := GetPinningKeys(chain)
	return len(a)+len(b)+len(c) > 0
}

func HasEsKeys(chain string) bool {
	keys := GetRootConfig().Keys
	return len(keys["etherscan"].ApiKey) > 0
}
