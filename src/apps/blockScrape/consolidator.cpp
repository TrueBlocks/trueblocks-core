/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"
#include "consolidator.h"

//--------------------------------------------------------------------------
CConsolidator::CConsolidator(const CBlockProgress& prog) {
    prevBlock = (prog.staging == NOPOS ? (prog.finalized == NOPOS ? 0 : prog.finalized) : prog.staging);
    tmp_fn = indexFolder_staging + "000000000-temp.txt";
    tmp_stream.open(tmp_fn, ios::out | ios::trunc);
    unripe = prog.unripe;
    ripe = prog.ripe;
    staging = prog.staging;
    finalized = prog.finalized;
    client = prog.client;
}

//--------------------------------------------------------------------------
bool visitCopyRipeToStage(const string_q& path, void* data) {
    if (endsWith(path, '/')) {
        return forEveryFileInFolder(path + "*", visitCopyRipeToStage, data);

    } else {
        blknum_t e_unused;
        timestamp_t ts;
        blknum_t bn = bnFromPath(path, e_unused, ts);

        // If we're not one behind, we have a problem
        CConsolidator* con = reinterpret_cast<CConsolidator*>(data);
        if ((con->prevBlock + 1) != bn) {
            // For some reason, we're missing a file. Quit and try again next time
            LOG_WARN("Current file (", path, ") does not sequentially follow previous file ", con->prevBlock, ".");
            return false;
        }

        ifstream inputStream(path, ios::in);
        if (!inputStream.is_open()) {
            // Something went wrong, try again next time
            LOG_WARN("Could not open input stream ", path);
            return false;
        }

        lockSection();
        con->tmp_stream << inputStream.rdbuf();
        con->tmp_stream.flush();
        inputStream.close();
        ::remove(path.c_str());
        con->prevBlock = bn;
        unlockSection();
    }

    return !shouldQuit();
}

//--------------------------------------------------------------------------
bool appendFile(const string_q& toFile, const string_q& fromFile) {
    ofstream output;
    output.open(toFile, ios::out | ios::app);
    if (!output.is_open())
        return false;

    ifstream input(fromFile, ios::in);
    if (!input.is_open()) {
        output.close();
        return false;
    }

    output << input.rdbuf();
    output.flush();
    input.close();

    return true;
}
