// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"encoding/json"
	"errors"
	"io/fs"
	"math"
	"path/filepath"
	"strconv"
	"strings"
	"sync"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
)

type Meta struct {
	Latest    uint64 `json:"client"`
	Finalized uint64 `json:"finalized"`
	Staging   uint64 `json:"staging"`
	Ripe      uint64 `json:"ripe"`
	Unripe    uint64 `json:"unripe"`
}

func (m Meta) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

type MetaValue struct {
	folder string
	value  uint64
}

func GetMeta(testmode bool) *Meta {
	if testmode {
		return &Meta{
			Unripe:    0xdeadbeef,
			Ripe:      0xdeadbeef,
			Staging:   0xdeadbeef,
			Finalized: 0xdeadbeef,
			Latest:    0xdeadbeef,
		}
	}

	ethClient := Get()
	defer ethClient.Close()

	var meta Meta
	meta.Latest, _ = ethClient.BlockNumber(context.Background())

	valueChan := make(chan MetaValue)

	var wg sync.WaitGroup
	var nRoutines int = 4
	wg.Add(nRoutines)
	go walkIndexFolder(&wg, "finalized", valueChan)
	go walkIndexFolder(&wg, "staging", valueChan)
	go walkIndexFolder(&wg, "ripe", valueChan)
	go walkIndexFolder(&wg, "unripe", valueChan)

	for result := range valueChan {
		if strings.Contains(result.folder, "done-") {

		} else {
			switch result.folder {
			case "finalized":
				meta.Finalized = uint64(math.Max(float64(meta.Finalized), float64(result.value)))
			case "staging":
				meta.Staging = uint64(math.Max(float64(meta.Staging), float64(result.value)))
			case "ripe":
				meta.Ripe = uint64(math.Max(float64(meta.Ripe), float64(result.value)))
			case "unripe":
				meta.Unripe = uint64(math.Max(float64(meta.Unripe), float64(result.value)))
			case "done":
				nRoutines--
				if nRoutines == 0 {
					close(valueChan)
				}
			}
		}
	}

	wg.Wait()

	if meta.Staging == 0 {
		meta.Staging = meta.Finalized
	}
	meta.Ripe = uint64(math.Max(float64(meta.Staging), float64(meta.Ripe)))
	meta.Unripe = uint64(math.Max(float64(meta.Ripe), float64(meta.Unripe)))

	return &meta
}

func walkIndexFolder(wg *sync.WaitGroup, folder string, valueChan chan<- MetaValue) {
	defer func() {
		valueChan <- MetaValue{folder: "done"}
		wg.Done()
	}()

	filepath.Walk(config.ReadTrueBlocks().Settings.IndexPath+folder, func(path string, info fs.FileInfo, err error) error {
		if err != nil {
			// If the scraper is running, this will sometimes send an error for a file, for example, that existed
			// when it was first seen, but the scraper deletes before this call. We ignore any file system errors
			// this routine, but if we expect problems, we can uncomment this line
			// fmt.Printf("prevent panic by handling failure accessing a path %q: %v\n", path, err)
			return err
		}
		if !info.IsDir() {
			_, last, _ := fn_2_Vals(path)
			// fmt.Println(" <-- ", folder, last)
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
