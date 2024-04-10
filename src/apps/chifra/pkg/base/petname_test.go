package base

import (
	"testing"
)

func Test_Petnames(t *testing.T) {
	type Test struct {
		Address string
		Petname string
	}
	tests := []Test{
		{
			Address: "0xecfd004d02f36cd4d8b4a8c1a9533b6af85cd716",
			Petname: "briefly-easy-labrador",
		},
		{
			Address: "0xed16ce39feef3bd7f5d162045e0f67c0f00046bb",
			Petname: "firmly-noble-cobra",
		},
		{
			Address: "0xfcfc3a5004d678613f0b36a642269a7f371c3f6a",
			Petname: "initially-closing-egret",
		},
		{
			Address: "0xfd191a35157d781373fb411bf9f25290047c5eef",
			Petname: "promptly-fitting-shrimp",
		},
		{
			Address: "0xfe80e9232deaff19baf99869883a4bdf0004e53c",
			Petname: "adequately-ethical-ocelot",
		}}

	for _, test := range tests {
		if AddrToPetname(test.Address, "-") != test.Petname {
			t.Errorf("AddrToPetname(%s, '-') = %s; want %s", test.Address, AddrToPetname(test.Address, "-"), test.Petname)
		}
	}
}
