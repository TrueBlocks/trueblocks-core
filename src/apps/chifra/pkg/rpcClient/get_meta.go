// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"encoding/json"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/utils"
)

type MetaData struct {
	Latest    uint64 `json:"client"`
	Finalized uint64 `json:"finalized"`
	Staging   uint64 `json:"staging"`
	Ripe      uint64 `json:"ripe"`
	Unripe    uint64 `json:"unripe"`
	ChainId   uint64 `json:"chainId,omitempty"`
	NetworkId uint64 `json:"networkId,omitempty"`
	Chain     string `json:"chain,omitempty"`
}

func (m MetaData) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func (m MetaData) Age(bn uint64) int64 {
	return int64(m.Latest) - int64(bn) // Allows negative
}

func GetMetaData(chain string, testmode bool) (*MetaData, error) {
	provider, _ := config.GetRpcProvider(chain)

	chainId, networkId, err := GetIDs(provider)
	if err != nil {
		return nil, err
	}

	if testmode {
		return &MetaData{
			Unripe:    0xdeadbeef,
			Ripe:      0xdeadbeef,
			Staging:   0xdeadbeef,
			Finalized: 0xdeadbeef,
			Latest:    0xdeadbeef,
			Chain:     chain,
			ChainId:   chainId,
			NetworkId: networkId,
		}, nil
	}

	var meta MetaData
	meta.Chain = chain
	meta.ChainId = chainId
	meta.NetworkId = networkId
	meta.Latest = GetLatestBlockNumber(chain)

	filenameChan := make(chan cache.CacheFileInfo)

	var nRoutines = 4
	go cache.WalkCacheFolder(context.Background(), chain, cache.Index_Bloom, nil, filenameChan)
	go cache.WalkCacheFolder(context.Background(), chain, cache.Index_Staging, nil, filenameChan)
	go cache.WalkCacheFolder(context.Background(), chain, cache.Index_Ripe, nil, filenameChan)
	go cache.WalkCacheFolder(context.Background(), chain, cache.Index_Unripe, nil, filenameChan)

	for result := range filenameChan {
		switch result.Type {
		case cache.Index_Bloom:
			fallthrough
		case cache.Index_Final:
			meta.Finalized = utils.Max(meta.Finalized, result.Range.Last)
		case cache.Index_Staging:
			meta.Staging = utils.Max(meta.Staging, result.Range.Last)
		case cache.Index_Ripe:
			meta.Ripe = utils.Max(meta.Ripe, result.Range.Last)
		case cache.Index_Unripe:
			meta.Unripe = utils.Max(meta.Unripe, result.Range.Last)
		case cache.Cache_NotACache:
			nRoutines--
			if nRoutines == 0 {
				close(filenameChan)
			}
		}
	}

	if meta.Staging == 0 {
		meta.Staging = meta.Finalized
	}
	meta.Ripe = utils.Max(meta.Staging, meta.Ripe)
	meta.Unripe = utils.Max(meta.Ripe, meta.Unripe)

	return &meta, nil
}
