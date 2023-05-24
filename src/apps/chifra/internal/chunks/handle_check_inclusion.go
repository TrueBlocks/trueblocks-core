// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package chunksPkg

import (
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/manifest"
)

// CheckInclusion digs deep into the index chunk and makes sure that all addresses in the chunk return
// true when tested against the bloom filter. It requires an IPFS node to be running locally.
func (opts *ChunksOptions) CheckInclusion(man *manifest.Manifest, report *simpleReportCheck) error {
	// for _, chunk := range man.Chunks {
	// 	report.VisitedCnt++
	// 	report.CheckedCnt++
	// 	utils.IterateOverMap(chunk.Range, func(addr string) {
	// 		fmt.Println(addr)
	// 	})
	// 	// bloomFilter := index.GetBloomFilterAt(chunk.Range)
	// 	// err := index.ForEveryAddressInChunk(chunk.Range, func(addr base.Address) error {
	// 	// 	fmt.Println(addr)
	// 	// 	if !bloomFilter.Test(addr.Bytes()) {
	// 	// 		msg := fmt.Sprintf("%s: Address (%s) not in BloomFilter", chunk.Range, addr)
	// 	// 		report.MsgStrings = append(report.MsgStrings, msg)
	// 	// 	} else {
	// 	// 		report.PassedCnt++
	// 	// 	}
	// 	// 	return nil
	// 	// })
	// }

	return nil
}

// func bloomPathFromRange(fileRange base.FileRange) (string, error) {
// 	tmp := cache.New CachePath("mainnet", cache.Index_Final)
// 	path := tmp.Get FullPath(cache.FilenameFromRange(fileRange, ""))
// 	if file.FileExists(path) {
// 		return path, nil
// 	}
// 	return path, errors.New("file path missing: " + path)
// }
