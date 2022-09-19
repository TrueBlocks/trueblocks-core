// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package paths

type CacheType uint

const (
	None CacheType = iota
	Index_Bloom
	Index_Final
	Index_Staging
	Index_Ripe
	Index_Unripe
)

func (ct CacheType) String() string {
	descrs := map[CacheType]string{
		None:          "unknown",
		Index_Bloom:   "bloom",
		Index_Final:   "index",
		Index_Staging: "staging",
		Index_Ripe:    "ripe",
		Index_Unripe:  "unripe",
	}
	return descrs[ct]
}
