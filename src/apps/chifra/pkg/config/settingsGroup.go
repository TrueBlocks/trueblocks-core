// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

type settingsGroup struct {
	CachePath      string      `toml:"cachePath" comment:"The location of the per chain caches"`
	IndexPath      string      `toml:"indexPath" comment:"The location of the per chain unchained indexes"`
	DefaultChain   string      `toml:"defaultChain" comment:"The default chain to use if none is provided"`
	DefaultGateway string      `toml:"defaultGateway,omitempty"`
	Notify         notifyGroup `toml:"notify"`
}

func GetSettings() settingsGroup {
	return GetRootConfig().Settings
}
