// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"context"
	"encoding/json"
)

type Meta struct {
	Unripe    uint64 `json:"unripe"`
	Ripe      uint64 `json:"ripe"`
	Staging   uint64 `json:"staging"`
	Finalized uint64 `json:"finalized"`
	Latest    uint64 `json:"client"`
}

func (m Meta) String() string {
	ret, _ := json.MarshalIndent(m, "", "  ")
	return string(ret)
}

func GetMeta(testMode bool) *Meta {
	if testMode {
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
	bn, _ := ethClient.BlockNumber(context.Background())

	// TODO: This needs to get filled in with the rest of the data
	return &Meta{Latest: bn}
}
