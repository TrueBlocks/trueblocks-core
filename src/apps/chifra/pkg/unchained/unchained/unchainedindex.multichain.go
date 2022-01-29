// Code generated - DO NOT EDIT.
// This file is a generated binding and any manual changes will be lost.

package unchained

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

// UnchainedABI is the input ABI used to generate the binding from.
const UnchainedABI = "[{\"inputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"constructor\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"address\",\"name\":\"from\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"amount\",\"type\":\"uint256\"},{\"indexed\":false,\"internalType\":\"uint256\",\"name\":\"ts\",\"type\":\"uint256\"}],\"name\":\"DonationSent\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"string\",\"name\":\"chainName\",\"type\":\"string\"},{\"indexed\":false,\"internalType\":\"string\",\"name\":\"hash\",\"type\":\"string\"}],\"name\":\"HashPublished\",\"type\":\"event\"},{\"anonymous\":false,\"inputs\":[{\"indexed\":false,\"internalType\":\"address\",\"name\":\"oldOwner\",\"type\":\"address\"},{\"indexed\":false,\"internalType\":\"address\",\"name\":\"newOwner\",\"type\":\"address\"}],\"name\":\"OwnerChanged\",\"type\":\"event\"},{\"inputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"name\":\"chainNameToHash\",\"outputs\":[{\"internalType\":\"string\",\"name\":\"\",\"type\":\"string\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"address\",\"name\":\"newOwner\",\"type\":\"address\"}],\"name\":\"changeOwner\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"oldOwner\",\"type\":\"address\"}],\"stateMutability\":\"nonpayable\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"donate\",\"outputs\":[],\"stateMutability\":\"payable\",\"type\":\"function\"},{\"inputs\":[],\"name\":\"owner\",\"outputs\":[{\"internalType\":\"address\",\"name\":\"\",\"type\":\"address\"}],\"stateMutability\":\"view\",\"type\":\"function\"},{\"inputs\":[{\"internalType\":\"string\",\"name\":\"chainName\",\"type\":\"string\"},{\"internalType\":\"string\",\"name\":\"hash\",\"type\":\"string\"}],\"name\":\"publishHash\",\"outputs\":[],\"stateMutability\":\"nonpayable\",\"type\":\"function\"}]"

