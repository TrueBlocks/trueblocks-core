// Copyright 2021 The TrueBlocks Authors. All rights reserved.
// Use of this source code is governed by a license that can
// be found in the LICENSE file.

package index

import (
	"fmt"
	"strings"
	"testing"

	"github.com/ethereum/go-ethereum/common"
)

func Test_Bloom(t *testing.T) {
	tests := []struct {
		Addr     common.Address
		Parts    [5]string
		Values   [5]uint32
		Bits     [5]uint32
		AddToSet bool
	}{
		{
			Addr:     common.HexToAddress("0x0371a82e4a9d0a4312f3ee2ac9c6958512891372"),
			Parts:    [5]string{"0371a82e", "4a9d0a43", "12f3ee2a", "c9c69585", "12891372"},
			Values:   [5]uint32{57780270, 1251805763, 317976106, 3385234821, 310973298},
			Bits:     [5]uint32{108590, 854595, 257578, 431493, 594802},
			AddToSet: true,
		},
		{
			Addr:     common.HexToAddress("0x3d493c51a916f86d6d1c04824b3a7431e61a3ca3"),
			Parts:    [5]string{"3d493c51", "a916f86d", "6d1c0482", "4b3a7431", "e61a3ca3"},
			Values:   [5]uint32{1028209745, 2836854893, 1830552706, 1262122033, 3860479139},
			Bits:     [5]uint32{605265, 456813, 787586, 685105, 670883},
			AddToSet: true,
		},
		{
			Addr:     common.HexToAddress("0xe1c15164dcfe79431f8421b5a311a829cf0907f3"),
			Parts:    [5]string{"e1c15164", "dcfe7943", "1f8421b5", "a311a829", "cf0907f3"},
			Values:   [5]uint32{3787542884, 3707664707, 528753077, 2735843369, 3473475571},
			Bits:     [5]uint32{86372, 948547, 270773, 108585, 591859},
			AddToSet: true,
		},
		{
			Addr:     common.HexToAddress("0x1296d3bb6dc0efbae431a12939fc15b2823db49b"),
			Parts:    [5]string{"1296d3bb", "6dc0efba", "e431a129", "39fc15b2", "823db49b"},
			Values:   [5]uint32{311874491, 1841360826, 3828457769, 972821938, 2185082011},
			Bits:     [5]uint32{447419, 61370, 106793, 791986, 898203},
			AddToSet: true,
		},
		{
			Addr:     common.HexToAddress("0xd09022c48298f268c2c431dadb9ca4c2534d9c1c"),
			Parts:    [5]string{"d09022c4", "8298f268", "c2c431da", "db9ca4c2", "534d9c1c"},
			Values:   [5]uint32{3499107012, 2191061608, 3267637722, 3684476098, 1397595164},
			Bits:     [5]uint32{8900, 586344, 274906, 828610, 891932},
			AddToSet: true,
		},
		{
			Addr:     common.HexToAddress("0x1296d3bb6dc0efbae431a12939fc15b2823db79b"),
			Parts:    [5]string{"1296d3bb", "6dc0efba", "e431a129", "39fc15b2", "823db79b"},
			Values:   [5]uint32{311874491, 1841360826, 3828457769, 972821938, 2185082779},
			Bits:     [5]uint32{447419, 61370, 106793, 791986, 898971},
			AddToSet: false,
		},
		{
			Addr:     common.HexToAddress("0xd09022c48298f268c2c431dadb9ca4c2534d9c1e"),
			Parts:    [5]string{"d09022c4", "8298f268", "c2c431da", "db9ca4c2", "534d9c1e"},
			Values:   [5]uint32{3499107012, 2191061608, 3267637722, 3684476098, 1397595166},
			Bits:     [5]uint32{8900, 586344, 274906, 828610, 891934},
			AddToSet: false,
		},
		{
			Addr:     common.HexToAddress("0x0341a82e4a9d0a4312f3ee2ac9c6958512891342"),
			Parts:    [5]string{"0341a82e", "4a9d0a43", "12f3ee2a", "c9c69585", "12891342"},
			Values:   [5]uint32{54634542, 1251805763, 317976106, 3385234821, 310973250},
			Bits:     [5]uint32{108590, 854595, 257578, 431493, 594754},
			AddToSet: false,
		},
		{
			Addr:     common.HexToAddress("0x3d493c51a916f86d6d1c04824b3a4431e61a3ca3"),
			Parts:    [5]string{"3d493c51", "a916f86d", "6d1c0482", "4b3a4431", "e61a3ca3"},
			Values:   [5]uint32{1028209745, 2836854893, 1830552706, 1262109745, 3860479139},
			Bits:     [5]uint32{605265, 456813, 787586, 672817, 670883},
			AddToSet: false,
		},
		{
			Addr:     common.HexToAddress("0xe1c15164dcfe49431f8421b5a311a829cf0904f3"),
			Parts:    [5]string{"e1c15164", "dcfe4943", "1f8421b5", "a311a829", "cf0904f3"},
			Values:   [5]uint32{3787542884, 3707652419, 528753077, 2735843369, 3473474803},
			Bits:     [5]uint32{86372, 936259, 270773, 108585, 591091},
			AddToSet: false,
		},
	}

	bloom := ChunkBloom{}
	for _, tt := range tests {
		if tt.AddToSet {
			bloom.AddToSet(tt.Addr)
		}
	}

	expectedLit := []uint64{
		8900, 61370, 86372, 106793, 108585, 108590, 257578,
		270773, 274906, 431493, 447419, 456813, 586344, 591859,
		594802, 605265, 670883, 685105, 787586, 791986, 828610,
		854595, 891932, 898203, 948547,
	}

	nBlooms, nInserted, nBitsLit, nBitsNotLit, sz, bitsLit := bloom.getStats()
	fmt.Println(nBlooms, nInserted, nBitsLit, nBitsNotLit, sz, bitsLit)

	if len(bitsLit) != len(expectedLit) {
		t.Error("mismatched lengths -- expected:", len(expectedLit), "got:", len(bitsLit))

	} else {
		for i, bit := range bitsLit {
			if bit != expectedLit[i] {
				t.Error("mismatched bit lit -- expected:", expectedLit[i], "got:", bitsLit[i])
			}
		}
	}

	for _, tt := range tests {
		if tt.AddToSet && !bloom.IsMemberBytes(tt.Addr) {
			t.Error("address should be member, but isn't", tt.Addr.Hex())

		} else if !tt.AddToSet && bloom.IsMemberBytes(tt.Addr) { // && !tt.FalsePositive {
			t.Error("address should not be member, but is (ignores false positives)", tt.Addr.Hex())
		}

		fmt.Println(strings.ToLower(tt.Addr.Hex()), bloom.IsMemberBytes(tt.Addr))
	}
}

