package types

import (
	"reflect"
	"sort"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/v5/pkg/base"
)

func TestSortByAddressAscending(t *testing.T) {
	abis := []Abi{
		{base.HexToAddress("0x2"), 2048, []Function{}, false, false, false, true, "2024-08-01", 10, 5, "AbiTwo", "/path/two", nil},
		{base.HexToAddress("0x3"), 4096, []Function{}, false, false, false, true, "2024-08-03", 12, 3, "AbiThree", "/path/three", nil},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one", nil},
	}

	expected := []Abi{
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one", nil},
		{base.HexToAddress("0x2"), 2048, []Function{}, false, false, false, true, "2024-08-01", 10, 5, "AbiTwo", "/path/two", nil},
		{base.HexToAddress("0x3"), 4096, []Function{}, false, false, false, true, "2024-08-03", 12, 3, "AbiThree", "/path/three", nil},
	}

	sort.Slice(abis, AbiCmp(abis, AbiBy(AbiAddress, Ascending)))

	if !reflect.DeepEqual(abis, expected) {
		t.Errorf("got %v, want %v", abis, expected)
	}
}

func TestSortByFileSizeDescending(t *testing.T) {
	abis := []Abi{
		{base.HexToAddress("0x2"), 2048, []Function{}, false, false, false, true, "2024-08-01", 10, 5, "AbiTwo", "/path/two", nil},
		{base.HexToAddress("0x3"), 4096, []Function{}, false, false, false, true, "2024-08-03", 12, 3, "AbiThree", "/path/three", nil},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one", nil},
	}

	expected := []Abi{
		{base.HexToAddress("0x3"), 4096, []Function{}, false, false, false, true, "2024-08-03", 12, 3, "AbiThree", "/path/three", nil},
		{base.HexToAddress("0x2"), 2048, []Function{}, false, false, false, true, "2024-08-01", 10, 5, "AbiTwo", "/path/two", nil},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one", nil},
	}

	sort.Slice(abis, AbiCmp(abis, AbiBy(AbiFileSize, Descending)))

	if !reflect.DeepEqual(abis, expected) {
		t.Errorf("got %v, want %v", abis, expected)
	}
}

func TestSortByIsKnownDescendingThenByNameAscending(t *testing.T) {
	abis := []Abi{
		{base.HexToAddress("0x2"), 2048, []Function{}, false, false, false, true, "2024-08-01", 10, 5, "AbiTwo", "/path/two", nil},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one", nil},
		{base.HexToAddress("0x3"), 4096, []Function{}, false, false, false, true, "2024-08-03", 12, 3, "AbiThree", "/path/three", nil},
	}

	expected := []Abi{
		{base.HexToAddress("0x3"), 4096, []Function{}, false, false, false, true, "2024-08-03", 12, 3, "AbiThree", "/path/three", nil},
		{base.HexToAddress("0x2"), 2048, []Function{}, false, false, false, true, "2024-08-01", 10, 5, "AbiTwo", "/path/two", nil},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one", nil},
	}

	sort.Slice(abis, AbiCmp(abis, AbiBy(AbiIsKnown, Descending), AbiBy(AbiName, Ascending)))

	if !reflect.DeepEqual(abis, expected) {
		t.Errorf("got %v, want %v", abis, expected)
	}
}
