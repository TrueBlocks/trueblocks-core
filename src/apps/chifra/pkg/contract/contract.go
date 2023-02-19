package contract

import (
	"context"
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

// isContractAt checks if an account is/has been a contract at the given block. Block number can be nil,
// in which case the latest block is used.
func IsContractAt(chain string, address types.Address, block *types.SimpleNamedBlock) (contract bool, err error) {
	provider := config.GetRpcProvider(chain)
	client := rpcClient.GetClient(provider)
	defer client.Close()

	var clientBlockArg *big.Int = nil
	if block != nil && block.Name != "latest" {
		clientBlockArg = big.NewInt(0).SetUint64(block.BlockNumber)
	}
	code, err := client.CodeAt(
		context.Background(),
		common.HexToAddress(address.Hex()),
		clientBlockArg,
	)
	if err != nil {
		return
	}

	contract = len(code) > 0
	return
}

// isProxy checks if an account is/has been a proxy at the given block. Block number can be nil,
// in which case the latest block is used.
func IsProxy(chain string, address types.Address, block *types.SimpleNamedBlock) (proxy bool, err error) {
	provider := config.GetRpcProvider(chain)
	client := rpcClient.GetClient(provider)
	defer client.Close()

	var clientBlockArg *big.Int = nil
	if block != nil && block.Name != "latest" {
		clientBlockArg = big.NewInt(0).SetUint64(block.BlockNumber)
	}
	// EIP 1967: https://eips.ethereum.org/EIPS/eip-1967
	// bytes32(uint256(keccak256('eip1967.proxy.implementation')) - 1) == 0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc
	eip1956Position := common.HexToHash("0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc")
	data, err := client.StorageAt(
		context.Background(),
		common.HexToAddress(address.Hex()),
		eip1956Position,
		clientBlockArg,
	)

	// convert []byte to hex string, get the last 20 bytes, and convert to address
	proxyAddress := common.HexToAddress(fmt.Sprintf("0x%x", data[len(data)-20:]))
	// address := types.HexToAddress(addr)
    proxyString := proxyAddress.Hex()
    address = types.HexToAddress(proxyString)
   // convert proxtAddress to types.Addres
	fmt.Println("proxyAddress", proxyAddress.Hex())
	proxy, err = IsContractAt(chain, address, block)
	return
}

func GetProxyAddress(chain string, address types.Address, block *types.SimpleNamedBlock) (proxyAddress types.Address, err error) {
	provider := config.GetRpcProvider(chain)
	client := rpcClient.GetClient(provider)
	defer client.Close()

	var clientBlockArg *big.Int = nil
	if block != nil && block.Name != "latest" {
		clientBlockArg = big.NewInt(0).SetUint64(block.BlockNumber)
	}
	// EIP 1967: https://eips.ethereum.org/EIPS/eip-1967
	// bytes32(uint256(keccak256('eip1967.proxy.implementation')) - 1) == 0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc
	eip1956Position := common.HexToHash("0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc")
	data, err := client.StorageAt(
		context.Background(),
		common.HexToAddress(address.Hex()),
		eip1956Position,
		clientBlockArg,
	)

	// convert []byte to hex string, get the last 20 bytes, and convert to address
    tmpAddress := common.HexToAddress(fmt.Sprintf("0x%x", data[len(data)-20:]))
	// address := types.HexToAddress(addr)
    proxyString := tmpAddress.Hex()
    proxyAddress = types.HexToAddress(proxyString)
	return
}
