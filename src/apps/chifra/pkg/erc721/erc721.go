// Code generated - DO NOT EDIT.
// This file is a generated binding and any manual changes will be lost.

package erc721

import (
	"math/big"
	"strings"

	ethereum "github.com/ethereum/go-ethereum"
	"github.com/ethereum/go-ethereum/accounts/abi"
	"github.com/ethereum/go-ethereum/accounts/abi/bind"
	"github.com/ethereum/go-ethereum/common"
	"github.com/ethereum/go-ethereum/core/types"
	"github.com/ethereum/go-ethereum/event"
)

// Reference imports to suppress errors if they are not otherwise used.
var (
	_ = big.NewInt
	_ = strings.NewReader
	_ = ethereum.NotFound
	_ = bind.Bind
	_ = common.Big1
	_ = types.BloomLookup
	_ = event.NewSubscription
)

// Erc721ABI is the input ABI used to generate the binding from.
const Erc721ABI = "[{\"encoding\":\"0x095ea7b3\",\"inputs\":[{\"name\":\"_approved\",\"type\":\"address\"},{\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"approve\",\"signature\":\"approve(address,uint256)\",\"stateMutability\":\"payable\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x70a08231\",\"inputs\":[{\"name\":\"_owner\",\"type\":\"address\"}],\"name\":\"balanceOf\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"uint256\"}],\"signature\":\"balanceOf(address)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x081812fc\",\"inputs\":[{\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"getApproved\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"address\"}],\"signature\":\"getApproved(uint256)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0xe985e9c5\",\"inputs\":[{\"name\":\"_owner\",\"type\":\"address\"},{\"name\":\"_operator\",\"type\":\"address\"}],\"name\":\"isApprovedForAll\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"bool\"}],\"signature\":\"isApprovedForAll(address,address)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x06fdde03\",\"name\":\"name\",\"outputs\":[{\"name\":\"_name\",\"type\":\"string\"}],\"signature\":\"name()\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"encoding\":\"0x150b7a02\",\"inputs\":[{\"name\":\"_operator\",\"type\":\"address\"},{\"name\":\"_from\",\"type\":\"address\"},{\"name\":\"_tokenId\",\"type\":\"uint256\"},{\"name\":\"_data\",\"type\":\"bytes\"}],\"name\":\"onERC721Received\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"bytes4\"}],\"signature\":\"onERC721Received(address,address,uint256,bytes)\",\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x6352211e\",\"inputs\":[{\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"ownerOf\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"address\"}],\"signature\":\"ownerOf(uint256)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"encoding\":\"0x42842e0e\",\"inputs\":[{\"name\":\"_from\",\"type\":\"address\"},{\"name\":\"_to\",\"type\":\"address\"},{\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"safeTransferFrom\",\"signature\":\"safeTransferFrom(address,address,uint256)\",\"stateMutability\":\"payable\",\"type\":\"function\"},{\"encoding\":\"0xb88d4fde\",\"inputs\":[{\"name\":\"_from\",\"type\":\"address\"},{\"name\":\"_to\",\"type\":\"address\"},{\"name\":\"_tokenId\",\"type\":\"uint256\"},{\"name\":\"data\",\"type\":\"bytes\"}],\"name\":\"safeTransferFrom\",\"signature\":\"safeTransferFrom(address,address,uint256,bytes)\",\"stateMutability\":\"payable\",\"type\":\"function\"},{\"encoding\":\"0xa22cb465\",\"inputs\":[{\"name\":\"_operator\",\"type\":\"address\"},{\"name\":\"_approved\",\"type\":\"bool\"}],\"name\":\"setApprovalForAll\",\"signature\":\"setApprovalForAll(address,bool)\",\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x01ffc9a7\",\"inputs\":[{\"name\":\"interfaceID\",\"type\":\"bytes4\"}],\"name\":\"supportsInterface\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"bool\"}],\"signature\":\"supportsInterface(bytes4)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x95d89b41\",\"name\":\"symbol\",\"outputs\":[{\"name\":\"_symbol\",\"type\":\"string\"}],\"signature\":\"symbol()\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x4f6ccce7\",\"inputs\":[{\"name\":\"_index\",\"type\":\"uint256\"}],\"name\":\"tokenByIndex\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"uint256\"}],\"signature\":\"tokenByIndex(uint256)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x2f745c59\",\"inputs\":[{\"name\":\"_owner\",\"type\":\"address\"},{\"name\":\"_index\",\"type\":\"uint256\"}],\"name\":\"tokenOfOwnerByIndex\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"uint256\"}],\"signature\":\"tokenOfOwnerByIndex(address,uint256)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0xc87b56dd\",\"inputs\":[{\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"tokenURI\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"string\"}],\"signature\":\"tokenURI(uint256)\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"constant\":true,\"encoding\":\"0x18160ddd\",\"name\":\"totalSupply\",\"outputs\":[{\"name\":\"ret_0\",\"type\":\"uint256\"}],\"signature\":\"totalSupply()\",\"stateMutability\":\"view\",\"type\":\"function\"},{\"encoding\":\"0x23b872dd\",\"inputs\":[{\"name\":\"_from\",\"type\":\"address\"},{\"name\":\"_to\",\"type\":\"address\"},{\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"transferFrom\",\"signature\":\"transferFrom(address,address,uint256)\",\"stateMutability\":\"payable\",\"type\":\"function\"},{\"encoding\":\"0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925\",\"inputs\":[{\"indexed\":true,\"name\":\"_owner\",\"type\":\"address\"},{\"indexed\":true,\"name\":\"_approved\",\"type\":\"address\"},{\"indexed\":true,\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"Approval\",\"signature\":\"Approval(address,address,uint256)\",\"stateMutability\":\"nonpayable\",\"type\":\"event\"},{\"encoding\":\"0x17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c31\",\"inputs\":[{\"indexed\":true,\"name\":\"_owner\",\"type\":\"address\"},{\"indexed\":true,\"name\":\"_operator\",\"type\":\"address\"},{\"name\":\"_approved\",\"type\":\"bool\"}],\"name\":\"ApprovalForAll\",\"signature\":\"ApprovalForAll(address,address,bool)\",\"stateMutability\":\"nonpayable\",\"type\":\"event\"},{\"encoding\":\"0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef\",\"inputs\":[{\"indexed\":true,\"name\":\"_from\",\"type\":\"address\"},{\"indexed\":true,\"name\":\"_to\",\"type\":\"address\"},{\"indexed\":true,\"name\":\"_tokenId\",\"type\":\"uint256\"}],\"name\":\"Transfer\",\"signature\":\"Transfer(address,address,uint256)\",\"stateMutability\":\"nonpayable\",\"type\":\"event\"}]"

