// Copyright 2016, 2024 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.
/*
 * Parts of this file were auto generated. Edit only those parts of
 * the code inside of 'EXISTING_CODE' tags.
 */
package main

// EXISTING_CODE
import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// EXISTING_CODE

// DoMonitors tests the Monitors sdk function
func DoMonitors() {
	file.EstablishFolder("sdkFuzzer-output/monitors")
	opts := sdk.MonitorsOptions{}
	ShowHeader("DoMonitors", opts)

	// EXISTING_CODE
	opts = sdk.MonitorsOptions{
		Addrs: []string{testAddrs[0]},
	}
	// monitors,command,default|caching|
	// Addrs     []string `json:"addrs,omitempty"`
	// Delete    bool     `json:"delete,omitempty"`
	// Undelete  bool     `json:"undelete,omitempty"`
	// Remove    bool     `json:"remove,omitempty"`
	// Watch     bool     `json:"watch,omitempty"`
	// Watchlist string   `json:"watchlist,omitempty"`
	// Commands  string   `json:"commands,omitempty"`
	// BatchSize uint64   `json:"batchSize,omitempty"`
	// RunCount  uint64   `json:"runCount,omitempty"`
	// Sleep     float64  `json:"sleep,omitempty"`
	// func (opts *MonitorsOptions) Monitors() ([]bool, *types.MetaData, error) {
	// func (opts *MonitorsOptions) MonitorsClean() ([]types.MonitorClean, *types.MetaData, error) {
	// func (opts *MonitorsOptions) MonitorsList() ([]types.Monitor, *types.MetaData, error) {

	if monitors, _, err := opts.MonitorsList(); err != nil {
		logger.Error(err)
	} else {
		if err := SaveAndClean[types.Monitor]("sdkFuzzer/monitors.json", monitors, &opts, func() error {
			_, _, err := opts.MonitorsList()
			return err
		}); err != nil {
			logger.Error(err)
		}
	}

	// opts.List = false
	// if monitors, _, err := opts.Monitors(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[bool]("sdkFuzzer/monitors.json", monitors, &opts, func() error {
	// 		_, _, err := opts.Monitors()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }

	// if monitors, _, err := opts.MonitorsClean(); err != nil {
	// 	logger.Error(err)
	// } else {
	// 	if err := SaveAndClean[types.MonitorClean]("sdkFuzzer/monitors.json", monitors, &opts, func() error {
	// 		_, _, err := opts.MonitorsClean()
	// 		return err
	// 	}); err != nil {
	// 		logger.Error(err)
	// 	}
	// }
	// EXISTING_CODE
	Wait()
}

func TestMonitors(which, value, fn string, opts *sdk.MonitorsOptions) {
	// fn = strings.Replace(fn, ".json", "-"+which+".json", 1)
	switch which {
	}
}

// EXISTING_CODE
// EXISTING_CODE
