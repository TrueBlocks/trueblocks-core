#include "etherlib.h"

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

#if 1
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    for (size_t i = 11390799; i < 11637800; i++) {
        CBlock block;
        getBlock_light(block, i);
        cout << block.blockNumber << "," << block.timestamp << "," << block.difficulty << endl;
        cerr << block.blockNumber << "\r"; cerr.flush();
    }
    return 0;
}
#endif

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
    uint32_t *ts_array = NULL;
    size_t cnt;
    loadTimestampFile(&ts_array, cnt);
    for (size_t i=0 ; i < cnt*2 ; i = i+2) {
        cout << (i/2) << "\t" << ((i/2)-ts_array[i]) << "\t" << ts_array[i] << "\t" << ts_array[i+1] << endl;
    }
    return 0;
}
#endif
