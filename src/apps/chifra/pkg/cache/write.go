package cache

import (
	"bufio"
	"encoding/binary"
	"encoding/json"
	"math"
	"math/big"
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

type writeBytes = func(data any) error

// createReadFn returns function that reads bytes from `reader` with
// the correct endianness
func createWriteFn(writer *bufio.Writer) writeBytes {
	return func(data any) error {
		return binary.Write(writer, binary.LittleEndian, data)
	}
}

func lowercaseHex(hex string) string {
	return hex[:2] + strings.ToLower(hex[2:])
}

func writeString(writer *bufio.Writer, str *string) (err error) {
	write := createWriteFn(writer)
	err = write(uint64(len(*str)))
	if err != nil {
		return
	}
	err = write([]byte(*str))
	if err != nil {
		return
	}
	return
}

func writeHash(writer *bufio.Writer, hash *base.Hash) (err error) {
	value := lowercaseHex(hash.Hex())
	return writeString(writer, &value)
}

func writeAddress(writer *bufio.Writer, address *base.Address) (err error) {
	value := lowercaseHex(address.Hex())
	if value == "0x0000000000000000000000000000000000000000" {
		value = "0x0"
		return writeString(writer, &value)
	}
	return writeString(writer, &value)
}

func writeBigUint(writer *bufio.Writer, value *big.Int) (err error) {
	write := createWriteFn(writer)
	// check how many blocks of uint64 we need
	length := int(math.Ceil(float64(value.BitLen()) / float64(64)))
	items := make([]byte, length*8)
	// dump value into slice of bytes
	value.FillBytes(items)

	orderedItems := reverseBytes(items)

	// capacity
	err = write(int32(length))
	if err != nil {
		return
	}
	// len
	err = write(int32(length))
	if err != nil {
		return
	}

	if length > 0 {
		err = write(orderedItems)
		if err != nil {
			return
		}
	}

	return
}

func writeBoolAsUint(writer *bufio.Writer, value *bool) (err error) {
	write := createWriteFn(writer)
	uintValue := uint8(0)
	if *value {
		uintValue = uint8(1)
	}
	return write(uintValue)
}

func writeArray[Item ArrayItem](
	writer *bufio.Writer,
	items []Item,
	writeFn func(*bufio.Writer, *Item) (err error),
) (err error) {
	write := createWriteFn(writer)
	itemCount := uint64(len(items))
	err = write(itemCount)
	if err != nil {
		return
	}

	for _, item := range items {
		err = writeFn(writer, &item)
		if err != nil {
			return
		}
	}
	return
}

func writeDefaultHeader(writer *bufio.Writer, className string) (err error) {
	write := createWriteFn(writer)
	err = write(uint64(0))
	if err != nil {
		return
	}
	err = write(uint64(41000))
	if err != nil {
		return
	}
	err = write(uint64(1))
	if err != nil {
		return
	}

	err = writeString(writer, &className)
	if err != nil {
		return
	}
	return
}

func WriteBlock(writer *bufio.Writer, block *types.SimpleBlock[types.SimpleTransaction]) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CBlock")
	if err != nil {
		return
	}

	err = write(block.GasLimit)
	if err != nil {
		return
	}

	err = write(block.GasUsed)
	if err != nil {
		return
	}

	err = writeHash(writer, &block.Hash)
	if err != nil {
		return
	}

	err = write(block.BlockNumber)
	if err != nil {
		return
	}

	err = writeHash(writer, &block.ParentHash)
	if err != nil {
		return
	}

	err = writeAddress(writer, &block.Miner)
	if err != nil {
		return
	}

	err = write(block.Difficulty)
	if err != nil {
		return
	}

	block.UnusedBool = true
	err = write(block.UnusedBool)
	if err != nil {
		return
	}

	err = write(&block.Timestamp)
	if err != nil {
		return
	}

	err = writeBigUint(writer, &block.BaseFeePerGas)
	if err != nil {
		return
	}

	err = writeArray(writer, block.Transactions, WriteTransaction)
	if err != nil {
		return
	}

	return
}

