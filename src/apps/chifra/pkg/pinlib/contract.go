package pinlib

import (
	"context"
	"log"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/ethereum/go-ethereum"
)

var unchainedIndexAddr = "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"
var manifestHashEncoding = "0x337f3f32"

func GetManifestCidFromContract() string {
	ethClient := rpcClient.Get()
	defer ethClient.Close()

	address := rpcClient.HexToAddress(unchainedIndexAddr)
	data := rpcClient.DecodeHex(manifestHashEncoding)

	response, err := ethClient.CallContract(
		context.Background(),
		ethereum.CallMsg{
			To:   &address,
			Data: data,
		},
		nil,
	)
	if err != nil {
		log.Fatalln(err)
	}

	cid, err := rpcClient.ParseAbiString(response)

	if err != nil {
		log.Fatalln(err)
	}

	return cid
}
