// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package config

type historyGroup struct {
	Comment string `toml:"comment"`
	Init    string `toml:"init,omitempty"`
}

func GetHistory() historyGroup {
	return historyGroup{} // GetRootConfig().History
}