// Erc721 is an auto generated Go binding around an Ethereum contract.
type Erc721 struct {
	Erc721Caller     // Read-only binding to the contract
	Erc721Transactor // Write-only binding to the contract
	Erc721Filterer   // Log filterer for contract events
}

// Erc721Caller is an auto generated read-only Go binding around an Ethereum contract.
type Erc721Caller struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// Erc721Transactor is an auto generated write-only Go binding around an Ethereum contract.
type Erc721Transactor struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// Erc721Filterer is an auto generated log filtering Go binding around an Ethereum contract events.
type Erc721Filterer struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// Erc721Session is an auto generated Go binding around an Ethereum contract,
// with pre-set call and transact options.
type Erc721Session struct {
	Contract     *Erc721           // Generic contract binding to set the session for
	CallOpts     bind.CallOpts     // Call options to use throughout this session
	TransactOpts bind.TransactOpts // Transaction auth options to use throughout this session
}

// Erc721CallerSession is an auto generated read-only Go binding around an Ethereum contract,
// with pre-set call options.
type Erc721CallerSession struct {
	Contract *Erc721Caller // Generic contract caller binding to set the session for
	CallOpts bind.CallOpts // Call options to use throughout this session
}

// Erc721TransactorSession is an auto generated write-only Go binding around an Ethereum contract,
// with pre-set transact options.
type Erc721TransactorSession struct {
	Contract     *Erc721Transactor // Generic contract transactor binding to set the session for
	TransactOpts bind.TransactOpts // Transaction auth options to use throughout this session
}

// Erc721Raw is an auto generated low-level Go binding around an Ethereum contract.
type Erc721Raw struct {
	Contract *Erc721 // Generic contract binding to access the raw methods on
}

// Erc721CallerRaw is an auto generated low-level read-only Go binding around an Ethereum contract.
type Erc721CallerRaw struct {
	Contract *Erc721Caller // Generic read-only contract binding to access the raw methods on
}

// Erc721TransactorRaw is an auto generated low-level write-only Go binding around an Ethereum contract.
type Erc721TransactorRaw struct {
	Contract *Erc721Transactor // Generic write-only contract binding to access the raw methods on
}

// NewErc721 creates a new instance of Erc721, bound to a specific deployed contract.
func NewErc721(address common.Address, backend bind.ContractBackend) (*Erc721, error) {
	contract, err := bindErc721(address, backend, backend, backend)
	if err != nil {
		return nil, err
	}
	return &Erc721{Erc721Caller: Erc721Caller{contract: contract}, Erc721Transactor: Erc721Transactor{contract: contract}, Erc721Filterer: Erc721Filterer{contract: contract}}, nil
}

// NewErc721Caller creates a new read-only instance of Erc721, bound to a specific deployed contract.
func NewErc721Caller(address common.Address, caller bind.ContractCaller) (*Erc721Caller, error) {
	contract, err := bindErc721(address, caller, nil, nil)
	if err != nil {
		return nil, err
	}
	return &Erc721Caller{contract: contract}, nil
}

// NewErc721Transactor creates a new write-only instance of Erc721, bound to a specific deployed contract.
func NewErc721Transactor(address common.Address, transactor bind.ContractTransactor) (*Erc721Transactor, error) {
	contract, err := bindErc721(address, nil, transactor, nil)
	if err != nil {
		return nil, err
	}
	return &Erc721Transactor{contract: contract}, nil
}

// NewErc721Filterer creates a new log filterer instance of Erc721, bound to a specific deployed contract.
func NewErc721Filterer(address common.Address, filterer bind.ContractFilterer) (*Erc721Filterer, error) {
	contract, err := bindErc721(address, nil, nil, filterer)
	if err != nil {
		return nil, err
	}
	return &Erc721Filterer{contract: contract}, nil
}

// bindErc721 binds a generic wrapper to an already deployed contract.
func bindErc721(address common.Address, caller bind.ContractCaller, transactor bind.ContractTransactor, filterer bind.ContractFilterer) (*bind.BoundContract, error) {
	parsed, err := abi.JSON(strings.NewReader(Erc721ABI))
	if err != nil {
		return nil, err
	}
	return bind.NewBoundContract(address, parsed, caller, transactor, filterer), nil
}

// Call invokes the (constant) contract method with params as input values and
// sets the output to result. The result type might be a single field for simple
// returns, a slice of interfaces for anonymous returns and a struct for named
// returns.
func (_Erc721 *Erc721Raw) Call(opts *bind.CallOpts, result *[]interface{}, method string, params ...interface{}) error {
	return _Erc721.Contract.Erc721Caller.contract.Call(opts, result, method, params...)
}

// Transfer initiates a plain transaction to move funds to the contract, calling
// its default method if one is available.
func (_Erc721 *Erc721Raw) Transfer(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _Erc721.Contract.Erc721Transactor.contract.Transfer(opts)
}

// Transact invokes the (paid) contract method with params as input values.
func (_Erc721 *Erc721Raw) Transact(opts *bind.TransactOpts, method string, params ...interface{}) (*types.Transaction, error) {
	return _Erc721.Contract.Erc721Transactor.contract.Transact(opts, method, params...)
}

// Call invokes the (constant) contract method with params as input values and
// sets the output to result. The result type might be a single field for simple
// returns, a slice of interfaces for anonymous returns and a struct for named
// returns.
func (_Erc721 *Erc721CallerRaw) Call(opts *bind.CallOpts, result *[]interface{}, method string, params ...interface{}) error {
	return _Erc721.Contract.contract.Call(opts, result, method, params...)
}

