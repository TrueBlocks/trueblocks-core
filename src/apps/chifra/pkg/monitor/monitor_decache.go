package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"
	"path/filepath"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
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
		caches := []string{"blocks", "txs", "traces", "recons", "abis"}
		if cont, err := DecacheItems(chain, mon.Address.Hex(), processor, caches, pairs); err != nil || !cont {
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

func DecacheItems(chain, address string, processor func(string) bool, caches []string, apps []base.Pair[uint32, uint32]) (bool, error) {
	lastBlock := 0
	lastBasePath := ""

	for _, cache := range caches {
		logger.Info("Decaching", cache, "for", address, "on", chain, strings.Repeat(" ", 60))
		if cache == "abis" {
			_, path := getCachePath(chain, cache, address, 0, 0)
			if file.FileExists(path) {
				if !processor(path) {
					return false, nil
				}
			}
		} else {
			for index, app := range apps {
				basePath, path := getCachePath(chain, cache, address, app.First, app.Second)
				folderExists := file.FolderExists(basePath)
				if basePath == lastBasePath && !folderExists {
					logger.Progress(index%207 == 0, "Skipping: ", path)
					continue
				}

				switch cache {
				case "recons":
					hasAddr := len(address) > 0
					if hasAddr && !folderExists {
						logger.Progress(index%207 == 0, "Skipping: ", path)
						goto outer
					}
				case "blocks":
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

// TODO: Use Dawid's path code from cache package
// getCachePath returns the path and the basePath for a given cache item depending on type
func getCachePath(chain, typ, addr string, blockNum, txid uint32) (basePath, path string) {
	blkStr := fmt.Sprintf("%09d", blockNum)
	txStr := fmt.Sprintf("%05d", txid)
	part1 := blkStr[0:2]
	part2 := blkStr[2:4]
	part3 := blkStr[4:6]
	part4 := blkStr
	part5 := ""
	// fmt.Println("getCachePath", chain, typ, addr, blockNum, txid, blkStr, txStr, part1, part2, part3, part4, part5)

	cachePath := config.GetPathToCache(chain)
	switch typ {
	case "abis":
		basePath = fmt.Sprintf("%s%s/", cachePath, typ)
		path = fmt.Sprintf("%s%s/%s.json", cachePath, typ, addr)
	case "blocks":
		basePath = fmt.Sprintf("%s%s/%s/%s/%s/", cachePath, typ, part1, part2, part3)
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s%s.bin", cachePath, typ, part1, part2, part3, part4, part5)
	case "txs":
		fallthrough
	case "traces":
		part5 = "-" + txStr
		basePath = fmt.Sprintf("%s%s/%s/%s/", cachePath, typ, part1, part2)
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s%s.bin", cachePath, typ, part1, part2, part3, part4, part5)
	case "recons":
		if len(addr) >= 8 {
			addr = addr[2:]
			part1 := addr[0:4]
			part2 := addr[4:8]
			part3 := addr[8:]
			part4 := blkStr
			part5 := txStr
			basePath = fmt.Sprintf("%s%s/%s/%s/", cachePath, typ, part1, part2)
			path = fmt.Sprintf("%s%s/%s/%s/%s/%s.%s.bin", cachePath, typ, part1, part2, part3, part4, part5)
		}
	default:
		fmt.Println("Unknown type in deleteIfPresent: ", typ)
		os.Exit(1)
	}
	// fmt.Println("getCachePath", basePath, path)

	return basePath, path
}
