package cache

import (
	"bufio"
	"bytes"
	"encoding/binary"
	"math/big"
	"os"
	"strings"
	"testing"
	"time"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
	"github.com/ethereum/go-ethereum/common"
)

func helperCheckTransaction(t *testing.T, tr *types.SimpleTransaction, expected *types.SimpleTransaction) (err error) {
	if tr.Hash.String() != expected.Hash.String() {
		t.Fatal("mismatched transaction hash", tr.Hash.String())
	}
	if tr.BlockHash.String() != expected.BlockHash.String() {
		t.Fatal("mismatched blockHash")
	}
	if tr.BlockNumber != expected.BlockNumber {
		t.Fatal("mismatched blockNumber")
	}
	if tr.TransactionIndex != expected.TransactionIndex {
		t.Fatal("mismatched transactionIndex")
	}
	if tr.Nonce != expected.Nonce {
		t.Fatal("mismatched nonce")
	}
	if tr.Timestamp != expected.Timestamp {
		t.Fatal("mismatched timestamp", tr.Timestamp, expected.Timestamp)
	}
	if tr.From.String() != expected.From.String() {
		t.Fatal("mismatched from")
	}
	if tr.To.String() != expected.To.String() {
		t.Fatal("mismatched to")
	}
	if tr.Value.Cmp(&expected.Value) != 0 {
		t.Fatal("mismatched value")
	}
	if tr.Gas != expected.Gas {
		t.Fatal("mismatched gas")
	}
	if tr.GasPrice != expected.GasPrice {
		t.Fatal("mismatched gasPrice")
	}
	if tr.MaxFeePerGas != expected.MaxFeePerGas {
		t.Fatal("mismatched maxFeePerGas")
	}
	if tr.MaxPriorityFeePerGas != expected.MaxPriorityFeePerGas {
		t.Fatal("mismatched maxPriorityFeePerGas")
	}
	if tr.Input != expected.Input {
		t.Fatal("mismatched input")
	}
	if tr.HasToken != expected.HasToken {
		t.Fatal("mismatched hasToken")
	}

	receipt := tr.Receipt
	if receipt.ContractAddress.String() != expected.Receipt.ContractAddress.String() {
		t.Fatal("mismatched contractAddress")
	}
	if receipt.GasUsed != expected.Receipt.GasUsed {
		t.Fatal("mismatched gasUsed")
	}
	if receipt.EffectiveGasPrice != expected.Receipt.EffectiveGasPrice {
		t.Fatal("mismatched effectiveGasPrice")
	}
	if receipt.Status != expected.Receipt.Status {
		t.Fatal("mismatched status")
	}

	logs := receipt.Logs
	for logIndex, log := range logs {
		if log.Address.String() != expected.Receipt.Logs[logIndex].Address.String() {
			t.Fatal("mismatched address")
		}
		if log.LogIndex != expected.Receipt.Logs[logIndex].LogIndex {
			t.Fatal("mismatched logIndex")
		}
		for topicIndex, topic := range log.Topics {
			if topic != expected.Receipt.Logs[logIndex].Topics[topicIndex] {
				t.Fatal("mismatched topic")
			}
		}

		if log.Data != expected.Receipt.Logs[logIndex].Data {
			t.Fatal("mismatched data")
		}
	}

	return
}

