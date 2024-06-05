package types

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

func TestTraceCache(t *testing.T) {
	expected := &TraceGroup{
		BlockNumber:      17432262,
		TransactionIndex: 44,
		Traces: []Trace{
			{
				Action: &TraceAction{
					CallType: "call",
					From:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
					Gas:      39927,
					Input:    "0x1fee5cd200000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000a7365706f6c69612d747300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002e516d5441623866625233674d6f777876624e466f584339337a6762386f66395567756d3363554b564366414e3172000000000000000000000000000000000000",
					To:       base.HexToAddress("0x0c316b7042b419d07d343f2f4f5bd54ff731183d"),
				},
				BlockHash:   base.HexToHash("0x0bee6d19dab1ce5ddc296a83da21097c902e8d32f0d8c0b6ffad19b9bcffcd67"),
				BlockNumber: 17432262,
				Result: &TraceResult{
					GasUsed: 19058,
				},
				Timestamp:        1686186779,
				TransactionHash:  base.HexToHash("0x7b0dd622b0de6448937d564be16e08fb885895383391b890448cd284ce33f993"),
				TransactionIndex: 44,
				TraceType:        "call",
			},
		},
	}

	store, err := cache.NewStore(&cache.StoreOptions{Location: cache.MemoryCache})
	if err != nil {
		t.Fatal(err)
	}

	if err := store.Write(expected, nil); err != nil {
		t.Fatal(err)
	}

	readBack := &TraceGroup{
		BlockNumber:      17432262,
		TransactionIndex: 44,
	}
	if err := store.Read(readBack, nil); err != nil {
		t.Fatal(err)
	}

	if !reflect.DeepEqual(expected, readBack) {
		t.Fatalf("value mismatch: got %+v want %+v\n", readBack, expected)
	}
}

func TestTraceCacheArticulated(t *testing.T) {
	expected := &TraceGroup{
		BlockNumber:      17432262,
		TransactionIndex: 44,
		Traces: []Trace{
			{
				Action: &TraceAction{
					CallType: "call",
					From:     base.HexToAddress("0xf503017d7baf7fbc0fff7492b751025c6a78179b"),
					Gas:      39927,
					Input:    "0x1fee5cd200000000000000000000000000000000000000000000000000000000000000400000000000000000000000000000000000000000000000000000000000000080000000000000000000000000000000000000000000000000000000000000000a7365706f6c69612d747300000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000002e516d5441623866625233674d6f777876624e466f584339337a6762386f66395567756d3363554b564366414e3172000000000000000000000000000000000000",
					To:       base.HexToAddress("0x0c316b7042b419d07d343f2f4f5bd54ff731183d"),
				},
				ArticulatedTrace: &Function{
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
					Name: "publishHash",
				},
				BlockHash:   base.HexToHash("0x0bee6d19dab1ce5ddc296a83da21097c902e8d32f0d8c0b6ffad19b9bcffcd67"),
				BlockNumber: 17432262,
				Result: &TraceResult{
					GasUsed: 19058,
				},
				Timestamp:        1686186779,
				TransactionHash:  base.HexToHash("0x7b0dd622b0de6448937d564be16e08fb885895383391b890448cd284ce33f993"),
				TransactionIndex: 44,
				TraceType:        "call",
			},
		},
	}

	store, err := cache.NewStore(&cache.StoreOptions{Location: cache.MemoryCache})
	if err != nil {
		t.Fatal(err)
	}

	if err := store.Write(expected, nil); err != nil {
		t.Fatal(err)
	}

	// Read
	readBack := &TraceGroup{
		BlockNumber:      17432262,
		TransactionIndex: 44,
	}
	if err := store.Read(readBack, nil); err != nil {
		t.Fatal(err)
	}

	if !reflect.DeepEqual(expected, readBack) {
		t.Fatalf("value mismatch: got %+v want %+v\n", readBack, expected)
	}
}
