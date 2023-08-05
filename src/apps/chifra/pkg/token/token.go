package token

import (
	"errors"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decode"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

// erc721SupportsInterfaceData is the data needed to call the ERC-721 supportsInterface function
// 0x01ffc9a7: supportsInterface -- eips.ethereum.org/EIPS/eip-165
// 0x80ac58cd: ERC-721 interface ID -- eips.ethereum.org/EIPS/eip-721
const erc721SupportsInterfaceData = "0x01ffc9a780ac58cd00000000000000000000000000000000000000000000000000000000"

type tokenStateSelector = string

// TODO: If we used encoding we could use the function signature instead of the selector.

const tokenStateTotalSupply tokenStateSelector = "0x18160ddd"
const tokenStateDecimals tokenStateSelector = "0x313ce567"
const tokenStateSymbol tokenStateSelector = "0x95d89b41"
const tokenStateName tokenStateSelector = "0x06fdde03"
const tokenStateBalanceOf tokenStateSelector = "0x70a08231"

// GetTokenState returns token state for given block. `blockNumber` can be "latest" or "" for the latest block or
// decimal number or hex number with 0x prefix.
func GetTokenState(chain string, tokenAddress base.Address, blockNumber string) (token *types.SimpleToken, err error) {
	results, err := rpc.QueryBatch[string](
		chain,
		[]rpc.BatchPayload{
			{
				Key: "name",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddress,
							"data": tokenStateName,
						},
						blockNumber,
					},
				},
			},
			{
				Key: "symbol",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddress,
							"data": tokenStateSymbol,
						},
						blockNumber,
					},
				},
			},
			{
				Key: "decimals",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddress,
							"data": tokenStateDecimals,
						},
						blockNumber,
					},
				},
			},
			{
				Key: "totalSupply",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddress,
							"data": tokenStateTotalSupply,
						},
						blockNumber,
					},
				},
			},
			// Supports interface: ERC 721
			{
				Key: "erc721",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddress,
							"data": erc721SupportsInterfaceData,
						},
						blockNumber,
					},
				},
			},
		},
	)
	if err != nil {
		return
	}

	name, _ := articulate.ArticulateEncodedStringOrBytes32(*results["name"])
	symbol, _ := articulate.ArticulateEncodedStringOrBytes32(*results["symbol"])

	var decimals uint64 = 0
	rawDecimals := *results["decimals"]
	parsedDecimals, parseErr := strconv.ParseUint(rawDecimals, 0, 8)
	if parseErr == nil {
		decimals = uint64(parsedDecimals)
	}

	totalSupply := base.HexToWei(*results["totalSupply"])

	// According to ERC-20, name, symbol and decimals are optional, but such a token
	// would be of no use to us
	if name == "" && symbol == "" && decimals == 0 {
		return nil, errors.New("address is not token")
	}

	tokenType := types.TokenErc20
	erc721, erc721Err := decode.DecodeBool(*results["erc721"])
	if erc721Err == nil && erc721 {
		tokenType = types.TokenErc721
	}

	token = &types.SimpleToken{
		Address:     tokenAddress,
		Decimals:    decimals,
		Name:        name,
		Symbol:      symbol,
		TotalSupply: *totalSupply,
		Type:        tokenType,
	}

	return
}

// GetTokenBalanceAt returns token balance for given block. `blockNumber` can be "latest" or "" for the latest block or
// decimal number or hex number with 0x prefix.
func GetTokenBalanceAt(chain string, token, holder base.Address, blockNumber string) (balance *big.Int, err error) {
	output, err := rpc.QueryBatch[string](
		chain,
		[]rpc.BatchPayload{{
			Key: "balance",
			Payload: &rpc.Payload{
				Method: "eth_call",
				Params: rpc.Params{
					map[string]any{
						"to":   token.Hex(),
						"data": tokenStateBalanceOf + holder.Pad32(),
					},
					blockNumber,
				},
			},
		}},
	)

	if err != nil {
		return nil, err
	}

	if output["balance"] == nil {
		return big.NewInt(0), nil
	}

	return base.HexToWei(*output["balance"]), nil
}
