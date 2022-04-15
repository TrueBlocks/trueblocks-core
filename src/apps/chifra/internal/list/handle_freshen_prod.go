package listPkg

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

// moveAllToProduction completes the update by moving the monitor files from ./cache/<chain>/monitors/staging to
// ./cache/<chain>/monitors.
func (updater *MonitorUpdate) moveAllToProduction() error {
	for _, mon := range updater.MonitorMap {
		err := mon.MoveToProduction()
		if err != nil {
			return err
		}
	}
	return nil
}
