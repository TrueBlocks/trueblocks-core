package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/walk"
)

// Decache removes a monitor and all cached data from the cache
func (mon *Monitor) Decache(chain string, processor func(string) bool) error {
	if mon.IsOpen() {
		defer mon.Close()
	}

	if apps, cnt, err := mon.ReadAndFilterAppearances(NewEmptyFilter(chain)); err != nil {
		return err
	} else if cnt == 0 {
		return nil
	} else {
		pairs := make([]base.Pair[uint32, uint32], 0, len(apps))
		for _, app := range apps {
			pairs = append(pairs, base.Pair[uint32, uint32]{First: app.BlockNumber, Second: app.TransactionIndex})
		}

		// TODO: This should use go routines
		caches := []walk.CacheType{
			walk.Cache_Blocks,
			walk.Cache_CallResults,
			walk.Cache_Logs,
			walk.Cache_Slurps,
			walk.Cache_State,
			walk.Cache_Statements,
			walk.Cache_Tokens,
			walk.Cache_Traces,
			walk.Cache_Transactions,
			walk.Cache_Abis}
		if cont, err := DecacheItems(chain, mon.Address, caches, pairs, processor); err != nil || !cont {
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

func DecacheItems(
	chain string,
	address base.Address,
	caches []walk.CacheType,
	apps []base.Pair[uint32, uint32],
	processor func(string) bool,
) (bool, error) {
	lastBlock := 0
	lastBasePath := ""

	for _, cache := range caches {
		logger.Info("Decaching", cache, "for", address, "on", chain, strings.Repeat(" ", 60))
		if cache == walk.Cache_Abis {
			_, path := walk.GetDecachePath(chain, cache, address, 0, 0)
			if file.FileExists(path) {
				if !processor(path) {
					return false, nil
				}
			}
		} else {
			for index, app := range apps {
				basePath, path := walk.GetDecachePath(chain, cache, address, app.First, app.Second)
				folderExists := file.FolderExists(basePath)
				if basePath == lastBasePath && !folderExists {
					logger.Progress(index%207 == 0, "Skipping: ", path)
					continue
				}

				switch cache {
				case walk.Cache_CallResults:
					fallthrough
				case walk.Cache_Slurps:
					fallthrough
				case walk.Cache_State:
					fallthrough
				case walk.Cache_Statements:
					fallthrough
				case walk.Cache_Tokens:
					hasAddr := !address.IsZero()
					if hasAddr && !folderExists {
						logger.Progress(index%207 == 0, "Skipping: ", path)
						goto outer
					}
				case walk.Cache_Logs:
					fallthrough
				case walk.Cache_Blocks:
					if lastBlock > 0 && app.First == uint32(lastBlock) {
						logger.Progress(index%207 == 0, "Skipping: ", path)
						continue
					}
				default:
					// do nothing
				}
				lastBlock = int(app.First)
				lastBasePath = basePath

				if folderExists && file.FileExists(path) {
					if !processor(path) {
						return false, nil
					}
				} else {
					logger.Progress(index%207 == 0, "Skipping: ", path)
				}
			}
		}
	outer:
	}
	return true, nil
}