func (bloom *ChunkBloom) getStats() (nBlooms uint64, nInserted uint64, nBitsLit uint64, nBitsNotLit uint64, sz uint64, bitsLit []uint64) {
	bitsLit = []uint64{}
	sz += 4
	nBlooms = uint64(bloom.Count)
	for _, bf := range bloom.Blooms {
		nInserted += uint64(bf.NInserted)
		sz += 4 + uint64(len(bf.Bytes))
		for bitPos := 0; bitPos < len(bf.Bytes)*8; bitPos++ {
			tester := bitChecker{bit: uint32(bitPos), bytes: bf.Bytes}
			if bloom.isBitLit(&tester) {
				nBitsLit++
				bitsLit = append(bitsLit, uint64(bitPos))
			} else {
				nBitsNotLit++
				// fmt.Printf("%d", b)
			}
		}
	}
	return
}

/*
OLD TEST CODE FROM C++ CODE BEFORE MOVING INDEX BUILDING TO GOLANG
#include "etherlib.h"
#include "testing.h"
#include "bloomwrite.h"

extern void getBloomParts(const address_t& addr, CStringArray& parts, CUintArray& values, CUintArray& litBits);
//------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    loadEnvironmentPaths();
    etherlib_init(quickQuitHandler);

    CStringArray addrsInSet{"0x0371a82e4a9d0a4312f3ee2ac9c6958512891372", "0x3d493c51a916f86d6d1c04824b3a7431e61a3ca3",
                            "0xe1c15164dcfe79431f8421b5a311a829cf0907f3", "0x1296d3bb6dc0efbae431a12939fc15b2823db49b",
                            "0xd09022c48298f268c2c431dadb9ca4c2534d9c1c"};

    CStringArray addrsNotInSet{
        "0x1296d3bb6dc0efbae431a12939fc15b2823db79b", "0xd09022c48298f268c2c431dadb9ca4c2534d9c1e",
        "0x0341a82e4a9d0a4312f3ee2ac9c6958512891342", "0x3d493c51a916f86d6d1c04824b3a4431e61a3ca3",
        "0xe1c15164dcfe49431f8421b5a311a829cf0904f3"};

    CStringArray allAddrs;
    for (auto addr : addrsInSet) {
        allAddrs.push_back(addr);
    }
    for (auto addr : addrsNotInSet) {
        allAddrs.push_back(addr);
    }

    // First, we create a bloom filter and add each address in the addrsInSet set to it.
    CBloomFilte rWrite bloomFilter;
    for (auto addr : addrsInSet) {
        bloomFilter.addToSet(addr);
    }

    // Next we show a few statistics about the bloom filter
    cerr << bloomFilter << endl;

    // Next we show each address being processed by the bloom filter algorithm and accumulate the list
    // of all the bits that we expect to be lit in the bloom filter.
    cout << endl;
    for (auto addr : allAddrs) {
        if (addr == "0x1296d3bb6dc0efbae431a12939fc15b2823db79b")
            cout << endl;

        CStringArray parts;
        CUintArray values;
        CUintArray bits;
        getBloomParts(addr, parts, values, bits);
        const char* STR_ADDR_AS_BLOOM =
            "[{ADDR}]\n"
            "\tParts:  [{PARTS}]\n"
            "\tValues: [{VALUES}]\n"
            "\tBits:   [{BITS}]";

        ASSERT(parts.size() == values.size() && parts.size() == bits.size() && parts.size() == 5);
        ostringstream pStream, vStream, bStream;
        for (size_t i = 0; i < parts.size(); i++) {
            pStream << parts[i] << ",";
            vStream << values[i] << ",";
            bStream << bits[i] << ",";
        }
        string_q line = STR_ADDR_AS_BLOOM;
        replace(line, "[{ADDR}]", addr);
        replace(line, "[{PARTS}]", substitute(trim(pStream.str(), ','), ",", ", "));
        replace(line, "[{VALUES}]", substitute(trim(vStream.str(), ','), ",", ", "));
        replace(line, "[{BITS}]", substitute(trim(bStream.str(), ','), ",", ", "));
        cout << line << endl;
    }

    // Next, we test to see if the address we've put into the bloom filter reporting true when queried
    cout << endl;
    for (auto addr : addrsInSet) {
        cout << addr << ": " << bloomFilter.isMemberOf(addr) << endl;
    }

    // Next, we test that those that were not put in the filter report false (even though they could report true)
    for (auto addr : addrsNotInSet) {
        cout << addr << ": " << bloomFilter.isMemberOf(addr) << endl;
    }

    etherlib_cleanup();
    return 0;
}

//---------------------------------------------------------------------------
void getBloomParts(const address_t& addr, CStringArray& fourBytesOut, CUintArray& valuesOut, CUintArray& litBitsOut) {
    if (!isAddress(addr))
        return;

    getLitBits(addr, litBitsOut);

#define BLOOM_K 5
    for (size_t k = 0; k < BLOOM_K; k++) {
        string_q fourByte = extract(addr, 2 + (k * EXTRACT_WID), EXTRACT_WID);
        fourBytesOut.push_back(fourByte);
        uint64_t value = str_2_Uint("0x" + fourByte);
        valuesOut.push_back(value);
    }
}
*/