// UnchainedBin is the compiled bytecode used for deploying new contracts.
var UnchainedBin = "0x608060405234801561001057600080fd5b50600180546001600160a01b031916331790556040805160608101909152602e8082526109fa602083013960408051661b585a5b9b995d60ca1b8152600060078201529051908190036027019020815161006d9260200190610116565b507fc89cce2e1c4073dc6c892fd1d1320b76e81c7e493f153b9a51f264d10ab2234a60006040516100ab90661b585a5b9b995d60ca1b815260070190565b9081526040519081900360200181206100c3916101ea565b60405180910390a160015460408051600081526001600160a01b0390921660208301527fb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c910160405180910390a16102b2565b828054610122906101af565b90600052602060002090601f016020900481019282610144576000855561018a565b82601f1061015d57805160ff191683800117855561018a565b8280016001018555821561018a579182015b8281111561018a57825182559160200191906001019061016f565b5061019692915061019a565b5090565b5b80821115610196576000815560010161019b565b600181811c908216806101c357607f821691505b602082108114156101e457634e487b7160e01b600052602260045260246000fd5b50919050565b6040815260076040820152661b585a5b9b995d60ca1b6060820152600060206080818401526000845481600182811c91508083168061022a57607f831692505b85831081141561024857634e487b7160e01b85526022600452602485fd5b6080880183905260a088018180156102675760018114610278576102a3565b60ff198616825287820196506102a3565b60008b81526020902060005b8681101561029d57815484820152908501908901610284565b83019750505b50949998505050505050505050565b610739806102c16000396000f3fe60806040526004361061004a5760003560e01c80631fee5cd21461004f57806355a1e706146100715780638da5cb5b146100a7578063a6f9dae1146100df578063ed88c68e146100ff575b600080fd5b34801561005b57600080fd5b5061006f61006a36600461053e565b610107565b005b34801561007d57600080fd5b5061009161008c3660046105a2565b6101d1565b60405161009e919061063b565b60405180910390f35b3480156100b357600080fd5b506001546100c7906001600160a01b031681565b6040516001600160a01b03909116815260200161009e565b3480156100eb57600080fd5b506100c76100fa366004610655565b610276565b61006f610331565b6001546001600160a01b031633146101615760405162461bcd60e51b815260206004820152601860248201527736b9b39739b2b73232b91036bab9ba1031329037bbb732b960411b60448201526064015b60405180910390fd5b80600083604051610172919061067e565b90815260200160405180910390209080519060200190610193929190610402565b507fc89cce2e1c4073dc6c892fd1d1320b76e81c7e493f153b9a51f264d10ab2234a82826040516101c592919061069a565b60405180910390a15050565b8051602081830181018051600082529282019190930120915280546101f5906106c8565b80601f0160208091040260200160405190810160405280929190818152602001828054610221906106c8565b801561026e5780601f106102435761010080835404028352916020019161026e565b820191906000526020600020905b81548152906001019060200180831161025157829003601f168201915b505050505081565b6001546000906001600160a01b031633146102ce5760405162461bcd60e51b815260206004820152601860248201527736b9b39739b2b73232b91036bab9ba1031329037bbb732b960411b6044820152606401610158565b50600180546001600160a01b038381166001600160a01b031983168117909355604080519190921680825260208201939093527fb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c910160405180910390a1919050565b6001546001600160a01b031661037c5760405162461bcd60e51b815260206004820152601060248201526f1bdddb995c881a5cc81b9bdd081cd95d60821b6044820152606401610158565b600154604080516001600160a01b0390921682523460208301524282820152517f8d68ee857f05464080096b8fbaa1603c09866dfccd25bb348209b1188e60add29181900360600190a16001546040516001600160a01b03909116904780156108fc02916000818181858888f193505050501580156103ff573d6000803e3d6000fd5b50565b82805461040e906106c8565b90600052602060002090601f0160209004810192826104305760008555610476565b82601f1061044957805160ff1916838001178555610476565b82800160010185558215610476579182015b8281111561047657825182559160200191906001019061045b565b50610482929150610486565b5090565b5b808211156104825760008155600101610487565b634e487b7160e01b600052604160045260246000fd5b600082601f8301126104c257600080fd5b813567ffffffffffffffff808211156104dd576104dd61049b565b604051601f8301601f19908116603f011681019082821181831017156105055761050561049b565b8160405283815286602085880101111561051e57600080fd5b836020870160208301376000602085830101528094505050505092915050565b6000806040838503121561055157600080fd5b823567ffffffffffffffff8082111561056957600080fd5b610575868387016104b1565b9350602085013591508082111561058b57600080fd5b50610598858286016104b1565b9150509250929050565b6000602082840312156105b457600080fd5b813567ffffffffffffffff8111156105cb57600080fd5b6105d7848285016104b1565b949350505050565b60005b838110156105fa5781810151838201526020016105e2565b83811115610609576000848401525b50505050565b600081518084526106278160208601602086016105df565b601f01601f19169290920160200192915050565b60208152600061064e602083018461060f565b9392505050565b60006020828403121561066757600080fd5b81356001600160a01b038116811461064e57600080fd5b600082516106908184602087016105df565b9190910192915050565b6040815260006106ad604083018561060f565b82810360208401526106bf818561060f565b95945050505050565b600181811c908216806106dc57607f821691505b602082108114156106fd57634e487b7160e01b600052602260045260246000fd5b5091905056fea264697066735822122019431a5af9bba6f33200cb49d3e8f44f0337f63155b2485790b9d99d571638dd64736f6c634300080b0033516d5034693669686e56726a385478376354467734615936756e677061505978444a455a3756673152534e53646d"

