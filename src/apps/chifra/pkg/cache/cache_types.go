// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package cache

type CacheType uint

const (
	BloomChunk CacheType = iota
	IndexChunk
	BlockCache
	TxCache
	TraceCache
	MonitorCache
	NeighborCache
	ReconCache
)
