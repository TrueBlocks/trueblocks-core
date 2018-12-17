/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------------
extern bool addrBelongs (blknum_t bn, void *data);
extern bool checkBloom  (blknum_t bn, void *data);
extern bool rewriteBloom(blknum_t bn, void *data);
extern bool statsFunc   (blknum_t bn, void *data);
extern bool rawFunc     (blknum_t bn, void *data);

//-------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("binary", defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.isRaw) {
            options.blocks.forEveryBlockNumber(rawFunc, NULL);

        } else {
            bool hasAddrs  = !options.address_list.empty();
            bool hasBlocks =  options.blocks.hasBlocks();
            ASSERT(hasAddrs || hasBlocks);
            if (hasAddrs && !hasBlocks) {
                cerr << "Address only modes are not implemented. Quitting...";
                exit(0);

            } else if (hasBlocks && !hasAddrs) {
                if (options.isStats) {
                    CVisitor v(options);
                    options.blocks.forEveryBlockNumber(statsFunc, &v);
                    v.report(v.lastMarker+v.options.bucketSize);

                } else if (options.isCheck) {
                    options.blocks.forEveryBlockNumber(checkBloom, &options);
                    if (!verbose)
                        cout << "\n";

                } else {
                    ASSERT(options.isReWrite);
                    options.blocks.forEveryBlockNumber(rewriteBloom, &options);
                }
                exit(0);

            } else {
                ASSERT(hasAddrs && hasBlocks);
                options.blocks.forEveryBlockNumber(addrBelongs, &options);
                exit(0);
            }
        }
    }

    return 0;
}

//-------------------------------------------------------------------------
bool addrBelongs(blknum_t bn, void *data) {

    COptions *options = (COptions *)data;
    uint64_t bitBound = options->bitBound;

    static size_t since_hit=0;
    since_hit++;
    if (verbose)
        cout << "For block: " << bn << "\n";
    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (verbose)
        cout << "\tpath: " << bTeal << path << cOff << "\n";
    if (fileExists(path)) {
        CBloomArray blooms;
        readBloomArray(blooms, path);
        for (uint32_t x = 0 ; x < blooms.size() ; x++) {
            string_q addrs = options->address_list;
            while (!addrs.empty()) {
                string_q addr = nextTokenClear(addrs,'|');
                string_q res;
                if (verbose) {
                    cout << "\tcheck " << cRed << bn << "." << x << cOff << " at ";
                    cout << addr << " (";
                    cout << bTeal << (compareBlooms(makeBloom(addr), blooms[x], res) ? "true" : "false") << cOff;
                    cout << cOff << ")";
                    cout << res << "\n";
                } else {
                    if (isBloomHit(makeBloom(addr), blooms[x])) {
                        cout << addr << "\t" << bn << "\n";
                        since_hit = 0;
                    } else
                        // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
                        cerr << "\t" << bn << string_q((since_hit/bitBound), '.') << "\r";
                }
            }
        }
    } else {
        if (verbose) {
            cerr << "\tNo bloom file at block " << bn << "\n\n";
        } else {
            // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
            cerr << "\t" << bn << string_q((since_hit/bitBound), '.') << "\r";
        }
    }
    cerr.flush();
    cout.flush();
    return true;
}

//-------------------------------------------------------------------------
bool accumAddrs(const CAddressAppearance& item, void *data) {
    if (isZeroAddr(item.addr) || !data)
        return true;
    CAddressArray *array = (CAddressArray*)data;
    array->push_back(item.addr);
    return true;
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction *trans, void *data) {
    // TODO: Use an option here for deep trace
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//-------------------------------------------------------------------------
bool checkBloom(blknum_t bn, void *data) {

    CBlock block;
    getBlock(block, bn);

    CAddressArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (verbose)
        cout << "\tpath: " << bTeal << path << cOff << "\n";

    if (fileExists(path)) {
        CBloomArray blooms;
        readBloomArray(blooms, path);
        for (uint32_t j = 0 ; j < array.size() ; j++) {
            bool hit = false;
            bloom_t hitBloom;
            for (uint32_t x = 0 ; x < blooms.size() && !hit ; x++) {
                if (isBloomHit(makeBloom(array[j]), blooms[x])) {
                    hit = true;
                    hitBloom = blooms[x];
                }
            }
            if (hit)
                cerr << "\tAddress " << array[j] << " in bloom " << bn << (verbose ? "\n" : "                           \r");
            else
                cout << "\tAddress " << array[j] << " not in bloom for block " << bn << "\n";
            if (verbose) {
                string_q res;
                compareBlooms(makeBloom(array[j]), hitBloom, res);
                if (!res.empty())
                    cerr << res << "\n";
            }
        }

    } else {
        if (bn < 46000) {
            cerr << path << " does not exist.\r";
            cerr.flush();
        }
    }

    cerr.flush();
    cout.flush();

    return !shouldQuit();
}

extern string_q formatBloom(const bloom_t& b1, bool bits);
//-------------------------------------------------------------------------
bool rewriteBloom(blknum_t bn, void *data) {

    COptions *options = (COptions *)data;
    uint64_t bitBound = options->bitBound;

    CBlock block;
    getBlock(block, bn);

    CAddressArray array;
    block.forEveryUniqueAddress(accumAddrs, transFilter, &array);

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (verbose)
        cout << "\tpath: " << bTeal << path << cOff << "\n";
    cout << "\nRewritng block " << bn << "\n";
    CBloomArray newBlooms;
    for (uint32_t j = 0 ; j < array.size() ; j++) {
        // SEARCH FOR 'BIT_TWIDDLE_AMT 200'
        addAddrToBloom(array[j], newBlooms, bitBound);
        if (verbose)
            cout << "adding " << array[j] << "\n";
        if (verbose) {
            string_q res;
            compareBlooms(makeBloom(array[j]), newBlooms[0], res);
            cerr << res << "\n";
        }
    }

    if (options->isReWrite) {
        if (!isTestMode())
            writeBloomArray(newBlooms, path);
        cerr << "rewrote " << path << "\n";
    }
    for (uint32_t x = 0 ; x < newBlooms.size() ; x++) {
        cout << cYellow << "\n\tbloom: " << cTeal << x << cYellow << " bits: " << cTeal << bitsTwiddled(newBlooms[x]) << cOff;
        cout << formatBloom(newBlooms[x], false) << "\n";
    }
    cerr.flush();
    cout.flush();

    return !shouldQuit();
}

//-------------------------------------------------------------------------
string_q formatBloom(const bloom_t& b1, bool bits) {
    string_q ret;
    if (bits) {
        string_q s1 = substitute(bloom_2_Bits(b1), "0", ".");
        for (uint32_t i=0;i<16;i++) {
            string_q m1;
            for (uint32_t j=0;j<128;j=j+10) {
                m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
            }
            ret += ("\n\t" + m1);
        }
    } else {
        string_q s1 = substitute(substitute(bloom_2_Bytes(b1), "0x", ""), "0", ".");
        for (uint32_t i=0;i<4;i++) {
            string_q m1;
            for (uint32_t j=0;j<128;j=j+10) {
                m1 += extract(extract(s1, i*128, 128), j, 10) + " ";
            }
            ret += ("\n\t" + m1);
        }
    }
    return ret;
}

//-------------------------------------------------------------------------
bool rawFunc(blknum_t bn, void *data) {
    string_q cmd = "getBloom " + uint_2_Str(bn);
    string_q ret = doCommand(cmd);
    cout << ret << "\n";
    return !shouldQuit();
}
