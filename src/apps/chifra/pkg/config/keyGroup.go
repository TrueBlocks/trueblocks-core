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

func GetKey(set string) keyGroup {
	return GetRootConfig().Keys[set]
}
