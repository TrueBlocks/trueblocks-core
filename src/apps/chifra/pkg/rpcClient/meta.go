// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"encoding/json"
	"errors"
	"io/fs"
	"path/filepath"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type MetaData struct {
	Latest    uint64 `json:"client"`
	Finalized uint64 `json:"finalized"`
	Staging   uint64 `json:"staging"`
	Ripe      uint64 `json:"ripe"`
	Unripe    uint64 `json:"unripe"`
	ChainId   uint64 `json:"chainId"`
	NetworkId uint64 `json:"networkId"`
	Chain     string `json:"chain"`
}

func (m MetaData) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

type MetaValue struct {
	folder string
	value  uint64
}

func GetMetaData(chain string, testmode bool) *MetaData {
	ethClient := Get(config.GetRpcProvider(chain))
	defer ethClient.Close()

	if testmode {
		return &MetaData{
			Unripe:    0xdeadbeef,
			Ripe:      0xdeadbeef,
			Staging:   0xdeadbeef,
			Finalized: 0xdeadbeef,
			Latest:    0xdeadbeef,
			Chain:     chain,
			ChainId:   ChainID(&ethClient),
			NetworkId: NetworkID(&ethClient),
		}
	}

	var meta MetaData
	meta.Chain = chain
	meta.ChainId = ChainID(&ethClient)
	meta.NetworkId = NetworkID(&ethClient)
	meta.Latest = BlockNumber(&ethClient)

	valueChan := make(chan MetaValue)

	var nRoutines int = 4
	go walkIndexFolder(chain, "finalized", valueChan)
	go walkIndexFolder(chain, "staging", valueChan)
	go walkIndexFolder(chain, "ripe", valueChan)
	go walkIndexFolder(chain, "unripe", valueChan)

	for result := range valueChan {
		if strings.Contains(result.folder, "done-") {

		} else {
			switch result.folder {
			case "finalized":
				meta.Finalized = utils.Max(meta.Finalized, result.value)
			case "staging":
				meta.Staging = utils.Max(meta.Staging, result.value)
			case "ripe":
				meta.Ripe = utils.Max(meta.Ripe, result.value)
			case "unripe":
				meta.Unripe = utils.Max(meta.Unripe, result.value)
			case "done":
				nRoutines--
				if nRoutines == 0 {
					close(valueChan)
				}
			}
		}
	}

	if meta.Staging == 0 {
		meta.Staging = meta.Finalized
	}
	meta.Ripe = utils.Max(meta.Staging, meta.Ripe)
	meta.Unripe = utils.Max(meta.Ripe, meta.Unripe)

	return &meta
}

func walkIndexFolder(chain, folder string, valueChan chan<- MetaValue) {
	defer func() {
		valueChan <- MetaValue{folder: "done"}
	}()

	filepath.Walk(config.GetPathToIndex(chain)+folder, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we experience problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}
		if !info.IsDir() {
			_, last, _ := fn_2_Vals(path)
			valueChan <- MetaValue{folder: folder, value: last}
		}
		return nil
	})
}

func fn_2_Vals(path string) (uint64, uint64, error) {
	if !strings.Contains(path, ".") {
		return 0, 0, errors.New("invalid filename")
	}
	_, fn := filepath.Split(path)
	if strings.Contains(fn, ".") {
		fn = strings.Split(fn, ".")[0]
	}
	parts := strings.Split(fn, "-")
	var first, last int
	if len(parts) > 1 {
		first, _ = strconv.Atoi(parts[0])
		last, _ = strconv.Atoi(parts[1])
	} else {
		first = 0
		last, _ = strconv.Atoi(parts[0])
	}

	return uint64(first), uint64(last), nil
}
