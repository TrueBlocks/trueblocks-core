package types

import (
	"reflect"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/cache"
)

func TestBlockCache(t *testing.T) {
	expected := &LightBlock{
		BlockNumber:   4000001,
		BaseFeePerGas: 0,
		Difficulty:    1097113993909745,
		GasLimit:      6712392,
		GasUsed:       337966,
		Hash:          base.HexToHash("0x79990fd526c4751139a7a3afc7420cde1a1141b1920d2afd411858ecb4926a39"),
		Miner:         base.HexToAddress("0xea674fdde714fd979de3edf0f56aa9716b898ec8"),
		ParentHash:    base.HexToHash("0xb8a3f7f5cfc1748f91a684f20fe89031202cbadcd15078c49b85ec2a57f43853"),
		Timestamp:     1499633571,
		Transactions: []string{
			"0x62974c8152c87e14880c54007260e0d5fe9d182c2cd22c58797735a9ae88370a",
		},
	}
	store, err := cache.NewStore(&cache.StoreOptions{Location: cache.MemoryCache})
	if err != nil {
		t.Fatal(err)
	}

	if err := store.Write(expected); err != nil {
		t.Fatal(err)
	}

	// Read
	readBack := &LightBlock{
		BlockNumber: expected.BlockNumber,
	}
	if err := store.ReadFromStore(readBack); err != nil {
		t.Fatal(err)
	}

	if !reflect.DeepEqual(expected, readBack) {
		t.Fatalf("value mismatch: got %+v want %+v\n", readBack, expected)
	}
}
