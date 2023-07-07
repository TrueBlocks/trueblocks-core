package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"os"
	"path/filepath"
	"sort"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

type AppearanceSort int

const (
	NotSorted AppearanceSort = iota
	Sorted
	Reversed
)

func (mon *Monitor) ReadAppearances2(sortBy AppearanceSort) (apps []index.AppearanceRecord, cnt int, err error) {
	if mon.Count() == 0 {
		return nil, 0, nil
	}

	apps = make([]index.AppearanceRecord, mon.Count())
	if err := mon.ReadAppearances(&apps); err != nil {
		return nil, 0, err
	} else if len(apps) == 0 {
		return nil, 0, nil
	}

	Sort(apps, sortBy)

	return apps, len(apps), nil
}

func Sort(apps []index.AppearanceRecord, sortBy AppearanceSort) {
	if sortBy == Sorted || sortBy == Reversed {
		sort.Slice(apps, func(i, j int) bool {
			si := (uint64(apps[i].BlockNumber) << 32) + uint64(apps[i].TransactionId)
			sj := (uint64(apps[j].BlockNumber) << 32) + uint64(apps[j].TransactionId)
			if sortBy == Reversed {
				return sj < si
			} else {
				return si < sj
			}
		})
	}
}

// Decache removes a monitor and all cached data from the cache
func (mon *Monitor) Decache(chain string, processor cache.DecacheFunc) (err error) {
	if mon.IsOpen() {
		defer mon.Close()
	}

	apps, cnt, err := mon.ReadAppearances2(NotSorted)
	if err != nil {
		return err
	} else if cnt == 0 {
		return nil
	} else {
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
	}

	return err
}
