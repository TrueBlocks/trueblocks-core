/*-------------------------------------------------------------------------------------------
 * QuickBlocks - Decentralized, useful, and detailed data from Ethereum blockchains
 * Copyright (c) 2018 Great Hill Corporation (http://quickblocks.io)
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
#include "options.h"

//--------------------------------------------------------------
uint32_t blkSize(SFUintBN num) {
    if (!num.len)
        return 0;
    return sizeof(num.blk[0]);
}

//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    SFAddress addrs[] = { "", "0x", "0x0", "0x0000000000000000000000000000000000000000", "0x1111111111111111111111111111111111111111" };
    size_t nAddrs = sizeof(addrs) / sizeof(SFAddress);
    for (size_t i = 0 ; i < nAddrs ; i++) {
        SFUintBN num = addr2BN(addrs[i]);
        cout << addrs[i] << " : ";
        cout << num << " : ";
        cout << sizeof(num) << " + (" << num.len << " * " << blkSize(num) << "): ";
        cout << sizeof(num) + (num.len * blkSize(num)) << "\n";
    }
    return 0;
}

#if 0
toAddr
toAddrOld
toAddress
toBigNum
toBigNum2
toBigNum3
toBloom
toBool
toBool_in
toBoolean
toBytes
toDouble
toEther
toFloat
toHash
toLong
toLong32u
toLongU
toTopic
toUnsigned
toWei
toAscString
toBlock
toHex
toDate
toDayName
toJson
toJson1
toJsonFldList
toLower
toMonthName
toProper
toRecordID
toSpaces
toSpaces2
toString
toTimeStamp
toTrans
toUpper
to_hex
to_int
to_long
to_short
to_string
to_uint
to_ulong
to_ushort


fromAddress
fromBloom
fromHash
fromTopic
fromUnsigned
fromWei

asBitmap
asDollars
asEther
asHex
asPct
asString
asStringBN
asStringU
#endif
