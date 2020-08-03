// Code generated - DO NOT EDIT.
// This file is a generated binding and any manual changes will be lost.

package main

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

// UnchainedIndexABI is the input ABI used to generate the binding from.
const UnchainedIndexABI = "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"string\",\"name\":\"hash\",\"type\":\"string\"}],\"name\":\"HashPublished\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"address\",\"name\":\"oldOwner\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"address\",\"name\":\"newOwner\",\"type\":\"address\"}],\"name\":\"OwnerChanged\",\"type\":\"event\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"newOwner\",\"type\":\"address\"}],\"name\":\"changeOwner\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"oldOwner\",\"type\":\"address\"}],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"indexHash\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"owner\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"string\",\"name\":\"hash\",\"type\":\"string\"}],\"name\":\"publishHash\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"}]"

// UnchainedIndexFuncSigs maps the 4-byte function signature to its string representation.
var UnchainedIndexFuncSigs = map[string]string{
	"a6f9dae1": "changeOwner(address)",
	"5cb8eca4": "indexHash()",
	"8da5cb5b": "owner()",
	"8f1f9902": "publishHash(string)",
}

// UnchainedIndexBin is the compiled bytecode used for deploying new contracts.
var UnchainedIndexBin = "0x608060405234801561001057600080fd5b50600180546001600160a01b031916331790556040805160608101909152602e8082526106dd6020830139805161004f91600091602090910190610149565b5060408051602080825260008054600260001961010060018416150201909116049183018290527f0488fc5a0e346a47f90cfa4d25262d1a430445fa176c4b7000f005bc2f474b73939092918291820190849080156100ef5780601f106100c4576101008083540402835291602001916100ef565b820191906000526020600020905b8154815290600101906020018083116100d257829003601f168201915b50509250505060405180910390a160015460408051600081526001600160a01b03909216602083015280517fb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c9281900390910190a16101dc565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061018a57805160ff19168380011785556101b7565b828001600101855582156101b7579182015b828111156101b757825182559160200191906001019061019c565b506101c39291506101c7565b5090565b5b808211156101c357600081556001016101c8565b6104f2806101eb6000396000f3fe608060405234801561001057600080fd5b506004361061004c5760003560e01c80635cb8eca4146100515780638da5cb5b146100ce5780638f1f9902146100f2578063a6f9dae11461019a575b600080fd5b6100596101c0565b6040805160208082528351818301528351919283929083019185019080838360005b8381101561009357818101518382015260200161007b565b50505050905090810190601f1680156100c05780820380516001836020036101000a031916815260200191505b509250505060405180910390f35b6100d661024e565b604080516001600160a01b039092168252519081900360200190f35b6101986004803603602081101561010857600080fd5b81019060208101813564010000000081111561012357600080fd5b82018360208201111561013557600080fd5b8035906020019184600183028401116401000000008311171561015757600080fd5b91908080601f01602080910402602001604051908101604052809392919081815260200183838082843760009201919091525092955061025d945050505050565b005b6100d6600480360360208110156101b057600080fd5b50356001600160a01b0316610367565b6000805460408051602060026001851615610100026000190190941693909304601f810184900484028201840190925281815292918301828280156102465780601f1061021b57610100808354040283529160200191610246565b820191906000526020600020905b81548152906001019060200180831161022957829003601f168201915b505050505081565b6001546001600160a01b031681565b6001546001600160a01b031633146102b7576040805162461bcd60e51b815260206004820152601860248201527736b9b39739b2b73232b91036bab9ba1031329037bbb732b960411b604482015290519081900360640190fd5b80516102ca906000906020840190610429565b507f0488fc5a0e346a47f90cfa4d25262d1a430445fa176c4b7000f005bc2f474b73816040518080602001828103825283818151815260200191508051906020019080838360005b8381101561032a578181015183820152602001610312565b50505050905090810190601f1680156103575780820380516001836020036101000a031916815260200191505b509250505060405180910390a150565b6001546000906001600160a01b031633146103c4576040805162461bcd60e51b815260206004820152601860248201527736b9b39739b2b73232b91036bab9ba1031329037bbb732b960411b604482015290519081900360640190fd5b50600180546001600160a01b038381166001600160a01b0319831681179093556040805191909216808252602082019390935281517fb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c929181900390910190a1919050565b828054600181600116156101000203166002900490600052602060002090601f016020900481019282601f1061046a57805160ff1916838001178555610497565b82800160010185558215610497579182015b8281111561049757825182559160200191906001019061047c565b506104a39291506104a7565b5090565b5b808211156104a357600081556001016104a856fea2646970667358221220cb1a86485a3c7c2f7ecfb4df68c4b5176201f208ae98f2a8db84922261703a7564736f6c634300060c0033516d62464d6b65314b58716e5979424257784237344e34633553426e4a4d5641694d4e5263477536783141775148"

