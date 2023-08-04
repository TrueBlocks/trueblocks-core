package abi

import (
	"testing"
)

func TestLoadAbiFromFile(t *testing.T) {
	inputFile := "../../../../other/install/abis/known-015/moloch.json"
	abi := NewFunctionSyncMap()
	err := LoadAbiFromJsonFile(inputFile, abi)
	if err != nil {
		t.Fatal(err)
	}

	if count := abi.Count(); count != 69 {
		t.Fatal("item count mismatch:", count)
	}

	sample := abi.Get("0x502d145f")
	if encoding := sample.Encoding; encoding != "0x502d145f" {
		t.Fatal("wrong encoding", encoding)
	}
	if name := sample.Name; name != "_didPass" {
		t.Fatal("wrong name", name)
	}
	if signature := sample.Signature; signature != "_didPass(uint256)" {
		t.Fatal("wrong signature", signature)
	}
	if ftype := sample.FunctionType; ftype != "function" {
		t.Fatal("wrong type", ftype)
	}
	if len(sample.Inputs) != 1 {
		t.Fatal("wrong input count")
	}
	if len(sample.Outputs) != 1 {
		t.Fatal("wrong output count")
	}
	if name := sample.Inputs[0].Name; name != "proposalIndex" {
		t.Fatal("wrong input name", name)
	}
	if name := sample.Outputs[0].Name; name != "didPass" {
		t.Fatal("wrong output name", name)
	}
}
