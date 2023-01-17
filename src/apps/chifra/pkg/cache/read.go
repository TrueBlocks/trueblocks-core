package cache

import (
	"bufio"
	"encoding/binary"
	"errors"
	"math/big"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

// TODO(cache): I don't think we want to hard code this version value here. We want to read it programmatically
// TODO(cache): from auto-generated code. There is a string called version.LibraryVersion that we can use
// TODO(cache): to calculate this value. We can add a function to the version package.
var minimumCacheVersion = uint64(41000)

func validateHeader(header *cacheHeader) error {
	if header.schema < minimumCacheVersion {
		return errors.New("invalid schema")
	}
	return nil
}

type readBytes = func(data any) error

// createReadFn returns function that reads bytes from `reader` with
// the correct endianness
func createReadFn(reader *bufio.Reader) readBytes {
	return func(data any) error {
		return binary.Read(reader, binary.LittleEndian, data)
	}
}

// This generic type describes what can be array item in cache binary
// files
type ArrayItem interface {
	~string |
		common.Hash |
		common.Address |
		types.SimpleTransaction |
		types.SimpleTrace |
		types.SimpleParameter |
		types.SimpleLog
}

// readFromArray converts binary array into slice of type Item
func readFromArray[Item ArrayItem](
	reader *bufio.Reader,
	target *[]Item,
	readValue func(reader *bufio.Reader) (*Item, error),
) (err error) {
	// first, read item count
	var itemCount uint64 = 0
	read := createReadFn(reader)
	err = read(&itemCount)
	if err != nil {
		return
	}

	// make target large enough
	*target = make([]Item, 0, itemCount)

	// TODO(cache): Just noting. If we knew the records in the array were fixed with (I think we
	// TODO(cache): may be able to know that), we can read and write the entire chunk of memory
	// TODO(cache): in one write (or read). It will almost certainly be faster. I don't think we do
	// TODO(cache): this in C++ code, but I always wanted to.
	// read items
	for i := 0; uint64(i) < itemCount; i++ {
		item, readErr := readValue(reader)
		if readErr != nil {
			err = readErr
			return
		}

		*target = append(*target, *item)
	}

	return
}

// readCString reads cString structure from reader. It has different signature than
// the rest of `read*` functions in this package, to ease reading values into
// other structs' fields
func readCString(reader *bufio.Reader, str *cString) (err error) {
	read := createReadFn(reader)
	err = read(&str.size)
	if err != nil {
		return
	}

	str.content = make([]byte, str.size)
	err = read(str.content)
	if err != nil {
		return
	}
	return
}

func readAddress(reader *bufio.Reader, target *common.Address) (err error) {
	str := &cString{}
	err = readCString(reader, str)
	if err != nil {
		return
	}
	addr := common.HexToAddress(string(str.content))
	*target = addr
	return
}

func readHash(reader *bufio.Reader, target *common.Hash) (err error) {
	str := &cString{}
	err = readCString(reader, str)
	if err != nil {
		return
	}
	hash := common.HexToHash(string(str.content))
	*target = hash
	return
}

func readString(reader *bufio.Reader, target *string) (err error) {
	str := &cString{}
	err = readCString(reader, str)
	if err != nil {
		return
	}
	s := string(str.content)
	*target = s
	return
}

// readBigUint reads biguint from cache file and converts it to Go's big.Int
func readBigUint(reader *bufio.Reader, target *big.Int) (err error) {
	read := createReadFn(reader)
	// read capacity
	var capacity int32
	err = read(&capacity)
	if err != nil {
		return
	}
	// read length
	var length int32
	err = read(&length)
	if err != nil {
		return
	}
	if length == 0 {
		*target = *big.NewInt(0)
		return
	}

	// read items as slice of bytes
	items := make([]byte, length*8)
	err = read(&items)
	if err != nil {
		return
	}
	result := big.NewInt(0)
	// in order to get the correct number, we have to manually reverse bytes
	// see https://github.com/golang/go/issues/40891
	orderedItems := reverseBytes(items)
	// load bytes into big.Int
	result.SetBytes(orderedItems)
	*target = *result
	return
}

// makeArrayItemRead takes a function that reads bytes and changes variable's value
// and turns it into a function returning a value. Such a function can then be provided
// to `readFromArray` to read and convert array items
func makeArrayItemRead[Item ArrayItem](
	reader *bufio.Reader,
	readAndSetValue func(reader *bufio.Reader, target *Item) (err error),
) func(reader *bufio.Reader) (value *Item, err error) {
	return func(reader *bufio.Reader) (value *Item, err error) {
		v := new(Item)
		err = readAndSetValue(reader, v)
		value = v
		return
	}
}

func readUintAsBool(read readBytes, target *bool) (err error) {
	var raw uint8
	err = read(&raw)
	if err != nil {
		return
	}
	*target = raw != 0
	return
}

func readCacheHeader(reader *bufio.Reader, target *cacheHeader) (err error) {
	read := createReadFn(reader)

	err = read(&target.deleted)
	if err != nil {
		return
	}

	err = read(&target.schema)
	if err != nil {
		return
	}

	err = read(&target.showing)
	if err != nil {
		return
	}

	err = readCString(reader, &target.className)
	if err != nil {
		return err
	}

	return
}

// TODO(cache): Move to it's own type specific file
// TODO(cache): (see https://github.com/TrueBlocks/trueblocks-core/pull/2584#discussion_r1031564867)
// TODO(cache): Eventually much of this code will be auto-generated.
// (applies to the rest of "Read[DataType]" functions below as well)
func ReadBlock(reader *bufio.Reader) (block *types.SimpleBlock[types.SimpleTransaction], err error) {
	block = &types.SimpleBlock[types.SimpleTransaction]{}
	read := createReadFn(reader)

	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = read(&block.GasLimit)
	if err != nil {
		return
	}

	err = read(&block.GasUsed)
	if err != nil {
		return
	}

	err = readHash(reader, &block.Hash)
	if err != nil {
		return
	}

	err = read(&block.BlockNumber)
	if err != nil {
		return
	}

	err = readHash(reader, &block.ParentHash)
	if err != nil {
		return
	}

	err = readAddress(reader, &block.Miner)
	if err != nil {
		return
	}

	err = read(&block.Difficulty)
	if err != nil {
		return
	}

	err = read(&block.Finalized)
	if err != nil {
		return
	}

	err = read(&block.Timestamp)
	if err != nil {
		return
	}

	err = readBigUint(reader, &block.BaseFeePerGas)
	if err != nil {
		return
	}

	err = readFromArray(reader, &block.Transactions, ReadTransaction)
	if err != nil {
		return
	}

	return
}

func ReadTransaction(reader *bufio.Reader) (tx *types.SimpleTransaction, err error) {
	tx = &types.SimpleTransaction{}
	read := createReadFn(reader)

	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readHash(reader, &tx.Hash)
	if err != nil {
		return
	}

	err = readHash(reader, &tx.BlockHash)
	if err != nil {
		return
	}

	err = read(&tx.BlockNumber)
	if err != nil {
		return
	}

	err = read(&tx.TransactionIndex)
	if err != nil {
		return
	}

	err = read(&tx.Nonce)
	if err != nil {
		return
	}

	err = read(&tx.Timestamp)
	if err != nil {
		return
	}

	err = readAddress(reader, &tx.From)
	if err != nil {
		return
	}

	err = readAddress(reader, &tx.To)
	if err != nil {
		return
	}

	err = readBigUint(reader, &tx.Value)
	if err != nil {
		return
	}

	err = readBigUint(reader, &tx.ExtraValue1)
	if err != nil {
		return
	}

	err = readBigUint(reader, &tx.ExtraValue2)
	if err != nil {
		return
	}

	err = read(&tx.Gas)
	if err != nil {
		return
	}

	err = read(&tx.GasPrice)
	if err != nil {
		return
	}

	err = read(&tx.MaxFeePerGas)
	if err != nil {
		return
	}

	err = read(&tx.MaxPriorityFeePerGas)
	if err != nil {
		return
	}

	err = readString(reader, &tx.Input)
	if err != nil {
		return
	}

	err = readUintAsBool(read, &tx.IsError)
	if err != nil {
		return
	}

	err = readUintAsBool(read, &tx.HasToken)
	if err != nil {
		return
	}

	err = read(&tx.Cachebits)
	if err != nil {
		return
	}

	err = read(&tx.Reserved2)
	if err != nil {
		return
	}

	receipt, err := ReadReceipt(reader)
	if err != nil {
		return
	}
	tx.Receipt = receipt

	err = readFromArray(reader, &tx.Traces, ReadTrace)
	if err != nil {
		return
	}

	articulatedTx, err := ReadFunction(reader)
	if err != nil {
		return
	}
	tx.ArticulatedTx = articulatedTx

	return
}

func ReadReceipt(reader *bufio.Reader) (receipt *types.SimpleReceipt, err error) {
	receipt = &types.SimpleReceipt{}
	read := createReadFn(reader)

	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readAddress(reader, &receipt.ContractAddress)
	if err != nil {
		return
	}

	err = read(&receipt.GasUsed)
	if err != nil {
		return
	}

	err = read(&receipt.EffectiveGasPrice)
	if err != nil {
		return
	}

	err = readFromArray(reader, &receipt.Logs, ReadLog)
	if err != nil {
		return
	}

	err = read(&receipt.Status)
	if err != nil {
		return
	}

	return
}

func ReadLog(reader *bufio.Reader) (log *types.SimpleLog, err error) {
	log = &types.SimpleLog{}
	read := createReadFn(reader)
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readAddress(reader, &log.Address)
	if err != nil {
		return
	}

	err = read(&log.LogIndex)
	if err != nil {
		return
	}

	err = readFromArray(reader, &log.Topics, makeArrayItemRead(reader, readHash))
	if err != nil {
		return
	}

	err = readString(reader, &log.Data)
	if err != nil {
		return
	}

	articulatedLog, err := ReadFunction(reader)
	if err != nil {
		return
	}
	log.ArticulatedLog = articulatedLog

	return
}

func ReadFunction(reader *bufio.Reader) (function *types.SimpleFunction, err error) {
	function = &types.SimpleFunction{}
	read := createReadFn(reader)
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readString(reader, &function.Name)
	if err != nil {
		return
	}

	err = readString(reader, &function.FunctionType)
	if err != nil {
		return
	}

	err = readString(reader, &function.AbiSource)
	if err != nil {
		return
	}

	err = read(&function.Anonymous)
	if err != nil {
		return
	}

	err = read(&function.Constant)
	if err != nil {
		return
	}

	err = readString(reader, &function.StateMutability)
	if err != nil {
		return
	}

	err = readString(reader, &function.Signature)
	if err != nil {
		return
	}

	err = readString(reader, &function.Encoding)
	if err != nil {
		return
	}

	err = readFromArray(reader, &function.Inputs, ReadParameter)
	if err != nil {
		return
	}

	err = readFromArray(reader, &function.Outputs, ReadParameter)
	if err != nil {
		return
	}

	return
}

func ReadParameter(reader *bufio.Reader) (param *types.SimpleParameter, err error) {
	param = &types.SimpleParameter{}
	read := createReadFn(reader)
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readString(reader, &param.ParameterType)
	if err != nil {
		return
	}

	err = readString(reader, &param.Name)
	if err != nil {
		return
	}

	err = readString(reader, &param.StrDefault)
	if err != nil {
		return
	}

	err = readString(reader, &param.Value)
	if err != nil {
		return
	}

	err = read(&param.Indexed)
	if err != nil {
		return
	}

	err = readString(reader, &param.InternalType)
	if err != nil {
		return
	}

	err = readFromArray(reader, &param.Components, ReadParameter)
	if err != nil {
		return
	}

	err = read(&param.Unused)
	if err != nil {
		return
	}

	err = read(&param.IsFlags)
	if err != nil {
		return
	}

	return
}

func ReadTrace(reader *bufio.Reader) (trace *types.SimpleTrace, err error) {
	trace = &types.SimpleTrace{}
	read := createReadFn(reader)
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readHash(reader, &trace.BlockHash)
	if err != nil {
		return
	}

	err = read(&trace.BlockNumber)
	if err != nil {
		return
	}

	err = read(&trace.Subtraces)
	if err != nil {
		return
	}

	var junk []string
	err = readFromArray(reader, &junk, makeArrayItemRead(reader, readString))
	if err != nil {
		return
	}

	err = readHash(reader, &trace.TransactionHash)
	if err != nil {
		return
	}

	err = read(&trace.TransactionIndex)
	if err != nil {
		return
	}

	err = readString(reader, &trace.Type)
	if err != nil {
		return
	}

	err = readString(reader, &trace.Error)
	if err != nil {
		return
	}

	action, err := readTraceAction(reader)
	if err != nil {
		return
	}
	trace.Action = action

	result, err := readTraceResult(reader)
	if err != nil {
		return
	}
	trace.Result = result

	function, err := ReadFunction(reader)
	if err != nil {
		return
	}
	trace.ArticulatedTrace = function

	return
}

func readTraceAction(reader *bufio.Reader) (action *types.SimpleTraceAction, err error) {
	action = &types.SimpleTraceAction{}
	read := createReadFn(reader)
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readAddress(reader, &action.SelfDestructed)
	if err != nil {
		return
	}

	err = readBigUint(reader, &action.Balance)
	if err != nil {
		return
	}

	err = readString(reader, &action.CallType)
	if err != nil {
		return
	}

	err = readAddress(reader, &action.From)
	if err != nil {
		return
	}

	err = read(&action.Gas)
	if err != nil {
		return
	}

	err = readString(reader, &action.Init)
	if err != nil {
		return
	}

	err = readString(reader, &action.Input)
	if err != nil {
		return
	}

	err = readAddress(reader, &action.RefundAddress)
	if err != nil {
		return
	}

	err = readAddress(reader, &action.To)
	if err != nil {
		return
	}

	err = readBigUint(reader, &action.Value)
	if err != nil {
		return
	}

	return
}

func readTraceResult(reader *bufio.Reader) (result *types.SimpleTraceResult, err error) {
	result = &types.SimpleTraceResult{}
	read := createReadFn(reader)
	header := &cacheHeader{}
	err = readCacheHeader(reader, header)
	if err != nil {
		return
	}
	err = validateHeader(header)
	if err != nil {
		return
	}

	err = readAddress(reader, &result.NewContract)
	if err != nil {
		return
	}

	err = readString(reader, &result.Code)
	if err != nil {
		return
	}

	err = read(&result.GasUsed)
	if err != nil {
		return
	}

	err = readString(reader, &result.Output)
	if err != nil {
		return
	}

	return
}
