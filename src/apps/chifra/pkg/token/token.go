package token

import (
	"fmt"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/config"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/erc20"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/erc721"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/rpcClient"
	"github.com/ethereum/go-ethereum/accounts/abi/bind"
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

// GetState returns token state for given block. `blockNumber` can be "latest" or "" for the latest block or
// decimal number or hex number with 0x prefix.
func GetState(chain string, tokenAddress base.Address, blockNumber string) (token *Token, err error) {
	client := rpcClient.GetClient(config.GetRpcProvider(chain))
	defer client.Close()

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
	return getStateErc20(tokenAddress, client, callOpts)
}

func getStateErc20(address base.Address, client *ethclient.Client, callOpts *bind.CallOpts) (token *Token, err error) {
	contractErc20, err := erc20.NewErc20Caller(address.Address, client)

	if err != nil {
		return
	}

	name, err := contractErc20.Name(callOpts)
	if err != nil {
		return
	}

	symbol, err := contractErc20.Symbol(callOpts)
	if err != nil {
		return
	}

	decimals, err := contractErc20.Decimals(callOpts)
	if err != nil {
		return
	}

	totalSupply, err := contractErc20.TotalSupply(callOpts)
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

func getStateErc721(address base.Address, client *ethclient.Client, callOpts *bind.CallOpts) (token *Token, err error) {
	contractErc721, err := erc721.NewErc721Caller(address.Address, client)
	if err != nil {
		return
	}

	name, err := contractErc721.Name(callOpts)
	if err != nil {
		return
	}

	symbol, err := contractErc721.Symbol(callOpts)
	if err != nil {
		return
	}

	totalSupply, err := contractErc721.TotalSupply(callOpts)
	if err != nil {
		return
	}

	token = &Token{
		Type:        TokenErc721,
		Name:        name,
		Symbol:      symbol,
		TotalSupply: totalSupply.String(),
	}
	return
}

func (t *Token) IsErc20() bool {
	return t.Type == TokenErc20
}

func (t *Token) IsErc721() bool {
	return t.Type == TokenErc721
}
