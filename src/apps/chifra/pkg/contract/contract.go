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
func IsProxy(chain string, address types.Address, block *types.SimpleNamedBlock) (implementation types.Address, err error) {
	potentialAddresses, err := getKnownImplementationSlots(chain, address, block)
	if err != nil {
		return
	}

	// check if any of the potential addresses are contracts
	// if any are contracts, address is a proxy and potentialAddress is the implementation
	for _, potentialAddress := range potentialAddresses {
		isContract, err := IsContractAt(chain, potentialAddress, block)
		if err != nil {
			break
		}

		if isContract {
			implementation = potentialAddress
			break
		}
	}

	return
}

// Returns an array of potential implementation addresses for an addresss
func getKnownImplementationSlots(chain string, address types.Address, block *types.SimpleNamedBlock) (addresses []types.Address, err error) {
	provider := config.GetRpcProvider(chain)
	client := rpcClient.GetClient(provider)
	defer client.Close()

	var clientBlockArg *big.Int = nil
	if block != nil && block.Name != "latest" {
		clientBlockArg = big.NewInt(0).SetUint64(block.BlockNumber)
	}
	// EIP 1967: https://eips.ethereum.org/EIPS/eip-1967
	// bytes32(uint256(keccak256('eip1967.proxy.implementation')) - 1) == 0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc
	eip1967Slot := common.HexToHash("0x360894a13ba1a3210667c828492db98dca3e2076cc3735a920a3ca505d382bbc")
	eip1967ZOSSlot := common.HexToHash("0x7050c9e0f4ca769c69bd3a8ef740bc37934f8e2c036e5a723fd8ee048ed3f8c3")
	eip1822Slot := common.HexToHash("0xc5f16f0fcc639fa48a6947836d9850f504798523bf8c9a3a87d5876cf622bcf7")
	eip1822ZOSSlot := common.HexToHash("0x5f3b5dfeb7b28cdbd7faba78963ee202a494e2a2cc8c9978d5e30d2aebb8c197")
	// This is the slot used by the OpenZeppelin GnosisSafeProxy
	zeroSlot := common.HexToHash("0x0000000000000000000000000000000000000000000000000000000000000000")
	slots := []common.Hash{eip1967Slot, eip1967ZOSSlot, eip1822Slot, eip1822ZOSSlot, zeroSlot}

	for _, slot := range slots {
		slotStorage, err := client.StorageAt(
			context.Background(),
			common.HexToAddress(address.Hex()),
			slot,
			clientBlockArg,
		)
		// In theory all storage slots are 32 bytes, but we should check in case
		if err != nil || len(slotStorage) < 20 {
			break
		}
		// convert []byte to hex string, get the last 20 bytes, and convert to address
		potentialAddress := types.HexToAddress(fmt.Sprintf("0x%x", slotStorage[len(slotStorage)-20:]))
		addresses = append(addresses, potentialAddress)
	}

	if err != nil {
		addresses = []types.Address{}
		return
	}

	return
}