// DeployUnchainedIndex deploys a new Ethereum contract, binding an instance of UnchainedIndex to it.
func DeployUnchainedIndex(auth *bind.TransactOpts, backend bind.ContractBackend) (common.Address, *types.Transaction, *UnchainedIndex, error) {
	parsed, err := abi.JSON(strings.NewReader(UnchainedIndexABI))
	if err != nil {
		return common.Address{}, nil, nil, err
	}

	address, tx, contract, err := bind.DeployContract(auth, parsed, common.FromHex(UnchainedIndexBin), backend)
	if err != nil {
		return common.Address{}, nil, nil, err
	}
	return address, tx, &UnchainedIndex{UnchainedIndexCaller: UnchainedIndexCaller{contract: contract}, UnchainedIndexTransactor: UnchainedIndexTransactor{contract: contract}, UnchainedIndexFilterer: UnchainedIndexFilterer{contract: contract}}, nil
}

// UnchainedIndex is an auto generated Go binding around an Ethereum contract.
type UnchainedIndex struct {
	UnchainedIndexCaller     // Read-only binding to the contract
	UnchainedIndexTransactor // Write-only binding to the contract
	UnchainedIndexFilterer   // Log filterer for contract events
}

// UnchainedIndexCaller is an auto generated read-only Go binding around an Ethereum contract.
type UnchainedIndexCaller struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// UnchainedIndexTransactor is an auto generated write-only Go binding around an Ethereum contract.
type UnchainedIndexTransactor struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// UnchainedIndexFilterer is an auto generated log filtering Go binding around an Ethereum contract events.
type UnchainedIndexFilterer struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// UnchainedIndexSession is an auto generated Go binding around an Ethereum contract,
// with pre-set call and transact options.
type UnchainedIndexSession struct {
	Contract     *UnchainedIndex   // Generic contract binding to set the session for
	CallOpts     bind.CallOpts     // Call options to use throughout this session
	TransactOpts bind.TransactOpts // Transaction auth options to use throughout this session
}

// UnchainedIndexCallerSession is an auto generated read-only Go binding around an Ethereum contract,
// with pre-set call options.
type UnchainedIndexCallerSession struct {
	Contract *UnchainedIndexCaller // Generic contract caller binding to set the session for
	CallOpts bind.CallOpts         // Call options to use throughout this session
}

// UnchainedIndexTransactorSession is an auto generated write-only Go binding around an Ethereum contract,
// with pre-set transact options.
type UnchainedIndexTransactorSession struct {
	Contract     *UnchainedIndexTransactor // Generic contract transactor binding to set the session for
	TransactOpts bind.TransactOpts         // Transaction auth options to use throughout this session
}

// UnchainedIndexRaw is an auto generated low-level Go binding around an Ethereum contract.
type UnchainedIndexRaw struct {
	Contract *UnchainedIndex // Generic contract binding to access the raw methods on
}

// UnchainedIndexCallerRaw is an auto generated low-level read-only Go binding around an Ethereum contract.
type UnchainedIndexCallerRaw struct {
	Contract *UnchainedIndexCaller // Generic read-only contract binding to access the raw methods on
}

// UnchainedIndexTransactorRaw is an auto generated low-level write-only Go binding around an Ethereum contract.
type UnchainedIndexTransactorRaw struct {
	Contract *UnchainedIndexTransactor // Generic write-only contract binding to access the raw methods on
}

// NewUnchainedIndex creates a new instance of UnchainedIndex, bound to a specific deployed contract.
func NewUnchainedIndex(address common.Address, backend bind.ContractBackend) (*UnchainedIndex, error) {
	contract, err := bindUnchainedIndex(address, backend, backend, backend)
	if err != nil {
		return nil, err
	}
	return &UnchainedIndex{UnchainedIndexCaller: UnchainedIndexCaller{contract: contract}, UnchainedIndexTransactor: UnchainedIndexTransactor{contract: contract}, UnchainedIndexFilterer: UnchainedIndexFilterer{contract: contract}}, nil
}

