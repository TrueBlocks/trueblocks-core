package cache

import (
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/file"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/logger"
)

type DecacheFunc func(string) bool

func DecacheItems(chain, address string, processor DecacheFunc, caches []string, apps []base.NumPair[uint32]) (bool, error) {
	for index, cache := range caches {
		for _, app := range apps {
			basePath, path := getCachePath(chain, cache, address, app.N1, app.N2)
			hasAddr := len(address) > 0
			isRecon := cache == "recons"
			folderExists := file.FolderExists(basePath)
			if hasAddr && isRecon && !folderExists {
				goto outer
			} else if !folderExists {
				continue
			}

			if file.FileExists(path) {
				if !processor(path) {
					return false, nil
				}
			} else {
				logger.Progress(index%20 == 0, "Already removed:", path)
			}
		}
	outer:
	}
	return true, nil
}

// TODO: Use Dawid's path code from cache package
// getCachePath removes an item from the cache if its present, silently fails otherwise
func getCachePath(chain, typ, addr string, blockNum, txid uint32) (basePath, path string) {
	blkStr := fmt.Sprintf("%09d", blockNum)
	txStr := fmt.Sprintf("%05d", txid)
	part1 := blkStr[0:2]
	part2 := blkStr[2:4]
	part3 := blkStr[4:6]
	part4 := blkStr
	part5 := ""

	switch typ {
	case "abis":
		basePath = fmt.Sprintf("%s%s/", config.GetPathToCache(chain), typ)
		path = fmt.Sprintf("%s%s/%s.json", config.GetPathToCache(chain), typ, addr)
	case "blocks":
		basePath = fmt.Sprintf("%s%s/%s/%s/", config.GetPathToCache(chain), typ, part1, part2)
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s%s.bin", config.GetPathToCache(chain), typ, part1, part2, part3, part4, part5)
	case "txs":
		fallthrough
	case "traces":
		part5 = "-" + txStr
		basePath = fmt.Sprintf("%s%s/%s/%s/", config.GetPathToCache(chain), typ, part1, part2)
		path = fmt.Sprintf("%s%s/%s/%s/%s/%s%s.bin", config.GetPathToCache(chain), typ, part1, part2, part3, part4, part5)
	case "recons":
		if len(addr) >= 8 {
			addr = addr[2:]
			part1 := addr[0:4]
			part2 := addr[4:8]
			part3 := addr[8:]
			part4 := blkStr
			part5 := txStr
			basePath = fmt.Sprintf("%s%s/%s/%s/", config.GetPathToCache(chain), typ, part1, part2)
			path = fmt.Sprintf("%s%s/%s/%s/%s/%s.%s.bin", config.GetPathToCache(chain), typ, part1, part2, part3, part4, part5)
		}
	default:
		fmt.Println("Unknown type in deleteIfPresent: ", typ)
		os.Exit(1)
	}

	return basePath, path
}
