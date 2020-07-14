package main

import (
	"log"
	"os"
	"testing"
)

func getAddressRecords(t *testing.T, f *os.File, count uint32) (records []addressRecord) {
	for j := uint32(0); j < count; j++ {
		rec, err := getAddressRecord(f)
		if err != nil {
			t.Fatal(err)
		}
		records = append(records, rec)
	}
	return
}

func TestListAddresses(t *testing.T) {
	hash := make(map[int][]byte)
	f, err := os.Open("data/datafile-0.bin")
	if err != nil {
		t.Fatal(err)
	}
	defer f.Close()
	head, err := getHeader(f)
	if err != nil {
		t.Fatal(err)
	}
	addressRecords := getAddressRecords(t, f, head.NumberOafAddresses)
	for num, rec := range addressRecords {
		log.Printf("%d 0x%20x\n", num, rec.Bytes)
		hash[num] = rec.Bytes[:]
	}
	t.Fail()
}

func locateRecord(t *testing.T, addrStr string, expectedPos int) {
	f, err := os.Open("data/datafile-0.bin")
	if err != nil {
		t.Fatal(err)
	}
	defer f.Close()
	head, err := getHeader(f)
	if err != nil {
		t.Fatal(err)
	}
	addr, err := newAddressFromHex(addrStr)
	if err != nil {
		t.Fatal(err)
	}
	pos := findAddress(head.NumberOafAddresses, addr, f)

	if pos != expectedPos {
		t.Fatal("expected : ", expectedPos, "found", pos)
	}

}

func TestBeforeFirstRecord(t *testing.T) {
	addrStr := "0x000000000000000000000000002386f26fc00000"
	locateRecord(t, addrStr, -1)
}
func TestFirstRecord(t *testing.T) {
	addrStr := "0x000000000000000000000000002386f26fc10000"
	locateRecord(t, addrStr, 0)
}

func TestMiddleRecord(t *testing.T) {
	addrStr := "0x7dc3f10c4c714e26a6f1fb2c2da8d65b885e6e2d"
	locateRecord(t, addrStr, 67385)
}

func TestLastRecord(t *testing.T) {
	addrStr := "0xffffffffffffffffffffffffffffffffffffffff"
	locateRecord(t, addrStr, 134771)
}

func TestWrongMiddleRecord(t *testing.T) {
	// "0x75b480b30bcb0c57951837c8db362db72df25caa" in data.bin at 62921
	addrStr := "0x75b480b30bcb0c57951837c8db362db72df25cab" // record not in data.bin
	locateRecord(t, addrStr, -1)
}
