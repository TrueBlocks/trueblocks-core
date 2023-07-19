package main

import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cacheNew"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func main() {
	if block, err := rpcClient.GetBlockByNumber("mainnet", base.Blknum(3500000), cacheNew.NoCache); err != nil {
		fmt.Println(err)
	} else {
		bytes, _ := json.MarshalIndent(block, "", "  ")
		fmt.Println(string(bytes))
	}
}
