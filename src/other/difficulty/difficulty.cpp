/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
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

//----------------------------------------------------------------
int main(int argc, char* argv[]) {

    //check number of arguments
    if(argc < 4){
        cout << "\nNot enough arguments. The arguments are <starting block> <finishing block> <step size>\n\n";
    return 1;
    }
    if(argc > 4){
    cout << "\nToo much arguments. Only three arguments allowed. <starting block> <finishing block> <step size>\n\n";
    return 1;
    }

    uint64_t start = strtoull(argv[1], NULL, 0); 
    uint64_t count = strtoull(argv[2], NULL, 0);
    uint64_t skip = strtoull(argv[3], NULL, 0);

    // Initialize the library
    etherlib_init(quickQuitHandler);

    // Visit every block between the first and the most recent
    forEveryBlock(visitBlock, nullptr, start, count, skip);

    // Clean up
    etherlib_cleanup();
    return 0;
}

//----------------------------------------------------------------
// for each block
bool visitBlock(CBlock& block, void* data) {
    // Visit each tranaction and show it seperately

    //showing the blocknumber
    cerr << block.blockNumber << "\r";
    cerr.flush();
    for (auto trans : block.transactions){
        trans.timestamp = block.timestamp;
        if (!isZeroAddr(trans.receipt.contractAddress)) {
            cout << block.blockNumber << " ";
            cout << block.transactions.size() << " ";
            if (!visitTransaction(trans, data))
                return false;
        }
    }
    return true;
}

//----------------------------------------------------------------
// for each transaction in the block
bool visitTransaction(CTransaction& trans, void* data) {
   cout << trans.timestamp << " ";
   cout << trans.receipt.contractAddress << " ";
   cout << getCodeAt(trans.receipt.contractAddress, trans.blockNumber) << endl;
   return true;
}

#if 0
#include "etherlib.h"

#define START 12681280
#define END 12903266

#if 1
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    for (size_t i = START ; i < END; i++) {
        CBlock block;
        getBlock_light(block, i);
        cout << block.blockNumber << "," << block.timestamp << "," << block.difficulty << endl;
        cerr << block.blockNumber << " " << (END - block.blockNumber) << "     \r"; cerr.flush();
    }
    return 0;
}
#endif

//#define byzantiumBlock        4370000 - 3000000 = 1370000 --> period 13
//#define constantinopleBlock   7280000 - 5000000 = 2280000 --> period 22
//#define instanbulBlock        9069000 - 9000000 = 69000
//static blknum_t last = NOPOS;
//bool oneLine(const char* str, void* data) {
//    CStringArray parts;
//    explode(parts, substitute(str, "\n", ""), ',');
//    blknum_t bn = str_2_Uint(parts[0]);
//    uint64_t fake_block = bn;
//    if (bn > instanbulBlock)
//        fake_block = bn - 9000000;
//    else if (bn > constantinopleBlock)
//        fake_block = bn - 5000000;
//    else if (bn > byzantiumBlock)
//        fake_block = bn - 3000000;
////    uint64_t period = (uint64_t)floor(fake_block / 100000);
////    cout << bn << " ---- " << parts[1] << " ---- " << parts[2] << " --- " << fake_block << " --- " << period << endl;
//    last = bn;
//    return true;
//}

//11728437,1611625091,4459297003201165
//jrush@iMac:~/D/t/s/o/difficulty|develop⚡*?➤ whenBlock latest
//blocknumber timestamp   date    name
//11828006

#if 0
int main(int argc, const char* argv[]) {
   // forEveryLineInAsciiFile("/Users/jrush/Development/tokenomics/explorations/difficulty/data/difficulty.csv", oneLine, NULL);
    for (blknum_t bn = 8982001 ; bn < 10000000 ; bn++) { //last+1 ; bn < 10000000 ; bn++) {
        uint64_t fake_block = bn;
        if (bn > instanbulBlock)
            fake_block = bn - 9000000;
        else if (bn > constantinopleBlock)
            fake_block = bn - 5000000;
        else if (bn > byzantiumBlock)
            fake_block = bn - 3000000;
        uint64_t period = (uint64_t)floor(fake_block / 100000);
        if ((bn > instanbulBlock - 200000) && !(bn % 2000))
            cout << bn << " ---- " << 10 << " ---- " << 10 << " --- " << fake_block << " --- " << period << endl;
    }
    return 0;
}
#endif
#if 0
int main(int argc, const char* argv[]) {
    uint32_t *tsMemMap = NULL;
    size_t cnt;
    loadTimestamps(&tsMemMap, cnt);
    for (size_t i=0 ; i < cnt*2 ; i = i+2) {
        cout << (i/2) << "\t" << ((i/2)-tsMemMap[i]) << "\t" << tsMemMap[i] << "\t" << tsMemMap[i+1] << endl;
    }
    return 0;
}
#endif
#endif
