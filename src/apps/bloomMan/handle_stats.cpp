/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-------------------------------------------------------------------------
bool handle_stats(blknum_t bn, void *data) {

    CStatistics *stats = (CStatistics*)&((COptions*)data)->stats;

    string_q path = substitute(getBinaryFilename(bn), "/blocks/", "/blooms/");
    if (fileExists(path)) {
        CBloomArray blooms;
        readBloomArray(blooms, path);
        stats->nFiles  += 1;
        stats->nBlooms += blooms.size();
        stats->nBytes  += fileSize(path);
        for (auto bloom : blooms)
            stats->nBits += bitsTwiddled(bloom);

    } else {
        stats->nEmpty += 1;

    }
    stats->nVisits++;

    if (stats->curBucket == NOPOS)
        stats->curBucket = bn;
    blknum_t bucket = max(stats->curBucket, ((bn+1) / stats->options->bucketSize) * stats->options->bucketSize);
    if (bucket != stats->curBucket) {
        cout << stats->report(bucket) << endl;
        if (!stats->cummulative)
            stats->Reset();
        stats->curBucket = bucket;
        stats->lastReport = bn;
    } else if (!isTestMode() && !(bn%13)) {
        cerr << stats->interumReport(bn) << "\r"; cerr.flush();
    }

    return !shouldQuit();
}

//-------------------------------------------------------------------------
string_q CStatistics::report(blknum_t bn) const {
    ostringstream os;
    os << cRed;
    static bool been_here = false;
    if (!been_here) {
        been_here = true;
        os << string_q(80, ' ') << "\r";
        os << "bn"      << "\t";
        os << "nVisits" << "\t";
        os << "nFiles"  << "\t";
        os << "nEmpty"  << "\t";
        os << "nBlooms" << "\t";
        os << "nBytes"  << "\t";
        os << "nBits"   << "\n";
    }
    return os.str() + interumReport(bn) + cOff;
}

//-------------------------------------------------------------------------
string_q CStatistics::interumReport(blknum_t bn) const {
    ostringstream os;
    os << bn << "\t";
    os << nVisits << "\t";
    os << nFiles << "\t";
    os << nEmpty << "\t";
    os << nBlooms << "\t";
    os << nBytes << "\t";
    os << nBits << cOff;
    return os.str();
}
