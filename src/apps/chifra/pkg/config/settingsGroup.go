// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

type settingsGroup struct {
	CachePath      string `toml:"cachePath"`
	IndexPath      string `toml:"indexPath"`
	DefaultChain   string `toml:"defaultChain"`
	DefaultGateway string `toml:"defaultGateway,omitempty"`
}

func GetSettings() settingsGroup {
	return GetRootConfig().Settings
}