// Transfer initiates a plain transaction to move funds to the contract, calling
// its default method if one is available.
func (_Erc721 *Erc721TransactorRaw) Transfer(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _Erc721.Contract.contract.Transfer(opts)
}

// Transact invokes the (paid) contract method with params as input values.
func (_Erc721 *Erc721TransactorRaw) Transact(opts *bind.TransactOpts, method string, params ...interface{}) (*types.Transaction, error) {
	return _Erc721.Contract.contract.Transact(opts, method, params...)
}

// BalanceOf is a free data retrieval call binding the contract method 0x70a08231.
//
// Solidity: function balanceOf(address _owner) view returns(uint256 ret_0)
func (_Erc721 *Erc721Caller) BalanceOf(opts *bind.CallOpts, _owner common.Address) (*big.Int, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "balanceOf", _owner)

	if err != nil {
		return *new(*big.Int), err
	}

	out0 := *abi.ConvertType(out[0], new(*big.Int)).(**big.Int)

	return out0, err

}

// BalanceOf is a free data retrieval call binding the contract method 0x70a08231.
//
// Solidity: function balanceOf(address _owner) view returns(uint256 ret_0)
func (_Erc721 *Erc721Session) BalanceOf(_owner common.Address) (*big.Int, error) {
	return _Erc721.Contract.BalanceOf(&_Erc721.CallOpts, _owner)
}

// BalanceOf is a free data retrieval call binding the contract method 0x70a08231.
//
// Solidity: function balanceOf(address _owner) view returns(uint256 ret_0)
func (_Erc721 *Erc721CallerSession) BalanceOf(_owner common.Address) (*big.Int, error) {
	return _Erc721.Contract.BalanceOf(&_Erc721.CallOpts, _owner)
}

// GetApproved is a free data retrieval call binding the contract method 0x081812fc.
//
// Solidity: function getApproved(uint256 _tokenId) view returns(address ret_0)
func (_Erc721 *Erc721Caller) GetApproved(opts *bind.CallOpts, _tokenId *big.Int) (common.Address, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "getApproved", _tokenId)

	if err != nil {
		return *new(common.Address), err
	}

	out0 := *abi.ConvertType(out[0], new(common.Address)).(*common.Address)

	return out0, err

}

// GetApproved is a free data retrieval call binding the contract method 0x081812fc.
//
// Solidity: function getApproved(uint256 _tokenId) view returns(address ret_0)
func (_Erc721 *Erc721Session) GetApproved(_tokenId *big.Int) (common.Address, error) {
	return _Erc721.Contract.GetApproved(&_Erc721.CallOpts, _tokenId)
}

// GetApproved is a free data retrieval call binding the contract method 0x081812fc.
//
// Solidity: function getApproved(uint256 _tokenId) view returns(address ret_0)
func (_Erc721 *Erc721CallerSession) GetApproved(_tokenId *big.Int) (common.Address, error) {
	return _Erc721.Contract.GetApproved(&_Erc721.CallOpts, _tokenId)
}

// IsApprovedForAll is a free data retrieval call binding the contract method 0xe985e9c5.
//
// Solidity: function isApprovedForAll(address _owner, address _operator) view returns(bool ret_0)
func (_Erc721 *Erc721Caller) IsApprovedForAll(opts *bind.CallOpts, _owner common.Address, _operator common.Address) (bool, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "isApprovedForAll", _owner, _operator)

	if err != nil {
		return *new(bool), err
	}

	out0 := *abi.ConvertType(out[0], new(bool)).(*bool)

	return out0, err

}

// IsApprovedForAll is a free data retrieval call binding the contract method 0xe985e9c5.
//
// Solidity: function isApprovedForAll(address _owner, address _operator) view returns(bool ret_0)
func (_Erc721 *Erc721Session) IsApprovedForAll(_owner common.Address, _operator common.Address) (bool, error) {
	return _Erc721.Contract.IsApprovedForAll(&_Erc721.CallOpts, _owner, _operator)
}

// IsApprovedForAll is a free data retrieval call binding the contract method 0xe985e9c5.
//
// Solidity: function isApprovedForAll(address _owner, address _operator) view returns(bool ret_0)
func (_Erc721 *Erc721CallerSession) IsApprovedForAll(_owner common.Address, _operator common.Address) (bool, error) {
	return _Erc721.Contract.IsApprovedForAll(&_Erc721.CallOpts, _owner, _operator)
}

// Name is a free data retrieval call binding the contract method 0x06fdde03.
//
// Solidity: function name() view returns(string _name)
func (_Erc721 *Erc721Caller) Name(opts *bind.CallOpts) (string, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "name")

	if err != nil {
		return *new(string), err
	}

	out0 := *abi.ConvertType(out[0], new(string)).(*string)

	return out0, err

}

// Name is a free data retrieval call binding the contract method 0x06fdde03.
//
// Solidity: function name() view returns(string _name)
func (_Erc721 *Erc721Session) Name() (string, error) {
	return _Erc721.Contract.Name(&_Erc721.CallOpts)
}

// Name is a free data retrieval call binding the contract method 0x06fdde03.
//
// Solidity: function name() view returns(string _name)
func (_Erc721 *Erc721CallerSession) Name() (string, error) {
	return _Erc721.Contract.Name(&_Erc721.CallOpts)
}

// OwnerOf is a free data retrieval call binding the contract method 0x6352211e.
//
// Solidity: function ownerOf(uint256 _tokenId) view returns(address ret_0)
func (_Erc721 *Erc721Caller) OwnerOf(opts *bind.CallOpts, _tokenId *big.Int) (common.Address, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "ownerOf", _tokenId)

	if err != nil {
		return *new(common.Address), err
	}

	out0 := *abi.ConvertType(out[0], new(common.Address)).(*common.Address)

	return out0, err

}

