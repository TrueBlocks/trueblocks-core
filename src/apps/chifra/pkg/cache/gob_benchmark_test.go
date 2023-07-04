package cache

import (
	"bufio"
	"bytes"
	"encoding/json"
	"math/big"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

var blockJson = `
{
	"blockNumber": 17620045,
	"hash": "0x5e2384e57550dec6ddef1b99e795275a9d91372e817b71260fe8cb0ace8afa40",
	"parentHash": "0x7a473a2383cb063a994b7f614b49d9a8f7895e3f839f097f6da6a4b4d1cd6576",
	"timestamp": 1688467451
}
`

func initBlock() (block *types.SimpleBlock[types.SimpleTransaction]) {
	block = &types.SimpleBlock[types.SimpleTransaction]{}

	if err := json.Unmarshal([]byte(blockJson), block); err != nil {
		panic(err)
	}

	block.Transactions = []types.SimpleTransaction{
		{
			Hash:                 base.HexToHash("0x5a9d77909e77eee0a768d4160155c60fa77ae5b621e3c6f4fe6c54f509ea8c33"),
			BlockHash:            base.HexToHash("0x51bc754831f33817e755039d90af3b20ea1e21905529ddaa03d7ba9f5fc9e66f"),
			BlockNumber:          4369999,
			TransactionIndex:     0,
			Nonce:                175,
			Timestamp:            0,
			From:                 base.HexToAddress("0x53871c23523453988ebd6524fcb0ea29241ca4d2"),
			To:                   base.HexToAddress("0x8d12a197cb00d4747a1fe03395095ce2a5cc6819"),
			Value:                *big.NewInt(0),
			Gas:                  250000,
			GasPrice:             50000000000,
			MaxFeePerGas:         0,
			MaxPriorityFeePerGas: 0,
			Input:                "0x338b5dea000000000000000000000000f3db5fa2c66b7af3eb0c0b782510816cbe4813b800000000000000000000000000000000000000000000000000000000004c4b40",
			HasToken:             false,
			Receipt: &types.SimpleReceipt{
				ContractAddress:   base.HexToAddress("0x0"),
				GasUsed:           53673,
				EffectiveGasPrice: 50000000000,
				Logs: []types.SimpleLog{
					{
						Address:  base.HexToAddress("0xf3db5fa2c66b7af3eb0c0b782510816cbe4813b8"),
						LogIndex: 0,
						Topics: []base.Hash{
							base.HexToHash("0xddf252ad1be2c89b69c2b068fc378daa952ba7f163c4a11628f55a4df523b3ef"),
							base.HexToHash("0x00000000000000000000000053871c23523453988ebd6524fcb0ea29241ca4d2"),
							base.HexToHash("0x0000000000000000000000008d12a197cb00d4747a1fe03395095ce2a5cc6819"),
						},
						Data: "0x00000000000000000000000000000000000000000000000000000000004c4b40",
						ArticulatedLog: &types.SimpleFunction{
							Name: "SomeFunction",
						},
					},
					{
						Address:  base.HexToAddress("0x8d12a197cb00d4747a1fe03395095ce2a5cc6819"),
						LogIndex: 1,
						Topics: []base.Hash{
							base.HexToHash("0xdcbc1c05240f31ff3ad067ef1ee35ce4997762752e3a095284754544f4c709d7"),
						},
						Data: "0x000000000000000000000000f3db5fa2c66b7af3eb0c0b782510816cbe4813b800000000000000000000000053871c23523453988ebd6524fcb0ea29241ca4d200000000000000000000000000000000000000000000000000000000004c4b4000000000000000000000000000000000000000000000000000000000004c4b40",
						ArticulatedLog: &types.SimpleFunction{
							Name: "SomeFunction",
						},
					},
				},
				Status: 1,
			},
			ArticulatedTx: &types.SimpleFunction{
				Name: "SomeFunction",
			},
		},
	}

	return
}

func BenchmarkGobWrite(b *testing.B) {
	block := initBlock()
	buffer := new(bytes.Buffer)

	b.ResetTimer()
	_, err := block.WriteTo(buffer)
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkGobRead(b *testing.B) {
	writtenBlock := initBlock()
	buffer := new(bytes.Buffer)
	if _, err := writtenBlock.WriteTo(buffer); err != nil {
		b.Fatal(err)
	}

	block := &types.SimpleBlock[types.SimpleTransaction]{}

	b.ResetTimer()
	if _, err := block.ReadFrom(buffer); err != nil {
		b.Fatal(err)
	}
}

func BenchmarkCustomWrite(b *testing.B) {
	block := initBlock()
	buffer := new(bytes.Buffer)
	writer := bufio.NewWriter(buffer)

	b.ResetTimer()
	err := WriteBlock(writer, block)
	if err != nil {
		b.Fatal(err)
	}
}

func BenchmarkCustomRead(b *testing.B) {
	block := initBlock()
	buffer := new(bytes.Buffer)
	writer := bufio.NewWriter(buffer)
	if err := WriteBlock(writer, block); err != nil {
		b.Fatal(err)
	}
	writer.Flush()

	reader := bufio.NewReader(buffer)

	b.ResetTimer()
	if _, err := ReadBlock(reader); err != nil {
		b.Fatal(err)
	}
}
