package uniq

import (
	"testing"
)

func TestAddressBooleanMap_Insert(t *testing.T) {
	addrMap := make(AddressBooleanMap, 0)
	addrMap.Insert(
		"0xfffd8963efd1fc6a506488495d951d5263988d25",
		18509161,
		132,
	)
	key := "0xfffd8963efd1fc6a506488495d951d5263988d25	018509161	00132"

	if _, ok := addrMap[key]; !ok {
		t.Fatal("key not found")
	}
	if v := addrMap[key]; !v {
		t.Fatal("value is false")
	}
}
