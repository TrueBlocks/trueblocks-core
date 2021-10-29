package pinlib

import (
	"context"
	"fmt"
	"os"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/abi"
)

var unchainedIndexAddr = "0xcfd7f3b24f3551741f922fd8c4381aa4e00fc8fd"
var manifestHashEncoding = "0x337f3f32"

func GetManifestCidFromContract() (string, error) {
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
		return "", fmt.Errorf("while calling contract: %w", err)
	}

	abiSource, err := os.Open(
		config.GetConfigPath("abis/known-000/unchained.json"),
	)
	if err != nil {
		return "", fmt.Errorf("while reading contract ABI: %w", err)
	}

	contractAbi, err := abi.JSON(abiSource)
	if err != nil {
		return "", fmt.Errorf("while parsing contract ABI: %w", err)
	}

	unpacked, err := contractAbi.Unpack("manifestHash", response)
	if err != nil {
		return "", fmt.Errorf("while unpacking value: %w", err)
	}

	return unpacked[0].(string), nil
}
