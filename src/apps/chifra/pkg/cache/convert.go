// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

import "strconv"

func Convert(blockStrs []string) []uint64 {
	var ret []uint64
	for _, blockStr := range blockStrs {
		val, _ := strconv.ParseUint(blockStr, 10, 64)
		ret = append(ret, val)
	}
	return ret
}