// DeployUnchained deploys a new Ethereum contract, binding an instance of Unchained to it.
func DeployUnchained(auth *bind.TransactOpts, backend bind.ContractBackend) (common.Address, *types.Transaction, *Unchained, error) {
	parsed, err := abi.JSON(strings.NewReader(UnchainedABI))
	if err != nil {
		return common.Address{}, nil, nil, err
	}

	address, tx, contract, err := bind.DeployContract(auth, parsed, common.FromHex(UnchainedBin), backend)
	if err != nil {
		return common.Address{}, nil, nil, err
	}
	return address, tx, &Unchained{UnchainedCaller: UnchainedCaller{contract: contract}, UnchainedTransactor: UnchainedTransactor{contract: contract}, UnchainedFilterer: UnchainedFilterer{contract: contract}}, nil
}

// Unchained is an auto generated Go binding around an Ethereum contract.
type Unchained struct {
	UnchainedCaller     // Read-only binding to the contract
	UnchainedTransactor // Write-only binding to the contract
	UnchainedFilterer   // Log filterer for contract events
}

// UnchainedCaller is an auto generated read-only Go binding around an Ethereum contract.
type UnchainedCaller struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// UnchainedTransactor is an auto generated write-only Go binding around an Ethereum contract.
type UnchainedTransactor struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// UnchainedFilterer is an auto generated log filtering Go binding around an Ethereum contract events.
type UnchainedFilterer struct {
	contract *bind.BoundContract // Generic contract wrapper for the low level calls
}

// UnchainedSession is an auto generated Go binding around an Ethereum contract,
// with pre-set call and transact options.
type UnchainedSession struct {
	Contract     *Unchained        // Generic contract binding to set the session for
	CallOpts     bind.CallOpts     // Call options to use throughout this session
	TransactOpts bind.TransactOpts // Transaction auth options to use throughout this session
}

// UnchainedCallerSession is an auto generated read-only Go binding around an Ethereum contract,
// with pre-set call options.
type UnchainedCallerSession struct {
	Contract *UnchainedCaller // Generic contract caller binding to set the session for
	CallOpts bind.CallOpts    // Call options to use throughout this session
}

// UnchainedTransactorSession is an auto generated write-only Go binding around an Ethereum contract,
// with pre-set transact options.
type UnchainedTransactorSession struct {
	Contract     *UnchainedTransactor // Generic contract transactor binding to set the session for
	TransactOpts bind.TransactOpts    // Transaction auth options to use throughout this session
}

// UnchainedRaw is an auto generated low-level Go binding around an Ethereum contract.
type UnchainedRaw struct {
	Contract *Unchained // Generic contract binding to access the raw methods on
}

// UnchainedCallerRaw is an auto generated low-level read-only Go binding around an Ethereum contract.
type UnchainedCallerRaw struct {
	Contract *UnchainedCaller // Generic read-only contract binding to access the raw methods on
}

// UnchainedTransactorRaw is an auto generated low-level write-only Go binding around an Ethereum contract.
type UnchainedTransactorRaw struct {
	Contract *UnchainedTransactor // Generic write-only contract binding to access the raw methods on
}

// NewUnchained creates a new instance of Unchained, bound to a specific deployed contract.
func NewUnchained(address common.Address, backend bind.ContractBackend) (*Unchained, error) {
	contract, err := bindUnchained(address, backend, backend, backend)
	if err != nil {
		return nil, err
	}
	return &Unchained{UnchainedCaller: UnchainedCaller{contract: contract}, UnchainedTransactor: UnchainedTransactor{contract: contract}, UnchainedFilterer: UnchainedFilterer{contract: contract}}, nil
}

// NewUnchainedCaller creates a new read-only instance of Unchained, bound to a specific deployed contract.
func NewUnchainedCaller(address common.Address, caller bind.ContractCaller) (*UnchainedCaller, error) {
	contract, err := bindUnchained(address, caller, nil, nil)
	if err != nil {
		return nil, err
	}
	return &UnchainedCaller{contract: contract}, nil
}

// NewUnchainedTransactor creates a new write-only instance of Unchained, bound to a specific deployed contract.
func NewUnchainedTransactor(address common.Address, transactor bind.ContractTransactor) (*UnchainedTransactor, error) {
	contract, err := bindUnchained(address, nil, transactor, nil)
	if err != nil {
		return nil, err
	}
	return &UnchainedTransactor{contract: contract}, nil
}

