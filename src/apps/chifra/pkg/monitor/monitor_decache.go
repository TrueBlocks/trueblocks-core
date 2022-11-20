package monitor

// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
	"github.com/ethereum/go-ethereum/common/hexutil"
)

// Decache removes a monitor and all cached data from the cache
func (mon *Monitor) Decache(chain string, processor func(string) bool) (err error) {
	if mon.Count() == 0 {
		return nil
	}
	defer mon.Close()

	apps := make([]index.AppearanceRecord, mon.Count())
	err = mon.ReadAppearances(&apps)
	if err != nil {
		return err
	}

	caches := []string{"blocks", "txs", "traces", "recons"}
	for _, cache := range caches {
		for _, app := range apps {
			path := getCachePath(chain, cache, hexutil.Encode(mon.Address.Bytes()), app.BlockNumber, app.TransactionId)
			if file.FileExists(path) {
				if !processor(path) {
					return nil
				}
			}
		}
	}

	return err
}

// TODO: Use Dawid's path code from cache package
// getCachePath removes an item from the cache if its present, silently fails otherwise
func getCachePath(chain, typ, addr string, blockNum, txid uint32) string {
	path := ""
	blkStr := fmt.Sprintf("%09d", blockNum)
	txStr := fmt.Sprintf("%05d", txid)
	part1 := blkStr[0:2]
	part2 := blkStr[2:4]
	part3 := blkStr[4:6]
	part4 := blkStr
	part5 := ""

	switch typ {
	case "blocks":
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s%s.bin", config.GetPathToCache(chain), typ, part1, part2, part3, part4, part5)
	case "txs":
		fallthrough
	case "traces":
		part5 = "-" + txStr
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s%s.bin", config.GetPathToCache(chain), typ, part1, part2, part3, part4, part5)
	case "recons":
		addr = addr[2:]
		part1 := addr[0:4]
		part2 := addr[4:8]
		part3 := addr[8:]
		part4 := blkStr
		part5 := txStr
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s.%s.bin", config.GetPathToCache(chain), typ, part1, part2, part3, part4, part5)
	default:
		fmt.Println("Unknown type in deleteIfPresent: ", typ)
		os.Exit(1)
	}
	return path
}
