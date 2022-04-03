/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "etherlib.h"
#include "testing.h"

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
    CBloomFilter bloomFilter;
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

    // const char* STR_GOARRAY =
    //     "{\n"
    //     "    Addr:     common.HexToAddress(\"[{ADDR}]\"),\n"
    //     "    Values:   [5]uint32{[{VALUES}]},\n"
    //     "    Bits:     [5]uint32{[{BITS}]},\n"
    //     "    Member:   [{MEMBER}],\n"
    //     "    FalsePos: [{FALSEPOS}],\n"
    //     "},";

    // C BloomFilter b looms, a ddrBlooms;
    // for (size_t i = 0; i < addrs.size(); i++) {
    //     address_t addr = addrs[i];
    //     CUintArray values;
    //     CUintArray bits;
    //     b loom_t b = add r_2_Bloom2(addr, values, bits);
    //     if (!(i % 2)) {
    //         addA ddrToBloom(b looms, addr);
    //     }
    //     a ddrBlooms.push_back(b);
    // }
    // cout << endl;

    // for (size_t i = 0; i < addrs.size(); i++) {
    //     bool hit = is MemberOf(b looms, a ddrBlooms[i]);
    //     LOG_INFO(addrs[i], "-", hit, ((i % 2) && hit ? "-fp" : ""));
    // }

    // for (auto bb : b looms) {
    //     for (size_t i = 0; i < BLOOM_WIDTH_IN_BYTES; i++) {
    //         std::bitset<8> x(bb.bits[i]);
    //         ostringstream os;
    //         os << x << endl;
    //         if (contains(os.str(), "1")) {
    //             cout << i << ": " << x << endl;
    //         }
    //     }
    // }

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
