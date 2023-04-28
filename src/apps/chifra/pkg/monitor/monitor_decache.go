package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

// Decache removes a monitor and all cached data from the cache
func (mon *Monitor) Decache(chain string, processor cache.DecacheFunc) (err error) {
	if mon.IsOpen() {
		defer mon.Close()
	}

	if mon.Count() == 0 {
		return nil
	}

	apps := make([]index.AppearanceRecord, mon.Count())
	err = mon.ReadAppearances(&apps)
	if err != nil {
		return err
	}

	// TODO: This should use go routines
	caches := []string{"blocks", "txs", "traces", "recons", "abis"}
	if cont, err := cache.DecacheItems(chain, mon.Address.Hex(), processor, caches, index.AppsToNumPairs(apps)); err != nil || !cont {
		return err
	}

	// Clean up the stage if there's anything there
	path := filepath.Join(config.GetApiProvider(chain), "monitors/staging/", mon.Address.Hex()+".mon.bin")
	if file.FileExists(path) {
		err = os.Remove(path)
	}

	return err
}
