package main

import (
	"encoding/json"
	"fmt"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

func main() {
	chain := "mainnet"
	conn := rpcClient.TempConnection(chain)
	if block, err := conn.GetBlockHeaderByNumber(base.Blknum(3500000)); err != nil {
		fmt.Println(err)
	} else {
		bytes, _ := json.MarshalIndent(block, "", "  ")
		fmt.Println(string(bytes))
	}
}
