// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package rpcClient

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
)

// GetUncleCountByNumber returns the number of uncles in a block.
func GetUncleCountByNumber(chain string, bn uint64) (uint64, error) {
	method := "eth_getUncleCountByBlockNumber"
	params := rpc.Params{fmt.Sprintf("0x%x", bn)}

	if result, err := rpc.Query[string](chain, method, params); err != nil {
		return 0, err
	} else {
		return strconv.ParseUint(fmt.Sprint(*result), 0, 64)
	}
}