// NewUnchainedIndexCaller creates a new read-only instance of UnchainedIndex, bound to a specific deployed contract.
func NewUnchainedIndexCaller(address common.Address, caller bind.ContractCaller) (*UnchainedIndexCaller, error) {
	contract, err := bindUnchainedIndex(address, caller, nil, nil)
	if err != nil {
		return nil, err
	}
	return &UnchainedIndexCaller{contract: contract}, nil
}

// NewUnchainedIndexTransactor creates a new write-only instance of UnchainedIndex, bound to a specific deployed contract.
func NewUnchainedIndexTransactor(address common.Address, transactor bind.ContractTransactor) (*UnchainedIndexTransactor, error) {
	contract, err := bindUnchainedIndex(address, nil, transactor, nil)
	if err != nil {
		return nil, err
	}
	return &UnchainedIndexTransactor{contract: contract}, nil
}

// NewUnchainedIndexFilterer creates a new log filterer instance of UnchainedIndex, bound to a specific deployed contract.
func NewUnchainedIndexFilterer(address common.Address, filterer bind.ContractFilterer) (*UnchainedIndexFilterer, error) {
	contract, err := bindUnchainedIndex(address, nil, nil, filterer)
	if err != nil {
		return nil, err
	}
	return &UnchainedIndexFilterer{contract: contract}, nil
}

// bindUnchainedIndex binds a generic wrapper to an already deployed contract.
func bindUnchainedIndex(address common.Address, caller bind.ContractCaller, transactor bind.ContractTransactor, filterer bind.ContractFilterer) (*bind.BoundContract, error) {
	parsed, err := abi.JSON(strings.NewReader(UnchainedIndexABI))
	if err != nil {
		return nil, err
	}
	return bind.NewBoundContract(address, parsed, caller, transactor, filterer), nil
}

// Call invokes the (constant) contract method with params as input values and
// sets the output to result. The result type might be a single field for simple
// returns, a slice of interfaces for anonymous returns and a struct for named
// returns.
func (_UnchainedIndex *UnchainedIndexRaw) Call(opts *bind.CallOpts, result interface{}, method string, params ...interface{}) error {
	return _UnchainedIndex.Contract.UnchainedIndexCaller.contract.Call(opts, result, method, params...)
}

// Transfer initiates a plain transaction to move funds to the contract, calling
// its default method if one is available.
func (_UnchainedIndex *UnchainedIndexRaw) Transfer(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.UnchainedIndexTransactor.contract.Transfer(opts)
}

// Transact invokes the (paid) contract method with params as input values.
func (_UnchainedIndex *UnchainedIndexRaw) Transact(opts *bind.TransactOpts, method string, params ...interface{}) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.UnchainedIndexTransactor.contract.Transact(opts, method, params...)
}

// Call invokes the (constant) contract method with params as input values and
// sets the output to result. The result type might be a single field for simple
// returns, a slice of interfaces for anonymous returns and a struct for named
// returns.
func (_UnchainedIndex *UnchainedIndexCallerRaw) Call(opts *bind.CallOpts, result interface{}, method string, params ...interface{}) error {
	return _UnchainedIndex.Contract.contract.Call(opts, result, method, params...)
}

// Transfer initiates a plain transaction to move funds to the contract, calling
// its default method if one is available.
func (_UnchainedIndex *UnchainedIndexTransactorRaw) Transfer(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.contract.Transfer(opts)
}

// Transact invokes the (paid) contract method with params as input values.
func (_UnchainedIndex *UnchainedIndexTransactorRaw) Transact(opts *bind.TransactOpts, method string, params ...interface{}) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.contract.Transact(opts, method, params...)
}

// IndexHash is a free data retrieval call binding the contract method 0x5cb8eca4.
//
// Solidity: function indexHash() view returns(string)
func (_UnchainedIndex *UnchainedIndexCaller) IndexHash(opts *bind.CallOpts) (string, error) {
	var (
		ret0 = new(string)
	)
	out := ret0
	err := _UnchainedIndex.contract.Call(opts, out, "indexHash")
	return *ret0, err
}

// IndexHash is a free data retrieval call binding the contract method 0x5cb8eca4.
//
// Solidity: function indexHash() view returns(string)
func (_UnchainedIndex *UnchainedIndexSession) IndexHash() (string, error) {
	return _UnchainedIndex.Contract.IndexHash(&_UnchainedIndex.CallOpts)
}