// OwnerOf is a free data retrieval call binding the contract method 0x6352211e.
//
// Solidity: function ownerOf(uint256 _tokenId) view returns(address ret_0)
func (_Erc721 *Erc721Session) OwnerOf(_tokenId *big.Int) (common.Address, error) {
	return _Erc721.Contract.OwnerOf(&_Erc721.CallOpts, _tokenId)
}

// OwnerOf is a free data retrieval call binding the contract method 0x6352211e.
//
// Solidity: function ownerOf(uint256 _tokenId) view returns(address ret_0)
func (_Erc721 *Erc721CallerSession) OwnerOf(_tokenId *big.Int) (common.Address, error) {
	return _Erc721.Contract.OwnerOf(&_Erc721.CallOpts, _tokenId)
}

// SupportsInterface is a free data retrieval call binding the contract method 0x01ffc9a7.
//
// Solidity: function supportsInterface(bytes4 interfaceID) view returns(bool ret_0)
func (_Erc721 *Erc721Caller) SupportsInterface(opts *bind.CallOpts, interfaceID [4]byte) (bool, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "supportsInterface", interfaceID)

	if err != nil {
		return *new(bool), err
	}

	out0 := *abi.ConvertType(out[0], new(bool)).(*bool)

	return out0, err

}

// SupportsInterface is a free data retrieval call binding the contract method 0x01ffc9a7.
//
// Solidity: function supportsInterface(bytes4 interfaceID) view returns(bool ret_0)
func (_Erc721 *Erc721Session) SupportsInterface(interfaceID [4]byte) (bool, error) {
	return _Erc721.Contract.SupportsInterface(&_Erc721.CallOpts, interfaceID)
}

// SupportsInterface is a free data retrieval call binding the contract method 0x01ffc9a7.
//
// Solidity: function supportsInterface(bytes4 interfaceID) view returns(bool ret_0)
func (_Erc721 *Erc721CallerSession) SupportsInterface(interfaceID [4]byte) (bool, error) {
	return _Erc721.Contract.SupportsInterface(&_Erc721.CallOpts, interfaceID)
}

// Symbol is a free data retrieval call binding the contract method 0x95d89b41.
//
// Solidity: function symbol() view returns(string _symbol)
func (_Erc721 *Erc721Caller) Symbol(opts *bind.CallOpts) (string, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "symbol")

	if err != nil {
		return *new(string), err
	}

	out0 := *abi.ConvertType(out[0], new(string)).(*string)

	return out0, err

}

// Symbol is a free data retrieval call binding the contract method 0x95d89b41.
//
// Solidity: function symbol() view returns(string _symbol)
func (_Erc721 *Erc721Session) Symbol() (string, error) {
	return _Erc721.Contract.Symbol(&_Erc721.CallOpts)
}

// Symbol is a free data retrieval call binding the contract method 0x95d89b41.
//
// Solidity: function symbol() view returns(string _symbol)
func (_Erc721 *Erc721CallerSession) Symbol() (string, error) {
	return _Erc721.Contract.Symbol(&_Erc721.CallOpts)
}

// TokenByIndex is a free data retrieval call binding the contract method 0x4f6ccce7.
//
// Solidity: function tokenByIndex(uint256 _index) view returns(uint256 ret_0)
func (_Erc721 *Erc721Caller) TokenByIndex(opts *bind.CallOpts, _index *big.Int) (*big.Int, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "tokenByIndex", _index)

	if err != nil {
		return *new(*big.Int), err
	}

	out0 := *abi.ConvertType(out[0], new(*big.Int)).(**big.Int)

	return out0, err

}

// TokenByIndex is a free data retrieval call binding the contract method 0x4f6ccce7.
//
// Solidity: function tokenByIndex(uint256 _index) view returns(uint256 ret_0)
func (_Erc721 *Erc721Session) TokenByIndex(_index *big.Int) (*big.Int, error) {
	return _Erc721.Contract.TokenByIndex(&_Erc721.CallOpts, _index)
}

// TokenByIndex is a free data retrieval call binding the contract method 0x4f6ccce7.
//
// Solidity: function tokenByIndex(uint256 _index) view returns(uint256 ret_0)
func (_Erc721 *Erc721CallerSession) TokenByIndex(_index *big.Int) (*big.Int, error) {
	return _Erc721.Contract.TokenByIndex(&_Erc721.CallOpts, _index)
}

// TokenOfOwnerByIndex is a free data retrieval call binding the contract method 0x2f745c59.
//
// Solidity: function tokenOfOwnerByIndex(address _owner, uint256 _index) view returns(uint256 ret_0)
func (_Erc721 *Erc721Caller) TokenOfOwnerByIndex(opts *bind.CallOpts, _owner common.Address, _index *big.Int) (*big.Int, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "tokenOfOwnerByIndex", _owner, _index)

	if err != nil {
		return *new(*big.Int), err
	}

	out0 := *abi.ConvertType(out[0], new(*big.Int)).(**big.Int)

	return out0, err

}

// TokenOfOwnerByIndex is a free data retrieval call binding the contract method 0x2f745c59.
//
// Solidity: function tokenOfOwnerByIndex(address _owner, uint256 _index) view returns(uint256 ret_0)
func (_Erc721 *Erc721Session) TokenOfOwnerByIndex(_owner common.Address, _index *big.Int) (*big.Int, error) {
	return _Erc721.Contract.TokenOfOwnerByIndex(&_Erc721.CallOpts, _owner, _index)
}

// TokenOfOwnerByIndex is a free data retrieval call binding the contract method 0x2f745c59.
//
// Solidity: function tokenOfOwnerByIndex(address _owner, uint256 _index) view returns(uint256 ret_0)
func (_Erc721 *Erc721CallerSession) TokenOfOwnerByIndex(_owner common.Address, _index *big.Int) (*big.Int, error) {
	return _Erc721.Contract.TokenOfOwnerByIndex(&_Erc721.CallOpts, _owner, _index)
}