func TestReadBlock(t *testing.T) {
	f, err := os.Open("./cache_block.bin")
	defer f.Close()
	buf := bufio.NewReader(f)
	if err != nil {
		t.Fatal("cannot open file")
	}
	block, err := ReadBlock(buf)
	if err != nil {
		t.Fatal(err)
	}

	if block.GasLimit != 6712355 {
		t.Fatal("invalid gasLimit")
	}
	if block.GasUsed != 6630311 {
		t.Fatal("invalid gasUsed")

	}
	if block.Hash.String() != "0x51bc754831f33817e755039d90af3b20ea1e21905529ddaa03d7ba9f5fc9e66f" {
		t.Fatal("invalid hash")

	}
	if block.BlockNumber != 4369999 {
		t.Fatal("invalid blockNumber")

	}
	if block.ParentHash.String() != "0x0c8afc74a5e5c7d19f8dd5f2ca13098f871e5b6fd8a6e237df111f935d777105" {
		t.Fatal("invalid parentHash")

	}
	if strings.ToLower(block.Miner.String()) != "0xea674fdde714fd979de3edf0f56aa9716b898ec8" {
		t.Fatal("invalid miner")

	}
	if block.Difficulty != 2997274096101735 {
		t.Fatal("invalid difficulty")

	}
	if block.Finalized != true {
		t.Fatal("invalid finalized")

	}
	if block.Timestamp.Unix() != 1508131303 {
		t.Fatal("invalid timestamp")

	}
	if block.BaseFeePerGas.Cmp(big.NewInt(0)) != 0 {
		t.Fatal("invalid baseFeePerGas")
	}

	helperCheckTransaction(t, &block.Transactions[0], &types.SimpleTransaction{
		Hash:                 common.HexToHash("0x5a9d77909e77eee0a768d4160155c60fa77ae5b621e3c6f4fe6c54f509ea8c33"),
		BlockHash:            common.HexToHash("0x51bc754831f33817e755039d90af3b20ea1e21905529ddaa03d7ba9f5fc9e66f"),
		BlockNumber:          4369999,
		TransactionIndex:     0,
		Nonce:                175,
		Timestamp:            time.Unix(0, 0),
		From:                 common.HexToAddress("0x53871c23523453988ebd6524fcb0ea29241ca4d2"),
		To:                   common.HexToAddress("0x8d12a197cb00d4747a1fe03395095ce2a5cc6819"),
		Value:                *big.NewInt(0),
		Gas:                  250000,
		GasPrice:             50000000000,
		MaxFeePerGas:         0,
		MaxPriorityFeePerGas: 0,
		Input:                "0x338b5dea000000000000000000000000f3db5fa2c66b7af3eb0c0b782510816cbe4813b800000000000000000000000000000000000000000000000000000000004c4b40",
		HasToken:             false,
		Receipt: &types.SimpleReceipt{
			ContractAddress:   common.HexToAddress("0x0"),
			GasUsed:           53673,
			EffectiveGasPrice: 50000000000,
			Logs: []types.SimpleLog{
				{
					Address:  common.HexToAddress("0xf3db5fa2c66b7af3eb0c0b782510816cbe4813b8"),
					LogIndex: 0,
					Topics: []common.Hash{
						common.HexToHash("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef"),
						common.HexToHash("0x00000000000000000000000053871c23523453988ebd6524fcb0ea29241ca4d2"),
						common.HexToHash("0x0000000000000000000000008d12a197cb00d4747a1fe03395095ce2a5cc6819"),
					},
					Data: "0x00000000000000000000000000000000000000000000000000000000004c4b40",
				},
				{
					Address:  common.HexToAddress("0x8d12a197cb00d4747a1fe03395095ce2a5cc6819"),
					LogIndex: 1,
					Topics: []common.Hash{
						common.HexToHash("0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7"),
					},
					Data: "0x000000000000000000000000f3db5fa2c66b7af3eb0c0b782510816cbe4813b800000000000000000000000053871c23523453988ebd6524fcb0ea29241ca4d200000000000000000000000000000000000000000000000000000000004c4b4000000000000000000000000000000000000000000000000000000000004c4b40",
				},
			},
			Status: 1,
		},
	})

	expectedValue := big.NewInt(0)
	expectedValue.UnmarshalText([]byte("921920000000000"))
	helperCheckTransaction(t, &block.Transactions[8], &types.SimpleTransaction{
		Hash:                 common.HexToHash("0x5589ddfd9db108fc6be96c68df9fdcc89a0673fde85fb4089653334bb8c1fc71"),
		BlockHash:            common.HexToHash("0x51bc754831f33817e755039d90af3b20ea1e21905529ddaa03d7ba9f5fc9e66f"),
		BlockNumber:          4369999,
		TransactionIndex:     8,
		Nonce:                254,
		Timestamp:            time.Unix(0, 0),
		From:                 common.HexToAddress("0x1366a2ca67594ffd5174d0216d60d9ea8deb511f"),
		To:                   common.HexToAddress("0xb4a68dfdfb56184930c3a84e9244919823c3a2b4"),
		Value:                *expectedValue,
		Gas:                  21000,
		GasPrice:             20000000000,
		MaxFeePerGas:         0,
		MaxPriorityFeePerGas: 0,
		Input:                "0x",
		HasToken:             false,
		Receipt: &types.SimpleReceipt{
			ContractAddress:   common.HexToAddress("0x0"),
			GasUsed:           21000,
			EffectiveGasPrice: 20000000000,
			Status:            1,
		},
	})
}

func Test_readCString(t *testing.T) {
	source := bytes.NewBuffer([]byte{})
	err := binary.Write(source, binary.LittleEndian, uint64(6))
	if err != nil {
		t.Fatal("while preparing test data:", err)
	}
	err = binary.Write(source, binary.LittleEndian, []byte("CBlock"))
	if err != nil {
		t.Fatal("while preparing test data:", err)
	}

	result := cString{}
	buf := bufio.NewReader(source)
	err = readCString(buf, &result)
	if err != nil {
		t.Fatal(err)
	}
	if result.size != 6 {
		t.Fatal("wrong size:", result.size)
	}
	if string(result.content) != "CBlock" {
		t.Fatal("wrong content:", result.content)
	}
}
