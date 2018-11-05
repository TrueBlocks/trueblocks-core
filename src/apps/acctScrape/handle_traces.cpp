/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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

//-------------------------------------------------------------------------
// TODO: If we wrote code that stored traces for the dDos, they would automatically get picked up. We never write these
// files, so fileExists(getTraceCacheFilename(x,y)) never returns true
//-------------------------------------------------------------------------
string_q getTraceCacheFilename(blknum_t bn, const hash_t& hashIn) {
    string_q ret = substitute(substitute(getBinaryFilename(bn), "/blocks/", "/traces/"), ".bin", "_");
    return ret + hashIn + ".bin";
}

//-------------------------------------------------------------------------
string_q queryRawTrace2(const string_q& hashIn) {
    string_q ret = "[" + callRPC("trace_transaction", "[\"" + str_2_Hash(hashIn) +"\"]", true) + "]";
    CRPCResult generic;
    generic.parseJson3(ret);
    return generic.result;
}

//-------------------------------------------------------------------------
bool readFromCache(blknum_t bn, CTraceArray& traces, const hash_t& hashIn) {

    string_q fileName = getTraceCacheFilename(bn, hashIn);
    if (!fileExists(fileName))
        return false;

    CArchive traceCache(READING_ARCHIVE);
    if (traceCache.Lock(fileName, binaryReadOnly, LOCK_NOWAIT)) {
        traceCache >> traces;
        traceCache.Release();
        return true;
    }

    return false;
}

//--------------------------------------------------------------
void getTraces2(blknum_t bn, CTraceArray& traces, const hash_t& hashIn) {

    traces.clear();
    if (readFromCache(bn, traces, hashIn))
        return;

    string_q str = queryRawTrace2(hashIn);
    CTrace trace;
    while (trace.parseJson3(str)) {
        traces.push_back(trace);
        trace = CTrace();  // reset
    }
}
