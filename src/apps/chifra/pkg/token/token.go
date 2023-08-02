package token

import (
	"context"
	"errors"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
)

// Erc721SupportsInterfaceData is the data needed to call the ERC-721 supportsInterface function
// 0x01ffc9a7: supportsInterface -- eips.ethereum.org/EIPS/eip-165
// 0x80ac58cd: ERC-721 interface ID -- eips.ethereum.org/EIPS/eip-721
const Erc721SupportsInterfaceData = "0x01ffc9a780ac58cd00000000000000000000000000000000000000000000000000000000"

type TokenStateSelector = string

// TODO: If we used encoding we could use the function signature instead of the selector.

const TokenStateTotalSupply TokenStateSelector = "0x18160ddd"
const TokenStateDecimals TokenStateSelector = "0x313ce567"
const TokenStateSymbol TokenStateSelector = "0x95d89b41"
const TokenStateName TokenStateSelector = "0x06fdde03"
const TokenStateBalanceOf TokenStateSelector = "0x70a08231"

type TokenType int

const (
	TokenErc20 TokenType = iota
	TokenErc721
)

// Token type wraps information about ERC-20 token or ERC-721 NFT. Call
// Token.IsErcXXX to check the token type.
// TODO: This is a type and should be auto-generated
type Token struct {
	Address     base.Address
	Name        string
	Symbol      string
	Decimals    uint8
	TotalSupply string // uint256 in Solidity
	Type        TokenType
}

type ErrNodeConnection struct {
	err error
}

func NewErrNodeConnection(err error) (e ErrNodeConnection) {
	e.err = err
	return
}

func (e ErrNodeConnection) Error() string {
	return e.err.Error()
}

// GetState returns token state for given block. `blockNumber` can be "latest" or "" for the latest block or
// decimal number or hex number with 0x prefix.
func GetState(chain string, tokenAddress base.Address, blockNumber string) (token *Token, err error) {
	client := rpcClient.GetClient(config.GetRpcProvider(chain))
	defer client.Close()

	// Check if we can dial the node. This way we can make sure we report back the correct state
	// (err meaning that an address is not a token).
	_, err = client.BlockNumber(context.Background())
	if err != nil {
		err = NewErrNodeConnection(err)
		return
	}

	return queryToken(chain, tokenAddress, blockNumber)
}

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
						"data": TokenStateBalanceOf + holder.Pad32(),
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

func queryToken(chain string, address base.Address, blockNumber string) (token *Token, err error) {
	results, err := rpc.QueryBatch[string](
		chain,
		[]rpc.BatchPayload{
			{
				Key: "name",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   address,
							"data": TokenStateName,
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
							"to":   address,
							"data": TokenStateSymbol,
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
							"to":   address,
							"data": TokenStateDecimals,
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
							"to":   address,
							"data": TokenStateTotalSupply,
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
							"to":   address,
							"data": Erc721SupportsInterfaceData,
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

	var decimals uint8 = 0
	rawDecimals := *results["decimals"]
	parsedDecimals, parseErr := strconv.ParseUint(rawDecimals, 0, 8)
	if parseErr == nil {
		decimals = uint8(parsedDecimals)
	}

	totalSupplyRaw := base.HexToWei(*results["totalSupply"])
	var totalSupply string
	if len(totalSupplyRaw.Bits()) > 0 {
		totalSupply = totalSupplyRaw.String()
	}

	// According to ERC-20, name, symbol and decimals are optional, but such a token
	// would be of no use to us
	if name == "" && symbol == "" && decimals == 0 {
		return nil, errors.New("address is not token")
	}

	tokenType := TokenErc20
	erc721, erc721Err := articulate.ArticulateBoolean(*results["erc721"])
	if erc721Err == nil && erc721 {
		tokenType = TokenErc721
	}

	token = &Token{
		Address:     address,
		Type:        tokenType,
		Name:        name,
		Symbol:      symbol,
		Decimals:    decimals,
		TotalSupply: totalSupply,
	}

	return
}

func (t *Token) IsErc20() bool {
	return t.Type == TokenErc20
}

func (t *Token) IsErc721() bool {
	return t.Type == TokenErc721
}