// TokenURI is a free data retrieval call binding the contract method 0xc87b56dd.
//
// Solidity: function tokenURI(uint256 _tokenId) view returns(string ret_0)
func (_Erc721 *Erc721Caller) TokenURI(opts *bind.CallOpts, _tokenId *big.Int) (string, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "tokenURI", _tokenId)

	if err != nil {
		return *new(string), err
	}

	out0 := *abi.ConvertType(out[0], new(string)).(*string)

	return out0, err

}

// TokenURI is a free data retrieval call binding the contract method 0xc87b56dd.
//
// Solidity: function tokenURI(uint256 _tokenId) view returns(string ret_0)
func (_Erc721 *Erc721Session) TokenURI(_tokenId *big.Int) (string, error) {
	return _Erc721.Contract.TokenURI(&_Erc721.CallOpts, _tokenId)
}

// TokenURI is a free data retrieval call binding the contract method 0xc87b56dd.
//
// Solidity: function tokenURI(uint256 _tokenId) view returns(string ret_0)
func (_Erc721 *Erc721CallerSession) TokenURI(_tokenId *big.Int) (string, error) {
	return _Erc721.Contract.TokenURI(&_Erc721.CallOpts, _tokenId)
}

// TotalSupply is a free data retrieval call binding the contract method 0x18160ddd.
//
// Solidity: function totalSupply() view returns(uint256 ret_0)
func (_Erc721 *Erc721Caller) TotalSupply(opts *bind.CallOpts) (*big.Int, error) {
	var out []interface{}
	err := _Erc721.contract.Call(opts, &out, "totalSupply")

	if err != nil {
		return *new(*big.Int), err
	}

	out0 := *abi.ConvertType(out[0], new(*big.Int)).(**big.Int)

	return out0, err

}

// TotalSupply is a free data retrieval call binding the contract method 0x18160ddd.
//
// Solidity: function totalSupply() view returns(uint256 ret_0)
func (_Erc721 *Erc721Session) TotalSupply() (*big.Int, error) {
	return _Erc721.Contract.TotalSupply(&_Erc721.CallOpts)
}

// TotalSupply is a free data retrieval call binding the contract method 0x18160ddd.
//
// Solidity: function totalSupply() view returns(uint256 ret_0)
func (_Erc721 *Erc721CallerSession) TotalSupply() (*big.Int, error) {
	return _Erc721.Contract.TotalSupply(&_Erc721.CallOpts)
}

// Approve is a paid mutator transaction binding the contract method 0x095ea7b3.
//
// Solidity: function approve(address _approved, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721Transactor) Approve(opts *bind.TransactOpts, _approved common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.contract.Transact(opts, "approve", _approved, _tokenId)
}

// Approve is a paid mutator transaction binding the contract method 0x095ea7b3.
//
// Solidity: function approve(address _approved, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721Session) Approve(_approved common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.Contract.Approve(&_Erc721.TransactOpts, _approved, _tokenId)
}

// Approve is a paid mutator transaction binding the contract method 0x095ea7b3.
//
// Solidity: function approve(address _approved, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721TransactorSession) Approve(_approved common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.Contract.Approve(&_Erc721.TransactOpts, _approved, _tokenId)
}

// OnERC721Received is a paid mutator transaction binding the contract method 0x150b7a02.
//
// Solidity: function onERC721Received(address _operator, address _from, uint256 _tokenId, bytes _data) returns(bytes4 ret_0)
func (_Erc721 *Erc721Transactor) OnERC721Received(opts *bind.TransactOpts, _operator common.Address, _from common.Address, _tokenId *big.Int, _data []byte) (*types.Transaction, error) {
	return _Erc721.contract.Transact(opts, "onERC721Received", _operator, _from, _tokenId, _data)
}

// OnERC721Received is a paid mutator transaction binding the contract method 0x150b7a02.
//
// Solidity: function onERC721Received(address _operator, address _from, uint256 _tokenId, bytes _data) returns(bytes4 ret_0)
func (_Erc721 *Erc721Session) OnERC721Received(_operator common.Address, _from common.Address, _tokenId *big.Int, _data []byte) (*types.Transaction, error) {
	return _Erc721.Contract.OnERC721Received(&_Erc721.TransactOpts, _operator, _from, _tokenId, _data)
}

// OnERC721Received is a paid mutator transaction binding the contract method 0x150b7a02.
//
// Solidity: function onERC721Received(address _operator, address _from, uint256 _tokenId, bytes _data) returns(bytes4 ret_0)
func (_Erc721 *Erc721TransactorSession) OnERC721Received(_operator common.Address, _from common.Address, _tokenId *big.Int, _data []byte) (*types.Transaction, error) {
	return _Erc721.Contract.OnERC721Received(&_Erc721.TransactOpts, _operator, _from, _tokenId, _data)
}

// SafeTransferFrom is a paid mutator transaction binding the contract method 0x42842e0e.
//
// Solidity: function safeTransferFrom(address _from, address _to, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721Transactor) SafeTransferFrom(opts *bind.TransactOpts, _from common.Address, _to common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.contract.Transact(opts, "safeTransferFrom", _from, _to, _tokenId)
}

// SafeTransferFrom is a paid mutator transaction binding the contract method 0x42842e0e.
//
// Solidity: function safeTransferFrom(address _from, address _to, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721Session) SafeTransferFrom(_from common.Address, _to common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.Contract.SafeTransferFrom(&_Erc721.TransactOpts, _from, _to, _tokenId)
}

// SafeTransferFrom is a paid mutator transaction binding the contract method 0x42842e0e.
//
// Solidity: function safeTransferFrom(address _from, address _to, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721TransactorSession) SafeTransferFrom(_from common.Address, _to common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.Contract.SafeTransferFrom(&_Erc721.TransactOpts, _from, _to, _tokenId)
}

// SafeTransferFrom0 is a paid mutator transaction binding the contract method 0xb88d4fde.
//
// Solidity: function safeTransferFrom(address _from, address _to, uint256 _tokenId, bytes data) payable returns()
func (_Erc721 *Erc721Transactor) SafeTransferFrom0(opts *bind.TransactOpts, _from common.Address, _to common.Address, _tokenId *big.Int, data []byte) (*types.Transaction, error) {
	return _Erc721.contract.Transact(opts, "safeTransferFrom0", _from, _to, _tokenId, data)
}

