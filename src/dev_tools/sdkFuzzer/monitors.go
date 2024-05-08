package main

import (
	"github.com/TrueBlocks/trueblocks-core/sdk"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// DoMonitors tests the monitors sdk function
func DoMonitors() {
	opts := sdk.MonitorsOptions{
		Addrs: []string{testAddrs[0]},
	}
	ShowHeader("DoMonitors", &opts)

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
}