func WriteTransaction(writer *bufio.Writer, tx *types.SimpleTransaction) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CTransaction")
	if err != nil {
		return
	}

	err = writeHash(writer, &tx.Hash)
	if err != nil {
		return
	}

	err = writeHash(writer, &tx.BlockHash)
	if err != nil {
		return
	}

	err = write(&tx.BlockNumber)
	if err != nil {
		return
	}

	err = write(&tx.TransactionIndex)
	if err != nil {
		return
	}

	err = write(&tx.Nonce)
	if err != nil {
		return
	}

	err = write(&tx.Timestamp)
	if err != nil {
		return
	}

	err = writeAddress(writer, &tx.From)
	if err != nil {
		return
	}

	err = writeAddress(writer, &tx.To)
	if err != nil {
		return
	}

	err = writeBigUint(writer, &tx.Value)
	if err != nil {
		return
	}

	var extraValue1 big.Int
	err = writeBigUint(writer, &extraValue1)
	if err != nil {
		return
	}

	var extraValue2 big.Int
	err = writeBigUint(writer, &extraValue2)
	if err != nil {
		return
	}

	err = write(&tx.Gas)
	if err != nil {
		return
	}

	err = write(&tx.GasPrice)
	if err != nil {
		return
	}

	err = write(&tx.MaxFeePerGas)
	if err != nil {
		return
	}

	err = write(&tx.MaxPriorityFeePerGas)
	if err != nil {
		return
	}

	err = writeString(writer, &tx.Input)
	if err != nil {
		return
	}

	err = writeBoolAsUint(writer, &tx.IsError)
	if err != nil {
		return
	}

	err = writeBoolAsUint(writer, &tx.HasToken)
	if err != nil {
		return
	}

	var cachebits uint8
	err = write(&cachebits)
	if err != nil {
		return
	}

	var reserved2 uint8
	err = write(&reserved2)
	if err != nil {
		return
	}

	err = WriteReceipt(writer, tx.Receipt)
	if err != nil {
		return
	}

	err = writeArray(writer, tx.Traces, WriteTrace)
	if err != nil {
		return
	}

	err = WriteFunction(writer, tx.ArticulatedTx)
	if err != nil {
		return
	}

	return
}

func WriteReceipt(writer *bufio.Writer, receipt *types.SimpleReceipt) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CReceipt")
	if err != nil {
		return
	}

	err = writeAddress(writer, &receipt.ContractAddress)
	if err != nil {
		return
	}

	err = write(&receipt.GasUsed)
	if err != nil {
		return
	}

	err = write(&receipt.EffectiveGasPrice)
	if err != nil {
		return
	}

	err = writeArray(writer, receipt.Logs, WriteLog)
	if err != nil {
		return
	}

	err = write(&receipt.Status)
	if err != nil {
		return
	}

	return
}

func WriteLog(writer *bufio.Writer, log *types.SimpleLog) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CLogEntry")
	if err != nil {
		return
	}

	err = writeAddress(writer, &log.Address)
	if err != nil {
		return
	}

	err = write(&log.LogIndex)
	if err != nil {
		return
	}

	err = writeArray(writer, log.Topics, writeHash)
	if err != nil {
		return
	}

	err = writeString(writer, &log.Data)
	if err != nil {
		return
	}

	err = WriteFunction(writer, log.ArticulatedLog)
	if err != nil {
		return
	}

	return
}

func WriteFunction(writer *bufio.Writer, function *types.SimpleFunction) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CFunction")
	if err != nil {
		return
	}

	err = writeString(writer, &function.Name)
	if err != nil {
		return
	}

	err = writeString(writer, &function.FunctionType)
	if err != nil {
		return
	}

	var unused string
	err = writeString(writer, &unused)
	if err != nil {
		return
	}

	err = write(&function.Anonymous)
	if err != nil {
		return
	}

	err = write(&function.Constant)
	if err != nil {
		return
	}

	err = writeString(writer, &function.StateMutability)
	if err != nil {
		return
	}

	err = writeString(writer, &function.Signature)
	if err != nil {
		return
	}

	err = writeString(writer, &function.Encoding)
	if err != nil {
		return
	}

	err = writeArray(writer, function.Inputs, WriteParameter)
	if err != nil {
		return
	}

	err = writeArray(writer, function.Outputs, WriteParameter)
	if err != nil {
		return
	}

	return
}

