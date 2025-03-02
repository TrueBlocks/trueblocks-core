package types

import (
	"fmt"
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

func TestTransactionCache(t *testing.T) {
	expected := &Transaction{
		BlockHash:   base.HexToHash("0x79990fd526c4751139a7a3afc7420cde1a1141b1920d2afd411858ecb4926a39"),
		BlockNumber: 4000001,
		From:        base.HexToAddress("0xea674fdde714fd979de3edf0f56aa9716b898ec8"),
		Gas:         50000,
		GasPrice:    21000000000,
		GasUsed:     22966,
		Hash:        base.HexToHash("0x62974c8152c87e14880c54007260e0d5fe9d182c2cd22c58797735a9ae88370a"),
		Input:       "0x",
		Nonce:       2377519,
		Receipt: &Receipt{
			ContractAddress:   base.ZeroAddr,
			EffectiveGasPrice: 21000000000,
			GasUsed:           22966,
			Logs: []Log{
				{
					Address:  base.HexToAddress("0x68c4dc84382822d4ebaf4d2fbbafdc5fde80279b"),
					Data:     "0x00000000000000000000000000000000000000000000000000b7c59b8fd3edff",
					LogIndex: 0,
					Topics: []base.Hash{
						base.HexToHash("0x90890809c654f11d6e72a28fa60149770a0d11ec6c92319d6ceb2bb0a4ea1a15"),
						base.HexToHash("0x000000000000000000000000ea674fdde714fd979de3edf0f56aa9716b898ec8"),
						base.HexToHash("0x0000000000000000000000000000000000000000000000000000000000000058"),
					},
				},
			},
		},
		Timestamp:        1499633571,
		To:               base.HexToAddress("0x68c4dc84382822d4ebaf4d2fbbafdc5fde80279b"),
		TransactionIndex: 10,
		Value:            *(base.NewWei(517)),
	}

	store, err := cache.NewStore(&cache.StoreOptions{Location: cache.MemoryCache})
	if err != nil {
		t.Fatal(err)
	}

	if err := store.Write(expected); err != nil {
		t.Fatal(err)
	}

	// Read
	readBack := &Transaction{
		BlockNumber:      expected.BlockNumber,
		TransactionIndex: 10,
	}
	if err := store.Read(readBack); err != nil {
		t.Fatal(err)
	}

	if !reflect.DeepEqual(expected, readBack) {
		msg := fmt.Sprintf("value mismatch:\n\tgot %+v\n\twant %+v\n", readBack, expected)
		t.Fatal(msg)
	}
}

func TestTransactionCacheArticulated(t *testing.T) {
	expected := &Transaction{
		ArticulatedTx: &Function{
			Name: "publishHash",
			Inputs: []Parameter{
				{
					Name:  "chain",
					Value: "sepolia-ts",
				},
				{
					Name:  "hash",
					Value: "QmTAb8fbR3gMowxvbNFoXC93zgb8of9Ugum3cUKVCfAN1r",
				},
			},
		},
		BlockHash:   base.HexToHash("0x0bee6d19dab1ce5ddc296a83da21097c902e8d32f0d8c0b6ffad19b9bcffcd67"),
		BlockNumber: 17432262,
		From:        base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
		Gas:         62607,
		GasPrice:    19846462776,
		GasUsed:     41738,
		Hash:        base.HexToHash("0x7b0dd622b0de6448937d564be16e08fb885895383391b890448cd284ce33f993"),
		Input:       "0x1fee5cd200000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000a7365706f6c69612d747300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002e516d5441623866625233674d6f777876624e466f584339337a6762386f66395567756d3363554b564366414e3172000000000000000000000000000000000000",
		Nonce:       551,
		Receipt: &Receipt{
			ContractAddress:   base.ZeroAddr,
			EffectiveGasPrice: 0,
			GasUsed:           41738,
			Logs: []Log{
				{
					Address:  base.HexToAddress("0x0c316b7042b419d07d343f2f4f5bd54ff731183d"),
					Data:     "0x000000000000000000000000f503017d7baf7fbc0fff7492b751025c6a78179b000000000000000000000000000000000000000000000000000000000000006000000000000000000000000000000000000000000000000000000000000000a0000000000000000000000000000000000000000000000000000000000000000a7365706f6c69612d747300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002e516d5441623866625233674d6f777876624e466f584339337a6762386f66395567756d3363554b564366414e3172000000000000000000000000000000000000",
					LogIndex: 89,
					Topics: []base.Hash{
						base.HexToHash("0x09732eb896ea819849e5f867dd547ed8ba539c5d9b68be96055d96f1c85abaa7"),
					},
				},
			},
			Status: 1,
		},
		Timestamp:        1686186779,
		To:               base.HexToAddress("0x0c316b7042b419d07d343f2f4f5bd54ff731183d"),
		TransactionIndex: 44,
		Value:            *(base.NewWei(0)),
	}

	store, err := cache.NewStore(&cache.StoreOptions{Location: cache.MemoryCache})
	if err != nil {
		t.Fatal(err)
	}

	if err := store.Write(expected); err != nil {
		t.Fatal(err)
	}

	// Read
	readBack := &Transaction{
		BlockNumber:      expected.BlockNumber,
		TransactionIndex: 44,
	}
	if err := store.Read(readBack); err != nil {
		t.Fatal(err)
	}

	if !reflect.DeepEqual(expected, readBack) {
		t.Fatalf("value mismatch: got %+v want %+v\n", readBack, expected)
	}
}
