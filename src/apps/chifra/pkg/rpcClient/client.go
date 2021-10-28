package rpcClient

import (
	"bytes"
	"encoding/hex"
	"log"
	"strconv"

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

// Reads a string from the bytes returned by RPC
func ParseAbiString(input []byte) (string, error) {
	// target is a placeholder where we will keep values of
	// string position and length
	target := make([]byte, 32)
	reader := bytes.NewReader(input)
	// First 32 bytes inform where the string length is
	_, err := reader.Read(target)
	if err != nil {
		return "", err
	}
	position, err := strconv.ParseInt(hex.EncodeToString(target), 16, 32)
	if err != nil {
		return "", err
	}

	// Another 32 bytes are the string length
	_, err = reader.ReadAt(target, position)
	if err != nil {
		return "", err
	}
	strlen, err := strconv.ParseInt(hex.EncodeToString(target), 16, 32)
	if err != nil {
		return "", err
	}

	// To read the value of the string, we need to allocate strlen of memory
	str := make([]byte, strlen)
	// Anything that follows string length (which is at position+32 bytes) is the value
	_, err = reader.ReadAt(str, position+32)
	if err != nil {
		return "", err
	}

	return string(str), nil
}
