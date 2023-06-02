package rpcClient

import (
	"context"
	"errors"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

var ErrNotAContract = errors.New("not a contract")

// IsContractAt checks if an account is a contract
func IsContractAt(chain string, address base.Address, block *types.SimpleNamedBlock) error {
	provider := config.GetRpcProvider(chain)
	client := GetClient(provider)
	defer client.Close()

	var clientBlockArg *big.Int = nil
	if block != nil && block.Name != "latest" {
		clientBlockArg = big.NewInt(0).SetUint64(block.BlockNumber)
	}

	ctx := context.Background()
	if code, err := client.CodeAt(ctx, common.HexToAddress(address.Hex()), clientBlockArg); err != nil {
		return err
	} else {
		if len(code) == 0 {
			return ErrNotAContract
		}
	}

	return nil
}
