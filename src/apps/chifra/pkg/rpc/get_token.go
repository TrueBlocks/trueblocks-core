package rpc

import (
	"errors"
	"fmt"
	"strconv"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/decode"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc/query"
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

// GetTokenState returns token state for given block. `hexBlockNo` can be "latest" or "" for the latest
// block or decimal number or hex number with 0x prefix. (search: FromRpc)
func (conn *Connection) GetTokenState(tokenAddress base.Address, hexBlockNo string) (token *types.Token, err error) {
	if hexBlockNo != "" && hexBlockNo != "latest" && !strings.HasPrefix(hexBlockNo, "0x") {
		hexBlockNo = fmt.Sprintf("0x%x", base.MustParseUint64(hexBlockNo))
	}
	payloads := []query.BatchPayload{
		{
			Key: "name",
			Payload: &query.Payload{
				Method: "eth_call",
				Params: query.Params{
					map[string]any{
						"to":   tokenAddress,
						"data": tokenStateName,
					},
					hexBlockNo,
				},
			},
		},
		{
			Key: "symbol",
			Payload: &query.Payload{
				Method: "eth_call",
				Params: query.Params{
					map[string]any{
						"to":   tokenAddress,
						"data": tokenStateSymbol,
					},
					hexBlockNo,
				},
			},
		},
		{
			Key: "decimals",
			Payload: &query.Payload{
				Method: "eth_call",
				Params: query.Params{
					map[string]any{
						"to":   tokenAddress,
						"data": tokenStateDecimals,
					},
					hexBlockNo,
				},
			},
		},
		{
			Key: "totalSupply",
			Payload: &query.Payload{
				Method: "eth_call",
				Params: query.Params{
					map[string]any{
						"to":   tokenAddress,
						"data": tokenStateTotalSupply,
					},
					hexBlockNo,
				},
			},
		},
		// Supports interface: ERC 721
		{
			Key: "erc721",
			Payload: &query.Payload{
				Method: "eth_call",
				Params: query.Params{
					map[string]any{
						"to":   tokenAddress,
						"data": erc721SupportsInterfaceData,
					},
					hexBlockNo,
				},
			},
		},
	}

	results, err := query.QueryBatch[string](conn.Chain, payloads)
	if err != nil {
		return
	}

	name, _ := decode.ArticulateStringOrBytes(*results["name"])
	symbol, _ := decode.ArticulateStringOrBytes(*results["symbol"])

	var decimals uint64 = 0
	strDecimals := *results["decimals"]
	parsedDecimals, parseErr := strconv.ParseUint(strDecimals, 0, 8)
	if parseErr == nil {
		decimals = uint64(parsedDecimals)
	}

	totalSupply := base.HexToWei(*results["totalSupply"])

	// TODO: Maybe reconcsider this
	// TODO: According to ERC-20, name, symbol and decimals are optional, but such a token
	// TODO: would be of no use to us
	if name == "" && symbol == "" && decimals == 0 {
		return nil, errors.New(tokenAddress.Hex() + " address is not token")
	}

	tokenType := types.TokenErc20
	erc721, erc721Err := decode.ArticulateBool(*results["erc721"])
	if erc721Err == nil && erc721 {
		tokenType = types.TokenErc721
	}

	token = &types.Token{
		Address:     tokenAddress,
		Decimals:    decimals,
		Name:        name,
		Symbol:      symbol,
		TotalSupply: *totalSupply,
		TokenType:   tokenType,
	}

	return
}

// GetBalanceAtToken returns token balance for given block. `hexBlockNo` can be "latest" or "" for the latest block or
// decimal number or hex number with 0x prefix.
func (conn *Connection) GetBalanceAtToken(token, holder base.Address, hexBlockNo string) (*base.Wei, error) {
	if hexBlockNo != "" && hexBlockNo != "latest" && !strings.HasPrefix(hexBlockNo, "0x") {
		hexBlockNo = fmt.Sprintf("0x%x", base.MustParseUint64(hexBlockNo))
	}

	payloads := []query.BatchPayload{{
		Key: "balance",
		Payload: &query.Payload{
			Method: "eth_call",
			Params: query.Params{
				map[string]any{
					"to":   token.Hex(),
					"data": tokenStateBalanceOf + holder.Pad32(),
				},
				hexBlockNo,
			},
		},
	}}

	output, err := query.QueryBatch[string](conn.Chain, payloads)
	if err != nil {
		return nil, err
	}

	if output["balance"] == nil {
		return base.NewWei(0), nil
	}

	return base.HexToWei(*output["balance"]), nil
}