// NewUnchainedFilterer creates a new log filterer instance of Unchained, bound to a specific deployed contract.
func NewUnchainedFilterer(address common.Address, filterer bind.ContractFilterer) (*UnchainedFilterer, error) {
	contract, err := bindUnchained(address, nil, nil, filterer)
	if err != nil {
		return nil, err
	}
	return &UnchainedFilterer{contract: contract}, nil
}

// bindUnchained binds a generic wrapper to an already deployed contract.
func bindUnchained(address common.Address, caller bind.ContractCaller, transactor bind.ContractTransactor, filterer bind.ContractFilterer) (*bind.BoundContract, error) {
	parsed, err := abi.JSON(strings.NewReader(UnchainedABI))
	if err != nil {
		return nil, err
	}
	return bind.NewBoundContract(address, parsed, caller, transactor, filterer), nil
}

// Call invokes the (constant) contract method with params as input values and
// sets the output to result. The result type might be a single field for simple
// returns, a slice of interfaces for anonymous returns and a struct for named
// returns.
func (_Unchained *UnchainedRaw) Call(opts *bind.CallOpts, result *[]interface{}, method string, params ...interface{}) error {
	return _Unchained.Contract.UnchainedCaller.contract.Call(opts, result, method, params...)
}

// Transfer initiates a plain transaction to move funds to the contract, calling
// its default method if one is available.
func (_Unchained *UnchainedRaw) Transfer(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _Unchained.Contract.UnchainedTransactor.contract.Transfer(opts)
}

// Transact invokes the (paid) contract method with params as input values.
func (_Unchained *UnchainedRaw) Transact(opts *bind.TransactOpts, method string, params ...interface{}) (*types.Transaction, error) {
	return _Unchained.Contract.UnchainedTransactor.contract.Transact(opts, method, params...)
}

// Call invokes the (constant) contract method with params as input values and
// sets the output to result. The result type might be a single field for simple
// returns, a slice of interfaces for anonymous returns and a struct for named
// returns.
func (_Unchained *UnchainedCallerRaw) Call(opts *bind.CallOpts, result *[]interface{}, method string, params ...interface{}) error {
	return _Unchained.Contract.contract.Call(opts, result, method, params...)
}

// Transfer initiates a plain transaction to move funds to the contract, calling
// its default method if one is available.
func (_Unchained *UnchainedTransactorRaw) Transfer(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _Unchained.Contract.contract.Transfer(opts)
}

// Transact invokes the (paid) contract method with params as input values.
func (_Unchained *UnchainedTransactorRaw) Transact(opts *bind.TransactOpts, method string, params ...interface{}) (*types.Transaction, error) {
	return _Unchained.Contract.contract.Transact(opts, method, params...)
}

// ChainNameToHash is a free data retrieval call binding the contract method 0x55a1e706.
//
// Solidity: function chainNameToHash(string ) view returns(string)
func (_Unchained *UnchainedCaller) ChainNameToHash(opts *bind.CallOpts, arg0 string) (string, error) {
	var out []interface{}
	err := _Unchained.contract.Call(opts, &out, "chainNameToHash", arg0)

	if err != nil {
		return *new(string), err
	}

	out0 := *abi.ConvertType(out[0], new(string)).(*string)

	return out0, err

}

// ChainNameToHash is a free data retrieval call binding the contract method 0x55a1e706.
//
// Solidity: function chainNameToHash(string ) view returns(string)
func (_Unchained *UnchainedSession) ChainNameToHash(arg0 string) (string, error) {
	return _Unchained.Contract.ChainNameToHash(&_Unchained.CallOpts, arg0)
}

// ChainNameToHash is a free data retrieval call binding the contract method 0x55a1e706.
//
// Solidity: function chainNameToHash(string ) view returns(string)
func (_Unchained *UnchainedCallerSession) ChainNameToHash(arg0 string) (string, error) {
	return _Unchained.Contract.ChainNameToHash(&_Unchained.CallOpts, arg0)
}

// Owner is a free data retrieval call binding the contract method 0x8da5cb5b.
//
// Solidity: function owner() view returns(address)
func (_Unchained *UnchainedCaller) Owner(opts *bind.CallOpts) (common.Address, error) {
	var out []interface{}
	err := _Unchained.contract.Call(opts, &out, "owner")

	if err != nil {
		return *new(common.Address), err
	}

	out0 := *abi.ConvertType(out[0], new(common.Address)).(*common.Address)

	return out0, err

}

