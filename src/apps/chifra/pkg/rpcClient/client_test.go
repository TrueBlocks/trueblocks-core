package rpcClient

import (
	"testing"
)

var abiString = []byte{
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 32, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 46, 81, 109, 81, 65, 78, 122, 90, 115, 105, 118, 85, 110, 72, 121, 82, 66, 49, 115, 121, 97, 49, 106, 120, 122, 87, 114, 75, 53, 98, 56, 78, 105, 51, 77, 83, 106, 117, 106, 85, 109, 120, 67, 90, 86, 88, 51, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
}

func TestParseAbiString(t *testing.T) {
	value, err := ParseAbiString(
		abiString,
	)

	if err != nil {
		t.Error(err)
	}

	if value != "QmQANzZsivUnHyRB1sya1jxzWrK5b8Ni3MSjujUmxCZVX3" {
		t.Error("Bad value", value)
	}
}
