// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package validate

import "strconv"

func Convert(blockStrs []string) []uint64 {
	// TODO: This needs to handle string values such as specials and latest/first, etc.
	var ret []uint64
	for _, item := range blockStrs {
		val, _ := strconv.ParseUint(item, 10, 64)
		ret = append(ret, val)
	}
	return ret
}
