package token

import (
	"context"
	"errors"
	"fmt"
	"math/big"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/dsToken"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/erc20"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/erc721"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/ethereum/go-ethereum/accounts/abi/bind"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/ethclient"
)

var Erc721InterfaceId = [4]byte{
	0x80,
	0xac,
	0x58,
	0xcd,
}

type TokenType int

const (
	TokenErc20 TokenType = iota
	TokenErc721
)

// Token type wraps information about ERC-20 token or ERC-721 NFT. Call
// Token.IsErcXXX to check the token type.
type Token struct {
	Name        string
	Symbol      string
	Decimals    uint8
	TotalSupply string // uint256 in Solidity
	Type        TokenType
}

// var NodeConnectionError = errors.New("connection error")
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

	callOpts := &bind.CallOpts{}
	if blockNumber != "" && blockNumber != "latest" {
		// Convert block number to *big.Int
		blknum := big.NewInt(0)
		_, set := blknum.SetString(blockNumber, 0)
		if !set {
			err = fmt.Errorf("could not set block number to: %s", blockNumber)
			return
		}
		callOpts.BlockNumber = blknum
	}

	// First, check if token is ERC-721 (NFT)
	contractErc721, err := erc721.NewErc721Caller(tokenAddress.Address, client)
	if err != nil {
		return
	}

	// We have to ignore the error at this point, because it can mean that the contract is not ERC-721.
	// Unfortunatelly, we have no way to make sure. Erigon returns code -32000, but this code is non-standard
	erc721Supported, _ := contractErc721.SupportsInterface(callOpts, Erc721InterfaceId)
	if erc721Supported {
		return getStateErc721(tokenAddress, client, callOpts)
	}

	// If it is not, assume it's ERC-20
	token, err = getStateErc20(tokenAddress, client, callOpts)
	if err == nil {
		return
	}

	// It can also be DSToken
	token, err = getStateDsToken(tokenAddress, client, callOpts)
	if err == nil {
		return
	}

	// Non-standard
	return getStateNonStandard(tokenAddress, client, blockNumber)
}

func getStateErc20(address base.Address, client *ethclient.Client, callOpts *bind.CallOpts) (token *Token, err error) {
	contractErc20, err := erc20.NewErc20Caller(address.Address, client)

	if err != nil {
		return
	}

	partial := &Token{
		Type: TokenErc20,
	}

	name, err := contractErc20.Name(callOpts)
	if err != nil {
		return
	}
	partial.Name = name

	// Some tokens (0x014b50466590340d41307cc54dcee990c8d58aa8) don't implement Symbol, but they
	// do implement Decimals. We will ignore the error here, if there's no Decimals, then the code
	// below won't return success.
	symbol, _ := contractErc20.Symbol(callOpts)
	partial.Symbol = symbol

	decimals, err := contractErc20.Decimals(callOpts)
	if err != nil {
		if name != "" && symbol != "" {
			// Some tokens don't implement decimals (e.g. 0xefabe332d31c3982b76f8630a306c960169bd5b3)
			return partial, nil
		}
		return
	}
	partial.Decimals = decimals

	totalSupply, err := contractErc20.TotalSupply(callOpts)
	// Some tokens (0x5624eed6e0ebef05eb9edd4c24d5abc08e181e71) don't implement totalSupply.
	// If we reached this far, we have name and either symbol or decimals, so we can return
	if err != nil {
		return partial, nil
	}
	partial.TotalSupply = totalSupply.String()

	token = partial
	return
}

func getStateErc721(address base.Address, client *ethclient.Client, callOpts *bind.CallOpts) (token *Token, err error) {
	contractErc721, err := erc721.NewErc721Caller(address.Address, client)
	if err != nil {
		return
	}

	partial := &Token{
		Type: TokenErc721,
	}

	name, err := contractErc721.Name(callOpts)
	if err != nil {
		return
	}
	partial.Name = name

	symbol, err := contractErc721.Symbol(callOpts)
	if err != nil {
		return
	}
	partial.Symbol = symbol

	totalSupply, err := contractErc721.TotalSupply(callOpts)
	if err != nil {
		// Some NFTs don't implement totalSupply
		if name != "" && symbol != "" {
			return partial, nil
		}
		return
	}
	partial.TotalSupply = totalSupply.String()

	token = partial
	return
}

func getStateDsToken(address base.Address, client *ethclient.Client, callOpts *bind.CallOpts) (token *Token, err error) {
	contractDsToken, err := dsToken.NewDsTokenCaller(address.Address, client)

	if err != nil {
		return
	}

	name, err := contractDsToken.Name(callOpts)
	if err != nil {
		return
	}

	symbol, err := contractDsToken.Symbol(callOpts)
	if err != nil {
		return
	}

	decimals, err := contractDsToken.Decimals(callOpts)
	if err != nil {
		return
	}

	totalSupply, err := contractDsToken.TotalSupply(callOpts)
	if err != nil {
		return
	}

	token = &Token{
		Type:        TokenErc20,
		Name:        name,
		Symbol:      symbol,
		Decimals:    decimals,
		TotalSupply: totalSupply.String(),
	}

	return
}

type TokenStateSelector = string

const TokenStateTotalSupply TokenStateSelector = "0x18160ddd"
const TokenStateDecimals TokenStateSelector = "0x313ce567"
const TokenStateSymbol TokenStateSelector = "0x95d89b41"
const TokenStateName TokenStateSelector = "0x06fdde03"
const TokenStateSupportsInterface TokenStateSelector = "0x01ffc9a7"

// TODO: this could read all tokens, if name and stuff can be bytes32 OR strings
func getStateNonStandard(address base.Address, client *ethclient.Client, blockNumber string) (token *Token, err error) {
	results, err := rpc.BatchQuery[string](
		"mainnet",
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
							"data": fmt.Sprintf("%s%s", TokenStateSupportsInterface, common.HexToHash("0x80ac58cd")),
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

	name := articulate.HexToName(*results["name"], false)
	symbol := articulate.HexToName(*results["symbol"], false)

	rawDecimals, _ := strconv.ParseUint(*results["decimals"], 0, 8)
	decimals := uint8(rawDecimals)

	totalSupplyRaw := big.NewInt(0)
	totalSupplyRaw.SetString(*results["totalSupply"], 0)
	totalSupply := totalSupplyRaw.String()

	// Some tokens don't implement name (0xce5c603c78d047ef43032e96b5b785324f753a4f)
	if name == "" && symbol == "" {
		return nil, errors.New("cannot read token state")
	}

	tokenType := TokenErc20
	if *results["erc721"] == "T" || *results["erc721"] == "true" {
		tokenType = TokenErc721
	}

	token = &Token{
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
