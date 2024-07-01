// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

type versionGroup struct {
	Current string `toml:"current" comment:"Do not edit"`
}

func GetVersion() versionGroup {
	return GetRootConfig().Version
}
