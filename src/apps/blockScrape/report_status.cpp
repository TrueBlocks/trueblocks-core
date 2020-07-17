/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//--------------------------------------------------------------------------
inline bool prevLastBlocks(blknum_t& u, blknum_t& r, blknum_t& s, blknum_t& f, blknum_t& c, bool write) {
    string_q fn = configPath("cache/tmp/scraper-status.txt");
    if (write) {
        ostringstream os;
        os << u << "\t" << r << "\t" << s << "\t" << f << "\t" << c << endl;
        stringToAsciiFile(fn, os.str());
    } else {
        string_q contents = asciiFileToString(fn);
        CUintArray parts;
        explode(parts, contents, '\t');
        if (parts.size()) {
            u = parts[0];
            r = parts[1];
            s = parts[2];
            f = parts[3];
            c = parts[4];
        } else {
            u = r = s = f = c = NOPOS;
        }
    }
    return true;
}

//-------------------------------------------------------------------------
inline string_q showLastBlocks(const blknum_t u, const blknum_t r, const blknum_t s, const blknum_t f,
                               const blknum_t c) {
    if (isTestMode())
        return "--final--, --staging--, --ripe--, --unripe--, --client--";

    ostringstream os;
    os << cYellow;
    os << padNum9T((int64_t)f) << ", ";
    os << padNum9T((int64_t)s) << ", ";
    os << padNum9T((int64_t)r) << ", ";
    os << padNum9T((int64_t)u) << ", ";
    os << padNum9T((int64_t)c);
    os << cOff;
    return os.str();
}

//-------------------------------------------------------------------------
string_q scraperStatus(void) {
    char hostname[HOST_NAME_MAX];
    char username[LOGIN_NAME_MAX];

    gethostname(hostname, HOST_NAME_MAX);
    getlogin_r(username, LOGIN_NAME_MAX);

    string_q hostUser = string_q(hostname) + " (" + username + ")";

    uint64_t unripe, ripe, staging, finalized, client;
    uint64_t pUnripe, pRipe, pStaging, pFinalized, pClient;

    getLatestBlocks(unripe, ripe, staging, finalized, client);
    if (fileExists(configPath("cache/tmp/scraper-status.txt"))) {
        prevLastBlocks(pUnripe, pRipe, pStaging, pFinalized, pClient, false);
    } else {
        pUnripe = unripe;
        pRipe = ripe;
        pStaging = staging;
        pFinalized = finalized;
        pClient = client;
    }
    prevLastBlocks(unripe, ripe, staging, finalized, client, true);

    ostringstream heights;
    heights << showLastBlocks(unripe, ripe, staging, finalized, client);

    ostringstream pHeights;
    pHeights << showLastBlocks(pUnripe, pRipe, pStaging, pFinalized, pClient);

    ostringstream distances;
    distances << showLastBlocks(client - unripe, client - ripe, client - staging, client - finalized, client - client);

    ostringstream diffs;
    diffs << showLastBlocks(unripe - pUnripe, ripe - pRipe, staging - pStaging, finalized - pFinalized,
                            client - pClient);

    ostringstream pNeighbors;
    pNeighbors << showLastBlocks(pRipe - pUnripe, pStaging - pRipe, pFinalized - pStaging, 0, 0);

#define showOne(a, b) cYellow << (isTestMode() ? a : b) << cOff

    ostringstream os;
    os << cGreen << "  Client version:     " << showOne("--version--", getVersionFromClient()) << endl;
    os << cGreen << "  Trueblocks version: " << showOne(getVersionStr(), getVersionStr()) << endl;
    os << cGreen << "  RPC provider:       " << showOne("--rpc_provider--", getCurlContext()->baseURL) << endl;
    os << cGreen << "  Cache location:     " << showOne("--cache_dir--", getCachePath("")) << endl;
    os << cGreen << "  Host (user):        " << showOne("--host (user)--", hostUser) << endl;
    os << cGreen << "  Heights:            " << heights.str() << endl;
    os << cGreen << "  Previous Heights:   " << pHeights.str() << endl;
    os << cGreen << "  Distances:          " << distances.str() << endl;
    os << cGreen << "  Diffs:              " << diffs.str() << endl;
    os << cGreen << "  Neighbors:          " << pNeighbors.str() << endl;
    return os.str();
}