// SafeTransferFrom0 is a paid mutator transaction binding the contract method 0xb88d4fde.
//
// Solidity: function safeTransferFrom(address _from, address _to, uint256 _tokenId, bytes data) payable returns()
func (_Erc721 *Erc721Session) SafeTransferFrom0(_from common.Address, _to common.Address, _tokenId *big.Int, data []byte) (*types.Transaction, error) {
	return _Erc721.Contract.SafeTransferFrom0(&_Erc721.TransactOpts, _from, _to, _tokenId, data)
}

// SafeTransferFrom0 is a paid mutator transaction binding the contract method 0xb88d4fde.
//
// Solidity: function safeTransferFrom(address _from, address _to, uint256 _tokenId, bytes data) payable returns()
func (_Erc721 *Erc721TransactorSession) SafeTransferFrom0(_from common.Address, _to common.Address, _tokenId *big.Int, data []byte) (*types.Transaction, error) {
	return _Erc721.Contract.SafeTransferFrom0(&_Erc721.TransactOpts, _from, _to, _tokenId, data)
}

// SetApprovalForAll is a paid mutator transaction binding the contract method 0xa22cb465.
//
// Solidity: function setApprovalForAll(address _operator, bool _approved) returns()
func (_Erc721 *Erc721Transactor) SetApprovalForAll(opts *bind.TransactOpts, _operator common.Address, _approved bool) (*types.Transaction, error) {
	return _Erc721.contract.Transact(opts, "setApprovalForAll", _operator, _approved)
}

// SetApprovalForAll is a paid mutator transaction binding the contract method 0xa22cb465.
//
// Solidity: function setApprovalForAll(address _operator, bool _approved) returns()
func (_Erc721 *Erc721Session) SetApprovalForAll(_operator common.Address, _approved bool) (*types.Transaction, error) {
	return _Erc721.Contract.SetApprovalForAll(&_Erc721.TransactOpts, _operator, _approved)
}

// SetApprovalForAll is a paid mutator transaction binding the contract method 0xa22cb465.
//
// Solidity: function setApprovalForAll(address _operator, bool _approved) returns()
func (_Erc721 *Erc721TransactorSession) SetApprovalForAll(_operator common.Address, _approved bool) (*types.Transaction, error) {
	return _Erc721.Contract.SetApprovalForAll(&_Erc721.TransactOpts, _operator, _approved)
}

// TransferFrom is a paid mutator transaction binding the contract method 0x23b872dd.
//
// Solidity: function transferFrom(address _from, address _to, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721Transactor) TransferFrom(opts *bind.TransactOpts, _from common.Address, _to common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.contract.Transact(opts, "transferFrom", _from, _to, _tokenId)
}

// TransferFrom is a paid mutator transaction binding the contract method 0x23b872dd.
//
// Solidity: function transferFrom(address _from, address _to, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721Session) TransferFrom(_from common.Address, _to common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.Contract.TransferFrom(&_Erc721.TransactOpts, _from, _to, _tokenId)
}

// TransferFrom is a paid mutator transaction binding the contract method 0x23b872dd.
//
// Solidity: function transferFrom(address _from, address _to, uint256 _tokenId) payable returns()
func (_Erc721 *Erc721TransactorSession) TransferFrom(_from common.Address, _to common.Address, _tokenId *big.Int) (*types.Transaction, error) {
	return _Erc721.Contract.TransferFrom(&_Erc721.TransactOpts, _from, _to, _tokenId)
}

// Erc721ApprovalIterator is returned from FilterApproval and is used to iterate over the raw logs and unpacked data for Approval events raised by the Erc721 contract.
type Erc721ApprovalIterator struct {
	Event *Erc721Approval // Event containing the contract specifics and raw log

	contract *bind.BoundContract // Generic contract to use for unpacking event data
	event    string              // Event name to use for unpacking event data

	logs chan types.Log        // Log channel receiving the found contract events
	sub  ethereum.Subscription // Subscription for errors, completion and termination
	done bool                  // Whether the subscription completed delivering logs
	fail error                 // Occurred error to stop iteration
}

// Next advances the iterator to the subsequent event, returning whether there
// are any more events found. In case of a retrieval or parsing error, false is
// returned and Error() can be queried for the exact failure.
func (it *Erc721ApprovalIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(Erc721Approval)
			if err := it.contract.UnpackLog(it.Event, it.event, log); err != nil {
				it.fail = err
				return false
			}
			it.Event.Raw = log
			return true

		default:
			return false
		}
	}
	// Iterator still in progress, wait for either a data or an error event
	select {
	case log := <-it.logs:
		it.Event = new(Erc721Approval)
		if err := it.contract.UnpackLog(it.Event, it.event, log); err != nil {
			it.fail = err
			return false
		}
		it.Event.Raw = log
		return true

	case err := <-it.sub.Err():
		it.done = true
		it.fail = err
		return it.Next()
	}
}