// Owner is a free data retrieval call binding the contract method 0x8da5cb5b.
//
// Solidity: function owner() view returns(address)
func (_Unchained *UnchainedSession) Owner() (common.Address, error) {
	return _Unchained.Contract.Owner(&_Unchained.CallOpts)
}

// Owner is a free data retrieval call binding the contract method 0x8da5cb5b.
//
// Solidity: function owner() view returns(address)
func (_Unchained *UnchainedCallerSession) Owner() (common.Address, error) {
	return _Unchained.Contract.Owner(&_Unchained.CallOpts)
}

// ChangeOwner is a paid mutator transaction binding the contract method 0xa6f9dae1.
//
// Solidity: function changeOwner(address newOwner) returns(address oldOwner)
func (_Unchained *UnchainedTransactor) ChangeOwner(opts *bind.TransactOpts, newOwner common.Address) (*types.Transaction, error) {
	return _Unchained.contract.Transact(opts, "changeOwner", newOwner)
}

// ChangeOwner is a paid mutator transaction binding the contract method 0xa6f9dae1.
//
// Solidity: function changeOwner(address newOwner) returns(address oldOwner)
func (_Unchained *UnchainedSession) ChangeOwner(newOwner common.Address) (*types.Transaction, error) {
	return _Unchained.Contract.ChangeOwner(&_Unchained.TransactOpts, newOwner)
}

// ChangeOwner is a paid mutator transaction binding the contract method 0xa6f9dae1.
//
// Solidity: function changeOwner(address newOwner) returns(address oldOwner)
func (_Unchained *UnchainedTransactorSession) ChangeOwner(newOwner common.Address) (*types.Transaction, error) {
	return _Unchained.Contract.ChangeOwner(&_Unchained.TransactOpts, newOwner)
}

// Donate is a paid mutator transaction binding the contract method 0xed88c68e.
//
// Solidity: function donate() payable returns()
func (_Unchained *UnchainedTransactor) Donate(opts *bind.TransactOpts) (*types.Transaction, error) {
	return _Unchained.contract.Transact(opts, "donate")
}

// Donate is a paid mutator transaction binding the contract method 0xed88c68e.
//
// Solidity: function donate() payable returns()
func (_Unchained *UnchainedSession) Donate() (*types.Transaction, error) {
	return _Unchained.Contract.Donate(&_Unchained.TransactOpts)
}

// Donate is a paid mutator transaction binding the contract method 0xed88c68e.
//
// Solidity: function donate() payable returns()
func (_Unchained *UnchainedTransactorSession) Donate() (*types.Transaction, error) {
	return _Unchained.Contract.Donate(&_Unchained.TransactOpts)
}

// PublishHash is a paid mutator transaction binding the contract method 0x1fee5cd2.
//
// Solidity: function publishHash(string chainName, string hash) returns()
func (_Unchained *UnchainedTransactor) PublishHash(opts *bind.TransactOpts, chainName string, hash string) (*types.Transaction, error) {
	return _Unchained.contract.Transact(opts, "publishHash", chainName, hash)
}

// PublishHash is a paid mutator transaction binding the contract method 0x1fee5cd2.
//
// Solidity: function publishHash(string chainName, string hash) returns()
func (_Unchained *UnchainedSession) PublishHash(chainName string, hash string) (*types.Transaction, error) {
	return _Unchained.Contract.PublishHash(&_Unchained.TransactOpts, chainName, hash)
}

// PublishHash is a paid mutator transaction binding the contract method 0x1fee5cd2.
//
// Solidity: function publishHash(string chainName, string hash) returns()
func (_Unchained *UnchainedTransactorSession) PublishHash(chainName string, hash string) (*types.Transaction, error) {
	return _Unchained.Contract.PublishHash(&_Unchained.TransactOpts, chainName, hash)
}

