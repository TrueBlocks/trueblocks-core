package token

import (
	"encoding/json"
	"fmt"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/common"
)

type TokenStateSelector = string

const TokenStateTotalSupply TokenStateSelector = "0x18160ddd"
const TokenStateDecimals TokenStateSelector = "0x313ce567"
const TokenStateSymbol TokenStateSelector = "0x95d89b41"
const TokenStateName TokenStateSelector = "0x06fdde03"
const TokenStateSupportsInterface TokenStateSelector = "0x01ffc9a7"

func GetState(chain string, token base.Address, selector TokenStateSelector, abiSpec *abi.ABI, blockNum base.Blknum, bytes string) (string, error) {
	params, err := json.Marshal(map[string]any{
		"to":   token.Address,
		"data": fmt.Sprintf("%s%s", selector, strings.Replace(bytes, "0x", "", 1)),
	})
	if err != nil {
		return "", err
	}
	payload := rpc.Payload{
		Method: "eth_call",
		Params: rpc.Params{params, blockNum},
	}

	var response struct {
		Result string
	}
	err = rpc.FromRpc(config.GetRpcProvider(chain), &payload, &response)
	if err != nil {
		return "", nil
	}

	method, err := abiSpec.MethodById(common.Hex2Bytes(selector))
	if err != nil {
		return "", nil
	}
	values := make([]types.SimpleParameter, 0)
	articulate.ArticulateArguments(method.Outputs, response.Result, []base.Hash{}, values)
	return fmt.Sprint(values[0].Value), nil

}
