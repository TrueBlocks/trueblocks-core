package token

import (
	"fmt"
	"strconv"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/abi"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/articulate"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpc"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

type TokenStateSelector = string

const TokenStateTotalSupply TokenStateSelector = "0x18160ddd"
const TokenStateDecimals TokenStateSelector = "0x313ce567"
const TokenStateSymbol TokenStateSelector = "0x95d89b41"
const TokenStateName TokenStateSelector = "0x06fdde03"
const TokenStateSupportsInterface TokenStateSelector = "0x01ffc9a7"

// TODO: this function could:
// 1a. Take bitflags instead of selector
// 1b. Not take any selector and just multicall all getters (should be cheap)
// 2a. Return a map with values set (no zero value problem)
// 2b. Return a struct, if we fetch everything (no zero value problem) -- add isErc20 (false for non-tokens) and isErc721
// func GetState(chain string, token base.Address, selector TokenStateSelector, abis abi.AbiInterfaceMap, blockNum string) (string, error) {
// 	result, err := rpc.Query[string](
// 		"mainnet",
// 		"eth_call",
// 		rpc.Params{
// 			map[string]any{
// 				"to":   token.Address,
// 				"data": selector,
// 			},
// 			blockNum,
// 		},
// 	)
// 	if err != nil {
// 		return "", nil
// 	}

// 	abi, ok := abis[selector]
// 	if !ok {
// 		return "", fmt.Errorf("abi not found for method %s", selector)
// 	}

// 	method, err := abi.GetAbiMethod()
// 	if err != nil {
// 		return "", err
// 	}
// 	results := make([]types.SimpleParameter, len(method.Outputs))
// 	err = articulate.ArticulateArguments(method.Outputs, strings.Replace(*result, "0x", "", 1), []base.Hash{}, results)
// 	if err != nil {
// 		return "", err
// 	}
// 	if len(results) == 0 {
// 		return "", nil
// 	}
// 	return fmt.Sprint(results[0].Value), nil

// }

const Erc721InterfaceId = "0x80ac58cd"

type Token struct {
	Name        string
	Symbol      string
	Decimals    uint64 // uint8 in Solidity
	TotalSupply string // uint256 in Solidity
	Erc20       bool
	Erc721      bool
}

func GetState(chain string, tokenAddr base.Address, abis abi.AbiInterfaceMap, blockNum string) (token *Token, err error) {
	results, err := rpc.BatchQuery[string](
		"mainnet",
		[]rpc.BatchPayload{
			{
				Key: "name",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddr,
							"data": TokenStateName,
						},
						blockNum,
					},
				},
			},
			{
				Key: "symbol",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddr,
							"data": TokenStateSymbol,
						},
						blockNum,
					},
				},
			},
			{
				Key: "decimals",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddr,
							"data": TokenStateDecimals,
						},
						blockNum,
					},
				},
			},
			{
				Key: "totalSupply",
				Payload: &rpc.Payload{
					Method: "eth_call",
					Params: rpc.Params{
						map[string]any{
							"to":   tokenAddr,
							"data": TokenStateTotalSupply,
						},
						blockNum,
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
							"to":   tokenAddr,
							"data": fmt.Sprintf("%s%s", TokenStateSupportsInterface, common.HexToHash(Erc721InterfaceId)),
						},
						blockNum,
					},
				},
			},
		},
	)
	if err != nil {
		return
	}

	name, err := articulateTokenStateGetter(abis, TokenStateName, *results["name"])
	if err != nil {
		return
	}
	symbol, err := articulateTokenStateGetter(abis, TokenStateSymbol, *results["symbol"])
	if err != nil {
		return
	}
	decimalsRaw, err := articulateTokenStateGetter(abis, TokenStateDecimals, *results["decimals"])
	if err != nil {
		return
	}
	decimals, err := strconv.ParseUint(fmt.Sprint(decimalsRaw), 16, 8)
	if err != nil {
		// err = errors.New("cannot cast decimals to uint8")
		return
	}
	totalSupply, err := articulateTokenStateGetter(abis, TokenStateTotalSupply, *results["totalSupply"])
	if err != nil {
		return
	}

	token = &Token{
		Name:        fmt.Sprint(name),
		Symbol:      fmt.Sprint(symbol),
		Decimals:    decimals,
		TotalSupply: fmt.Sprint(totalSupply),
		Erc721:      *results["erc721"] == "T" || *results["erc721"] == "true",
	}

	token.Erc20 = len(token.Name) > 0 || len(token.Symbol) > 0 || token.Decimals > 0
	return
}

func articulateTokenStateGetter(abis abi.AbiInterfaceMap, selector string, value string) (any, error) {
	abi, ok := abis[selector]
	if !ok {
		return "", fmt.Errorf("abi not found for method %s", selector)
	}

	method, err := abi.GetAbiMethod()
	if err != nil {
		return "", err
	}
	results := make([]types.SimpleParameter, len(method.Outputs))
	err = articulate.ArticulateArguments(method.Outputs, value[2:], []base.Hash{}, results)
	if err != nil {
		return "", err
	}
	if len(results) == 0 {
		return "", nil
	}

	return results[0].Value, nil
}