// IndexHash is a free data retrieval call binding the contract method 0x5cb8eca4.
//
// Solidity: function indexHash() view returns(string)
func (_UnchainedIndex *UnchainedIndexCallerSession) IndexHash() (string, error) {
	return _UnchainedIndex.Contract.IndexHash(&_UnchainedIndex.CallOpts)
}

// Owner is a free data retrieval call binding the contract method 0x8da5cb5b.
//
// Solidity: function owner() view returns(address)
func (_UnchainedIndex *UnchainedIndexCaller) Owner(opts *bind.CallOpts) (common.Address, error) {
	var (
		ret0 = new(common.Address)
	)
	out := ret0
	err := _UnchainedIndex.contract.Call(opts, out, "owner")
	return *ret0, err
}

// Owner is a free data retrieval call binding the contract method 0x8da5cb5b.
//
// Solidity: function owner() view returns(address)
func (_UnchainedIndex *UnchainedIndexSession) Owner() (common.Address, error) {
	return _UnchainedIndex.Contract.Owner(&_UnchainedIndex.CallOpts)
}

// Owner is a free data retrieval call binding the contract method 0x8da5cb5b.
//
// Solidity: function owner() view returns(address)
func (_UnchainedIndex *UnchainedIndexCallerSession) Owner() (common.Address, error) {
	return _UnchainedIndex.Contract.Owner(&_UnchainedIndex.CallOpts)
}

// ChangeOwner is a paid mutator transaction binding the contract method 0xa6f9dae1.
//
// Solidity: function changeOwner(address newOwner) returns(address oldOwner)
func (_UnchainedIndex *UnchainedIndexTransactor) ChangeOwner(opts *bind.TransactOpts, newOwner common.Address) (*types.Transaction, error) {
	return _UnchainedIndex.contract.Transact(opts, "changeOwner", newOwner)
}

// ChangeOwner is a paid mutator transaction binding the contract method 0xa6f9dae1.
//
// Solidity: function changeOwner(address newOwner) returns(address oldOwner)
func (_UnchainedIndex *UnchainedIndexSession) ChangeOwner(newOwner common.Address) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.ChangeOwner(&_UnchainedIndex.TransactOpts, newOwner)
}

// ChangeOwner is a paid mutator transaction binding the contract method 0xa6f9dae1.
//
// Solidity: function changeOwner(address newOwner) returns(address oldOwner)
func (_UnchainedIndex *UnchainedIndexTransactorSession) ChangeOwner(newOwner common.Address) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.ChangeOwner(&_UnchainedIndex.TransactOpts, newOwner)
}

// PublishHash is a paid mutator transaction binding the contract method 0x8f1f9902.
//
// Solidity: function publishHash(string hash) returns()
func (_UnchainedIndex *UnchainedIndexTransactor) PublishHash(opts *bind.TransactOpts, hash string) (*types.Transaction, error) {
	return _UnchainedIndex.contract.Transact(opts, "publishHash", hash)
}

// PublishHash is a paid mutator transaction binding the contract method 0x8f1f9902.
//
// Solidity: function publishHash(string hash) returns()
func (_UnchainedIndex *UnchainedIndexSession) PublishHash(hash string) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.PublishHash(&_UnchainedIndex.TransactOpts, hash)
}

// PublishHash is a paid mutator transaction binding the contract method 0x8f1f9902.
//
// Solidity: function publishHash(string hash) returns()
func (_UnchainedIndex *UnchainedIndexTransactorSession) PublishHash(hash string) (*types.Transaction, error) {
	return _UnchainedIndex.Contract.PublishHash(&_UnchainedIndex.TransactOpts, hash)
}

