// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"encoding/binary"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/index"
)

func (opts *ChunksOptions) showBloom(path string, first bool) {
	if opts.Globals.TestMode {
		r, _ := cache.RangeFromFilename(path)
		if r.First > 1 {
			return
		}
	}

	var bloom index.ChunkBloom
	readBloom(&bloom, path)
	bloom.Display(int(opts.Globals.LogLevel))
}

//---------------------------------------------------------------------------
func readBloom(bloom *index.ChunkBloom, fileName string) (err error) {
	bloom.Range, err = cache.RangeFromFilename(fileName)
	if err != nil {
		return err
	}

	bloom.File, err = os.OpenFile(fileName, os.O_RDONLY, 0644)
	if err != nil {
		return err
	}
	defer bloom.File.Close()

	err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Count)
	if err != nil {
		return err
	}

	bloom.Blooms = make([]index.BloomBytes, bloom.Count)
	for i := uint32(0); i < bloom.Count; i++ {
		// fmt.Println("nBlooms:", bloom.Count)
		err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Blooms[i].NInserted)
		if err != nil {
			return err
		}
		// fmt.Println("nInserted:", bloom.Blooms[i].NInserted)
		bloom.Blooms[i].Bytes = make([]byte, index.BLOOM_WIDTH_IN_BYTES)
		err = binary.Read(bloom.File, binary.LittleEndian, &bloom.Blooms[i].Bytes)
		if err != nil {
			return err
		}
	}

	return nil
}