// Error returns any retrieval or parsing error occurred during filtering.
func (it *Erc721ApprovalIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *Erc721ApprovalIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// Erc721Approval represents a Approval event raised by the Erc721 contract.
type Erc721Approval struct {
	Owner    common.Address
	Approved common.Address
	TokenId  *big.Int
	Raw      types.Log // Blockchain specific contextual infos
}

// FilterApproval is a free log retrieval operation binding the contract event 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925.
//
// Solidity: event Approval(address indexed _owner, address indexed _approved, uint256 indexed _tokenId)
func (_Erc721 *Erc721Filterer) FilterApproval(opts *bind.FilterOpts, _owner []common.Address, _approved []common.Address, _tokenId []*big.Int) (*Erc721ApprovalIterator, error) {

	var _ownerRule []interface{}
	for _, _ownerItem := range _owner {
		_ownerRule = append(_ownerRule, _ownerItem)
	}
	var _approvedRule []interface{}
	for _, _approvedItem := range _approved {
		_approvedRule = append(_approvedRule, _approvedItem)
	}
	var _tokenIdRule []interface{}
	for _, _tokenIdItem := range _tokenId {
		_tokenIdRule = append(_tokenIdRule, _tokenIdItem)
	}

	logs, sub, err := _Erc721.contract.FilterLogs(opts, "Approval", _ownerRule, _approvedRule, _tokenIdRule)
	if err != nil {
		return nil, err
	}
	return &Erc721ApprovalIterator{contract: _Erc721.contract, event: "Approval", logs: logs, sub: sub}, nil
}

// WatchApproval is a free log subscription operation binding the contract event 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925.
//
// Solidity: event Approval(address indexed _owner, address indexed _approved, uint256 indexed _tokenId)
func (_Erc721 *Erc721Filterer) WatchApproval(opts *bind.WatchOpts, sink chan<- *Erc721Approval, _owner []common.Address, _approved []common.Address, _tokenId []*big.Int) (event.Subscription, error) {

	var _ownerRule []interface{}
	for _, _ownerItem := range _owner {
		_ownerRule = append(_ownerRule, _ownerItem)
	}
	var _approvedRule []interface{}
	for _, _approvedItem := range _approved {
		_approvedRule = append(_approvedRule, _approvedItem)
	}
	var _tokenIdRule []interface{}
	for _, _tokenIdItem := range _tokenId {
		_tokenIdRule = append(_tokenIdRule, _tokenIdItem)
	}

	logs, sub, err := _Erc721.contract.WatchLogs(opts, "Approval", _ownerRule, _approvedRule, _tokenIdRule)
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(Erc721Approval)
				if err := _Erc721.contract.UnpackLog(event, "Approval", log); err != nil {
					return err
				}
				event.Raw = log

				select {
				case sink <- event:
				case err := <-sub.Err():
					return err
				case <-quit:
					return nil
				}
			case err := <-sub.Err():
				return err
			case <-quit:
				return nil
			}
		}
	}), nil
}

// ParseApproval is a log parse operation binding the contract event 0x8c5be1e5ebec7d5bd14f71427d1e84f3dd0314c0f7b2291e5b200ac8c7c3b925.
//
// Solidity: event Approval(address indexed _owner, address indexed _approved, uint256 indexed _tokenId)
func (_Erc721 *Erc721Filterer) ParseApproval(log types.Log) (*Erc721Approval, error) {
	event := new(Erc721Approval)
	if err := _Erc721.contract.UnpackLog(event, "Approval", log); err != nil {
		return nil, err
	}
	event.Raw = log
	return event, nil
}

// Erc721ApprovalForAllIterator is returned from FilterApprovalForAll and is used to iterate over the raw logs and unpacked data for ApprovalForAll events raised by the Erc721 contract.
type Erc721ApprovalForAllIterator struct {
	Event *Erc721ApprovalForAll // Event containing the contract specifics and raw log

	contract *bind.BoundContract // Generic contract to use for unpacking event data
	event    string              // Event name to use for unpacking event data

	logs chan types.Log        // Log channel receiving the found contract events
	sub  ethereum.Subscription // Subscription for errors, completion and termination
	done bool                  // Whether the subscription completed delivering logs
	fail error                 // Occurred error to stop iteration
}

// Next advances the iterator to the subsequent event, returning whether there
// are any more events found. In case of a retrieval or parsing error, false is
// returned and Error() can be queried for the exact failure.
func (it *Erc721ApprovalForAllIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(Erc721ApprovalForAll)
			if err := it.contract.UnpackLog(it.Event, it.event, log); err != nil {
				it.fail = err
				return false
			}
			it.Event.Raw = log
			return true

		default:
			return false
		}
	}
	// Iterator still in progress, wait for either a data or an error event
	select {
	case log := <-it.logs:
		it.Event = new(Erc721ApprovalForAll)
		if err := it.contract.UnpackLog(it.Event, it.event, log); err != nil {
			it.fail = err
			return false
		}
		it.Event.Raw = log
		return true

	case err := <-it.sub.Err():
		it.done = true
		it.fail = err
		return it.Next()
	}
}

// Error returns any retrieval or parsing error occurred during filtering.
func (it *Erc721ApprovalForAllIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *Erc721ApprovalForAllIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// Erc721ApprovalForAll represents a ApprovalForAll event raised by the Erc721 contract.
type Erc721ApprovalForAll struct {
	Owner    common.Address
	Operator common.Address
	Approved bool
	Raw      types.Log // Blockchain specific contextual infos
}

// FilterApprovalForAll is a free log retrieval operation binding the contract event 0x17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c31.
//
// Solidity: event ApprovalForAll(address indexed _owner, address indexed _operator, bool _approved)
func (_Erc721 *Erc721Filterer) FilterApprovalForAll(opts *bind.FilterOpts, _owner []common.Address, _operator []common.Address) (*Erc721ApprovalForAllIterator, error) {

	var _ownerRule []interface{}
	for _, _ownerItem := range _owner {
		_ownerRule = append(_ownerRule, _ownerItem)
	}
	var _operatorRule []interface{}
	for _, _operatorItem := range _operator {
		_operatorRule = append(_operatorRule, _operatorItem)
	}

	logs, sub, err := _Erc721.contract.FilterLogs(opts, "ApprovalForAll", _ownerRule, _operatorRule)
	if err != nil {
		return nil, err
	}
	return &Erc721ApprovalForAllIterator{contract: _Erc721.contract, event: "ApprovalForAll", logs: logs, sub: sub}, nil
}

// WatchApprovalForAll is a free log subscription operation binding the contract event 0x17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c31.
//
// Solidity: event ApprovalForAll(address indexed _owner, address indexed _operator, bool _approved)
func (_Erc721 *Erc721Filterer) WatchApprovalForAll(opts *bind.WatchOpts, sink chan<- *Erc721ApprovalForAll, _owner []common.Address, _operator []common.Address) (event.Subscription, error) {

	var _ownerRule []interface{}
	for _, _ownerItem := range _owner {
		_ownerRule = append(_ownerRule, _ownerItem)
	}
	var _operatorRule []interface{}
	for _, _operatorItem := range _operator {
		_operatorRule = append(_operatorRule, _operatorItem)
	}

	logs, sub, err := _Erc721.contract.WatchLogs(opts, "ApprovalForAll", _ownerRule, _operatorRule)
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(Erc721ApprovalForAll)
				if err := _Erc721.contract.UnpackLog(event, "ApprovalForAll", log); err != nil {
					return err
				}
				event.Raw = log

				select {
				case sink <- event:
				case err := <-sub.Err():
					return err
				case <-quit:
					return nil
				}
			case err := <-sub.Err():
				return err
			case <-quit:
				return nil
			}
		}
	}), nil
}

