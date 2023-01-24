package account

import (
	"context"
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