// UnchainedDonationSentIterator is returned from FilterDonationSent and is used to iterate over the raw logs and unpacked data for DonationSent events raised by the Unchained contract.
type UnchainedDonationSentIterator struct {
	Event *UnchainedDonationSent // Event containing the contract specifics and raw log

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
func (it *UnchainedDonationSentIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(UnchainedDonationSent)
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
		it.Event = new(UnchainedDonationSent)
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
func (it *UnchainedDonationSentIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *UnchainedDonationSentIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// UnchainedDonationSent represents a DonationSent event raised by the Unchained contract.
type UnchainedDonationSent struct {
	From   common.Address
	Amount *big.Int
	Ts     *big.Int
	Raw    types.Log // Blockchain specific contextual infos
}

// FilterDonationSent is a free log retrieval operation binding the contract event 0x8d68ee857f05464080096b8fbaa1603c09866dfccd25bb348209b1188e60add2.
//
// Solidity: event DonationSent(address from, uint256 amount, uint256 ts)
func (_Unchained *UnchainedFilterer) FilterDonationSent(opts *bind.FilterOpts) (*UnchainedDonationSentIterator, error) {

	logs, sub, err := _Unchained.contract.FilterLogs(opts, "DonationSent")
	if err != nil {
		return nil, err
	}
	return &UnchainedDonationSentIterator{contract: _Unchained.contract, event: "DonationSent", logs: logs, sub: sub}, nil
}

// WatchDonationSent is a free log subscription operation binding the contract event 0x8d68ee857f05464080096b8fbaa1603c09866dfccd25bb348209b1188e60add2.
//
// Solidity: event DonationSent(address from, uint256 amount, uint256 ts)
func (_Unchained *UnchainedFilterer) WatchDonationSent(opts *bind.WatchOpts, sink chan<- *UnchainedDonationSent) (event.Subscription, error) {

	logs, sub, err := _Unchained.contract.WatchLogs(opts, "DonationSent")
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(UnchainedDonationSent)
				if err := _Unchained.contract.UnpackLog(event, "DonationSent", log); err != nil {
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

// ParseDonationSent is a log parse operation binding the contract event 0x8d68ee857f05464080096b8fbaa1603c09866dfccd25bb348209b1188e60add2.
//
// Solidity: event DonationSent(address from, uint256 amount, uint256 ts)
func (_Unchained *UnchainedFilterer) ParseDonationSent(log types.Log) (*UnchainedDonationSent, error) {
	event := new(UnchainedDonationSent)
	if err := _Unchained.contract.UnpackLog(event, "DonationSent", log); err != nil {
		return nil, err
	}
	event.Raw = log
	return event, nil
}

// UnchainedHashPublishedIterator is returned from FilterHashPublished and is used to iterate over the raw logs and unpacked data for HashPublished events raised by the Unchained contract.
type UnchainedHashPublishedIterator struct {
	Event *UnchainedHashPublished // Event containing the contract specifics and raw log

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
func (it *UnchainedHashPublishedIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(UnchainedHashPublished)
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
		it.Event = new(UnchainedHashPublished)
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
func (it *UnchainedHashPublishedIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *UnchainedHashPublishedIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// UnchainedHashPublished represents a HashPublished event raised by the Unchained contract.
type UnchainedHashPublished struct {
	ChainName string
	Hash      string
	Raw       types.Log // Blockchain specific contextual infos
}

// FilterHashPublished is a free log retrieval operation binding the contract event 0xc89cce2e1c4073dc6c892fd1d1320b76e81c7e493f153b9a51f264d10ab2234a.
//
// Solidity: event HashPublished(string chainName, string hash)
func (_Unchained *UnchainedFilterer) FilterHashPublished(opts *bind.FilterOpts) (*UnchainedHashPublishedIterator, error) {

	logs, sub, err := _Unchained.contract.FilterLogs(opts, "HashPublished")
	if err != nil {
		return nil, err
	}
	return &UnchainedHashPublishedIterator{contract: _Unchained.contract, event: "HashPublished", logs: logs, sub: sub}, nil
}

// WatchHashPublished is a free log subscription operation binding the contract event 0xc89cce2e1c4073dc6c892fd1d1320b76e81c7e493f153b9a51f264d10ab2234a.
//
// Solidity: event HashPublished(string chainName, string hash)
func (_Unchained *UnchainedFilterer) WatchHashPublished(opts *bind.WatchOpts, sink chan<- *UnchainedHashPublished) (event.Subscription, error) {

	logs, sub, err := _Unchained.contract.WatchLogs(opts, "HashPublished")
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(UnchainedHashPublished)
				if err := _Unchained.contract.UnpackLog(event, "HashPublished", log); err != nil {
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

// ParseHashPublished is a log parse operation binding the contract event 0xc89cce2e1c4073dc6c892fd1d1320b76e81c7e493f153b9a51f264d10ab2234a.
//
// Solidity: event HashPublished(string chainName, string hash)
func (_Unchained *UnchainedFilterer) ParseHashPublished(log types.Log) (*UnchainedHashPublished, error) {
	event := new(UnchainedHashPublished)
	if err := _Unchained.contract.UnpackLog(event, "HashPublished", log); err != nil {
		return nil, err
	}
	event.Raw = log
	return event, nil
}

// UnchainedOwnerChangedIterator is returned from FilterOwnerChanged and is used to iterate over the raw logs and unpacked data for OwnerChanged events raised by the Unchained contract.
type UnchainedOwnerChangedIterator struct {
	Event *UnchainedOwnerChanged // Event containing the contract specifics and raw log

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
func (it *UnchainedOwnerChangedIterator) Next() bool {
	// If the iterator failed, stop iterating
	if it.fail != nil {
		return false
	}
	// If the iterator completed, deliver directly whatever's available
	if it.done {
		select {
		case log := <-it.logs:
			it.Event = new(UnchainedOwnerChanged)
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
		it.Event = new(UnchainedOwnerChanged)
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
func (it *UnchainedOwnerChangedIterator) Error() error {
	return it.fail
}

// Close terminates the iteration process, releasing any pending underlying
// resources.
func (it *UnchainedOwnerChangedIterator) Close() error {
	it.sub.Unsubscribe()
	return nil
}

// UnchainedOwnerChanged represents a OwnerChanged event raised by the Unchained contract.
type UnchainedOwnerChanged struct {
	OldOwner common.Address
	NewOwner common.Address
	Raw      types.Log // Blockchain specific contextual infos
}

// FilterOwnerChanged is a free log retrieval operation binding the contract event 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c.
//
// Solidity: event OwnerChanged(address oldOwner, address newOwner)
func (_Unchained *UnchainedFilterer) FilterOwnerChanged(opts *bind.FilterOpts) (*UnchainedOwnerChangedIterator, error) {

	logs, sub, err := _Unchained.contract.FilterLogs(opts, "OwnerChanged")
	if err != nil {
		return nil, err
	}
	return &UnchainedOwnerChangedIterator{contract: _Unchained.contract, event: "OwnerChanged", logs: logs, sub: sub}, nil
}

// WatchOwnerChanged is a free log subscription operation binding the contract event 0xb532073b38c83145e3e5135377a08bf9aab55bc0fd7c1179cd4fb995d2a5159c.
//
// Solidity: event OwnerChanged(address oldOwner, address newOwner)
func (_Unchained *UnchainedFilterer) WatchOwnerChanged(opts *bind.WatchOpts, sink chan<- *UnchainedOwnerChanged) (event.Subscription, error) {

	logs, sub, err := _Unchained.contract.WatchLogs(opts, "OwnerChanged")
	if err != nil {
		return nil, err
	}
	return event.NewSubscription(func(quit <-chan struct{}) error {
		defer sub.Unsubscribe()
		for {
			select {
			case log := <-logs:
				// New log arrived, parse the event and forward to the user
				event := new(UnchainedOwnerChanged)
				if err := _Unchained.contract.UnpackLog(event, "OwnerChanged", log); err != nil {
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
func (_Unchained *UnchainedFilterer) ParseOwnerChanged(log types.Log) (*UnchainedOwnerChanged, error) {
	event := new(UnchainedOwnerChanged)
	if err := _Unchained.contract.UnpackLog(event, "OwnerChanged", log); err != nil {
		return nil, err
	}
	event.Raw = log
	return event, nil
}
