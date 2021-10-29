package rpcClient

import (
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// Setups a client instance and returns it
func Get() ethclient.Client {
	client, err := ethclient.Dial(config.ReadGlobal().Settings.RpcProvider)
	if err != nil {
		log.Fatalln(err)
	}

	return *client
}

func HexToAddress(hex string) common.Address {
	return common.HexToAddress(hex)
}

func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}
