package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"os"
	"path/filepath"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
)

// Decache removes a monitor and all cached data from the cache
func (mon *Monitor) Decache(chain string, processor cache.DecacheFunc) error {
	if mon.IsOpen() {
		defer mon.Close()
	}

	if apps, cnt, err := mon.ReadAndFilterAppearances(NewEmptyFilter(chain)); err != nil {
		return err
	} else if cnt == 0 {
		return nil
	} else {
		pairs := make([]base.NumPair[uint32], 0, len(apps))
		for _, app := range apps {
			pairs = append(pairs, base.NumPair[uint32]{N1: app.BlockNumber, N2: app.TransactionIndex})
		}

		// TODO: This should use go routines
		// TODO: BOGUS PERF
		// caches := []string{"blocks", "txs", "traces", "recons"}
		caches := []string{"blocks", "txs", "traces", "recons", "abis"}
		if cont, err := cache.DecacheItems(chain, mon.Address.Hex(), processor, caches, pairs); err != nil || !cont {
			return err
		}

		// Clean up the stage if there's anything there
		path := filepath.Join(config.GetApiProvider(chain), "monitors/staging/", mon.Address.Hex()+".mon.bin")
		if file.FileExists(path) {
			return os.Remove(path)
		}
	}

	return nil
}
