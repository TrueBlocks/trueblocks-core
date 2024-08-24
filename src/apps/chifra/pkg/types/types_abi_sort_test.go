package types

import (
	"reflect"
	"sort"
	"testing"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
)

func TestSortByAddressAscending(t *testing.T) {
	abis := []Abi{
		{base.HexToAddress("0x2"), 2048, []Function{}, true, false, "2024-08-01", 10, 5, "AbiTwo", "/path/two"},
		{base.HexToAddress("0x3"), 4096, []Function{}, true, false, "2024-08-03", 12, 3, "AbiThree", "/path/three"},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one"},
	}

	expected := []Abi{
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one"},
		{base.HexToAddress("0x2"), 2048, []Function{}, true, false, "2024-08-01", 10, 5, "AbiTwo", "/path/two"},
		{base.HexToAddress("0x3"), 4096, []Function{}, true, false, "2024-08-03", 12, 3, "AbiThree", "/path/three"},
	}

	sort.Slice(abis, Cmp(abis, AbiBy(AbiAddress, Ascending)))

	if !reflect.DeepEqual(abis, expected) {
		t.Errorf("got %v, want %v", abis, expected)
	}
}

func TestSortByFileSizeDescending(t *testing.T) {
	abis := []Abi{
		{base.HexToAddress("0x2"), 2048, []Function{}, true, false, "2024-08-01", 10, 5, "AbiTwo", "/path/two"},
		{base.HexToAddress("0x3"), 4096, []Function{}, true, false, "2024-08-03", 12, 3, "AbiThree", "/path/three"},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one"},
	}

	expected := []Abi{
		{base.HexToAddress("0x3"), 4096, []Function{}, true, false, "2024-08-03", 12, 3, "AbiThree", "/path/three"},
		{base.HexToAddress("0x2"), 2048, []Function{}, true, false, "2024-08-01", 10, 5, "AbiTwo", "/path/two"},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one"},
	}

	sort.Slice(abis, Cmp(abis, AbiBy(AbiFileSize, Descending)))

	if !reflect.DeepEqual(abis, expected) {
		t.Errorf("got %v, want %v", abis, expected)
	}
}

func TestSortByIsKnownDescendingThenByNameAscending(t *testing.T) {
	abis := []Abi{
		{base.HexToAddress("0x2"), 2048, []Function{}, true, false, "2024-08-01", 10, 5, "AbiTwo", "/path/two"},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one"},
		{base.HexToAddress("0x3"), 4096, []Function{}, true, false, "2024-08-03", 12, 3, "AbiThree", "/path/three"},
	}

	expected := []Abi{
		{base.HexToAddress("0x3"), 4096, []Function{}, true, false, "2024-08-03", 12, 3, "AbiThree", "/path/three"},
		{base.HexToAddress("0x2"), 2048, []Function{}, true, false, "2024-08-01", 10, 5, "AbiTwo", "/path/two"},
		{base.HexToAddress("0x1"), 1024, []Function{}, false, false, "2024-08-02", 8, 6, "AbiOne", "/path/one"},
	}

	sort.Slice(abis, Cmp(abis, AbiBy(AbiIsKnown, Descending), AbiBy(AbiName, Ascending)))

	if !reflect.DeepEqual(abis, expected) {
		t.Errorf("got %v, want %v", abis, expected)
	}
}
