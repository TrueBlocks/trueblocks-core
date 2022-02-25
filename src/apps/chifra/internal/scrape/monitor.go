package scrapePkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

type Monitor struct {
	Address string `json:"address"`
	Path    string `json:"path"`
	Count   uint64 `json:"count"`
	Size    uint64 `json:"size"`
}

func NewMonitor() Monitor {
	monitor := new(Monitor)
	return *monitor
}