func WriteParameter(writer *bufio.Writer, param *types.SimpleParameter) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CParameter")
	if err != nil {
		return
	}

	err = writeString(writer, &param.ParameterType)
	if err != nil {
		return
	}

	err = writeString(writer, &param.Name)
	if err != nil {
		return
	}

	err = writeString(writer, &param.StrDefault)
	if err != nil {
		return
	}

	jsonValue, err := json.Marshal(&param.Value)
	if err != nil {
		return
	}
	strValue := string(jsonValue)
	if err = writeString(writer, &strValue); err != nil {
		return
	}

	err = write(&param.Indexed)
	if err != nil {
		return
	}

	err = writeString(writer, &param.InternalType)
	if err != nil {
		return
	}

	err = writeArray(writer, param.Components, WriteParameter)
	if err != nil {
		return
	}

	unused1 := false
	err = write(&unused1)
	if err != nil {
		return
	}

	unused2 := uint64(0)
	err = write(&unused2)
	if err != nil {
		return
	}

	return
}

func WriteTrace(writer *bufio.Writer, trace *types.SimpleTrace) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CTrace")
	if err != nil {
		return
	}

	err = writeHash(writer, &trace.BlockHash)
	if err != nil {
		return
	}

	err = write(&trace.BlockNumber)
	if err != nil {
		return
	}

	err = write(&trace.Subtraces)
	if err != nil {
		return
	}

	// TODO - re-enable this - it won't be easy -- see #2687
	var junk []string
	err = writeArray(writer, junk, writeString)
	if err != nil {
		return
	}

	err = writeHash(writer, &trace.TransactionHash)
	if err != nil {
		return
	}

	err = write(&trace.TransactionIndex)
	if err != nil {
		return
	}

	err = writeString(writer, &trace.TraceType)
	if err != nil {
		return
	}

	err = writeString(writer, &trace.Error)
	if err != nil {
		return
	}

	err = writeTraceAction(writer, trace.Action)
	if err != nil {
		return
	}

	err = writeTraceResult(writer, trace.Result)
	if err != nil {
		return
	}

	err = WriteFunction(writer, trace.ArticulatedTrace)
	if err != nil {
		return
	}

	return
}

func writeTraceAction(writer *bufio.Writer, action *types.SimpleTraceAction) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CTraceAction")
	if err != nil {
		return
	}

	err = writeAddress(writer, &action.SelfDestructed)
	if err != nil {
		return
	}

	err = writeBigUint(writer, &action.Balance)
	if err != nil {
		return
	}

	err = writeString(writer, &action.CallType)
	if err != nil {
		return
	}

	err = writeAddress(writer, &action.From)
	if err != nil {
		return
	}

	err = write(&action.Gas)
	if err != nil {
		return
	}

	err = writeString(writer, &action.Init)
	if err != nil {
		return
	}

	err = writeString(writer, &action.Input)
	if err != nil {
		return
	}

	err = writeAddress(writer, &action.RefundAddress)
	if err != nil {
		return
	}

	err = writeAddress(writer, &action.To)
	if err != nil {
		return
	}

	err = writeBigUint(writer, &action.Value)
	if err != nil {
		return
	}

	return
}

func writeTraceResult(writer *bufio.Writer, result *types.SimpleTraceResult) (err error) {
	write := createWriteFn(writer)
	err = writeDefaultHeader(writer, "CTraceResult")
	if err != nil {
		return
	}

	err = writeAddress(writer, &result.Address)
	if err != nil {
		return
	}

	err = writeString(writer, &result.Code)
	if err != nil {
		return
	}

	err = write(&result.GasUsed)
	if err != nil {
		return
	}

	err = writeString(writer, &result.Output)
	if err != nil {
		return
	}

	return
}

// WriteAbis writes ABI cache (known.bin)
func WriteAbis(writer *bufio.Writer, abis []types.SimpleFunction) (err error) {
	err = writeDefaultHeader(writer, "CAbi")
	if err != nil {
		return
	}

	// This address is always empty
	address := base.Address{}
	if err = writeAddress(writer, &address); err != nil {
		return
	}
	err = writeArray(writer, abis, WriteFunction)
	return
}
