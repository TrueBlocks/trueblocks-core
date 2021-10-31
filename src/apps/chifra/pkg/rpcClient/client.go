package rpcClient

import (
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/common/hexutil"
	"github.com/ethereum/go-ethereum/ethclient"
)

// Get sets up a client instance and returns it
func Get() ethclient.Client {
	client, err := ethclient.Dial(config.ReadGlobal().Settings.RpcProvider)
	if err != nil {
		log.Fatalln(err)
	}

	return *client
}

// HexToAddress converts a string with hex to go-ethereum's common.Address
func HexToAddress(hex string) common.Address {
	return common.HexToAddress(hex)
}

// DecodeHex decodes a string with hex into a slice of bytes
func DecodeHex(hex string) []byte {
	return hexutil.MustDecode(hex)
}