// ParseApprovalForAll is a log parse operation binding the contract event 0x17307eab39ab6107e8899845ad3d59bd9653f200f220920489ca2b5937696c31.
//
// Solidity: event ApprovalForAll(address indexed _owner, address indexed _operator, bool _approved)
func (_Erc721 *Erc721Filterer) ParseApprovalForAll(log types.Log) (*Erc721ApprovalForAll, error) {
	event := new(Erc721ApprovalForAll)
	if err := _Erc721.contract.UnpackLog(event, "ApprovalForAll", log); err != nil {
		return nil, err
	}
	event.Raw = log
	return event, nil
}

// Erc721TransferIterator is returned from FilterTransfer and is used to iterate over the raw logs and unpacked data for Transfer events raised by the Erc721 contract.
type Erc721TransferIterator struct {
	Event *Erc721Transfer // Event containing the contract specifics and raw log

	contract *bind.BoundContract // Generic contract to use for unpacking event data
	event    string              // Event name to use for unpacking event data

	logs chan types.Log        // Log channel receiving the found contract events
	sub  ethereum.Subscription // Subscription for errors, completion and termination
	done bool                  // Whether the subscription completed delivering logs
	fail error                 // Occurred error to stop iteration
}

// Next advances the iterator to the subsequent event, returning whether there
// are any more events found. In case of a retrieval or parsing error, false is
// returned and Error() can be queried for the exact failure.
func (it *Erc721TransferIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(Erc721Transfer)
			if err := it.contract.UnpackLog(it.Event, it.event, log); err != nil {
				it.fail = err
				return false
			}
			it.Event.Raw = log
			return true

		default:
			return false
		}
	}
	// Iterator still in progress, wait for either a data or an error event
	select {
	case log := <-it.logs:
		it.Event = new(Erc721Transfer)
		if err := it.contract.UnpackLog(it.Event, it.event, log); err != nil {
			it.fail = err
			return false
		}
		it.Event.Raw = log
		return true

	case err := <-it.sub.Err():
		it.done = true
		it.fail = err
		return it.Next()
	}
}

// Error returns any retrieval or parsing error occurred during filtering.
func (it *Erc721TransferIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *Erc721TransferIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// Erc721Transfer represents a Transfer event raised by the Erc721 contract.
type Erc721Transfer struct {
	From    common.Address
	To      common.Address
	TokenId *big.Int
	Raw     types.Log // Blockchain specific contextual infos
}

// FilterTransfer is a free log retrieval operation binding the contract event 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef.
//
// Solidity: event Transfer(address indexed _from, address indexed _to, uint256 indexed _tokenId)
func (_Erc721 *Erc721Filterer) FilterTransfer(opts *bind.FilterOpts, _from []common.Address, _to []common.Address, _tokenId []*big.Int) (*Erc721TransferIterator, error) {

	var _fromRule []interface{}
	for _, _fromItem := range _from {
		_fromRule = append(_fromRule, _fromItem)
	}
	var _toRule []interface{}
	for _, _toItem := range _to {
		_toRule = append(_toRule, _toItem)
	}
	var _tokenIdRule []interface{}
	for _, _tokenIdItem := range _tokenId {
		_tokenIdRule = append(_tokenIdRule, _tokenIdItem)
	}

	logs, sub, err := _Erc721.contract.FilterLogs(opts, "Transfer", _fromRule, _toRule, _tokenIdRule)
	if err != nil {
		return nil, err
	}
	return &Erc721TransferIterator{contract: _Erc721.contract, event: "Transfer", logs: logs, sub: sub}, nil
}

// WatchTransfer is a free log subscription operation binding the contract event 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef.
//
// Solidity: event Transfer(address indexed _from, address indexed _to, uint256 indexed _tokenId)
func (_Erc721 *Erc721Filterer) WatchTransfer(opts *bind.WatchOpts, sink chan<- *Erc721Transfer, _from []common.Address, _to []common.Address, _tokenId []*big.Int) (event.Subscription, error) {

	var _fromRule []interface{}
	for _, _fromItem := range _from {
		_fromRule = append(_fromRule, _fromItem)
	}
	var _toRule []interface{}
	for _, _toItem := range _to {
		_toRule = append(_toRule, _toItem)
	}
	var _tokenIdRule []interface{}
	for _, _tokenIdItem := range _tokenId {
		_tokenIdRule = append(_tokenIdRule, _tokenIdItem)
	}

	logs, sub, err := _Erc721.contract.WatchLogs(opts, "Transfer", _fromRule, _toRule, _tokenIdRule)
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(Erc721Transfer)
				if err := _Erc721.contract.UnpackLog(event, "Transfer", log); err != nil {
					return err
				}
				event.Raw = log

				select {
				case sink <- event:
				case err := <-sub.Err():
					return err
				case <-quit:
					return nil
				}
			case err := <-sub.Err():
				return err
			case <-quit:
				return nil
			}
		}
	}), nil
}

// ParseTransfer is a log parse operation binding the contract event 0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef.
//
// Solidity: event Transfer(address indexed _from, address indexed _to, uint256 indexed _tokenId)
func (_Erc721 *Erc721Filterer) ParseTransfer(log types.Log) (*Erc721Transfer, error) {
	event := new(Erc721Transfer)
	if err := _Erc721.contract.UnpackLog(event, "Transfer", log); err != nil {
		return nil, err
	}
	event.Raw = log
	return event, nil
}