// UnchainedIndexHashPublishedIterator is returned from FilterHashPublished and is used to iterate over the raw logs and unpacked data for HashPublished events raised by the UnchainedIndex contract.
type UnchainedIndexHashPublishedIterator struct {
	Event *UnchainedIndexHashPublished // Event containing the contract specifics and raw log

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
func (it *UnchainedIndexHashPublishedIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(UnchainedIndexHashPublished)
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
		it.Event = new(UnchainedIndexHashPublished)
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
func (it *UnchainedIndexHashPublishedIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *UnchainedIndexHashPublishedIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// UnchainedIndexHashPublished represents a HashPublished event raised by the UnchainedIndex contract.
type UnchainedIndexHashPublished struct {
	Hash string
	Raw  types.Log // Blockchain specific contextual infos
}

// FilterHashPublished is a free log retrieval operation binding the contract event 0x0488fc5a0e346a47f90cfa4d25262d1a430445fa176c4b7000f005bc2f474b73.
//
// Solidity: event HashPublished(string hash)
func (_UnchainedIndex *UnchainedIndexFilterer) FilterHashPublished(opts *bind.FilterOpts) (*UnchainedIndexHashPublishedIterator, error) {

	logs, sub, err := _UnchainedIndex.contract.FilterLogs(opts, "HashPublished")
	if err != nil {
		return nil, err
	}
	return &UnchainedIndexHashPublishedIterator{contract: _UnchainedIndex.contract, event: "HashPublished", logs: logs, sub: sub}, nil
}

// WatchHashPublished is a free log subscription operation binding the contract event 0x0488fc5a0e346a47f90cfa4d25262d1a430445fa176c4b7000f005bc2f474b73.
//
// Solidity: event HashPublished(string hash)
func (_UnchainedIndex *UnchainedIndexFilterer) WatchHashPublished(opts *bind.WatchOpts, sink chan<- *UnchainedIndexHashPublished) (event.Subscription, error) {

	logs, sub, err := _UnchainedIndex.contract.WatchLogs(opts, "HashPublished")
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(UnchainedIndexHashPublished)
				if err := _UnchainedIndex.contract.UnpackLog(event, "HashPublished", log); err != nil {
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

// ParseHashPublished is a log parse operation binding the contract event 0x0488fc5a0e346a47f90cfa4d25262d1a430445fa176c4b7000f005bc2f474b73.
//
// Solidity: event HashPublished(string hash)
func (_UnchainedIndex *UnchainedIndexFilterer) ParseHashPublished(log types.Log) (*UnchainedIndexHashPublished, error) {
	event := new(UnchainedIndexHashPublished)
	if err := _UnchainedIndex.contract.UnpackLog(event, "HashPublished", log); err != nil {
		return nil, err
	}
	return event, nil
}

// UnchainedIndexOwnerChangedIterator is returned from FilterOwnerChanged and is used to iterate over the raw logs and unpacked data for OwnerChanged events raised by the UnchainedIndex contract.
type UnchainedIndexOwnerChangedIterator struct {
	Event *UnchainedIndexOwnerChanged // Event containing the contract specifics and raw log

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
func (it *UnchainedIndexOwnerChangedIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(UnchainedIndexOwnerChanged)
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
		it.Event = new(UnchainedIndexOwnerChanged)
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
func (it *UnchainedIndexOwnerChangedIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *UnchainedIndexOwnerChangedIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// UnchainedIndexOwnerChanged represents a OwnerChanged event raised by the UnchainedIndex contract.
type UnchainedIndexOwnerChanged struct {
	OldOwner common.Address
	NewOwner common.Address
	Raw      types.Log // Blockchain specific contextual infos
}

// FilterOwnerChanged is a free log retrieval operation binding the contract event 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c.
//
// Solidity: event OwnerChanged(address oldOwner, address newOwner)
func (_UnchainedIndex *UnchainedIndexFilterer) FilterOwnerChanged(opts *bind.FilterOpts) (*UnchainedIndexOwnerChangedIterator, error) {

	logs, sub, err := _UnchainedIndex.contract.FilterLogs(opts, "OwnerChanged")
	if err != nil {
		return nil, err
	}
	return &UnchainedIndexOwnerChangedIterator{contract: _UnchainedIndex.contract, event: "OwnerChanged", logs: logs, sub: sub}, nil
}

// WatchOwnerChanged is a free log subscription operation binding the contract event 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c.
//
// Solidity: event OwnerChanged(address oldOwner, address newOwner)
func (_UnchainedIndex *UnchainedIndexFilterer) WatchOwnerChanged(opts *bind.WatchOpts, sink chan<- *UnchainedIndexOwnerChanged) (event.Subscription, error) {

	logs, sub, err := _UnchainedIndex.contract.WatchLogs(opts, "OwnerChanged")
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(UnchainedIndexOwnerChanged)
				if err := _UnchainedIndex.contract.UnpackLog(event, "OwnerChanged", log); err != nil {
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

// ParseOwnerChanged is a log parse operation binding the contract event 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c.
//
// Solidity: event OwnerChanged(address oldOwner, address newOwner)
func (_UnchainedIndex *UnchainedIndexFilterer) ParseOwnerChanged(log types.Log) (*UnchainedIndexOwnerChanged, error) {
	event := new(UnchainedIndexOwnerChanged)
	if err := _UnchainedIndex.contract.UnpackLog(event, "OwnerChanged", log); err != nil {
		return nil, err
	}
	return event, nil
}
