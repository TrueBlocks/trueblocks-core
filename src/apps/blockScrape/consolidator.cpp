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

        if (bn > FIRST_SNAP_TO_GRID && !(bn % SNAP_TO_GRID_BLKS)) {
            LOG_INDEX3(path, " path");
            LOG3(bYellow, "We want to write the snap-to-grid file here", cOff);

            // Next, we try to create one or more chunks. Creating a chunk means consolidating them (writing
            // them to a binary relational table), and re-write any unfinalized records back onto the stage.
            // Again, if anything goes wrong we need clean up and leave the data in a recoverable state.
            if (!con->stage_chunks()) {
                cleanFolder(indexFolder_unripe);
                cleanFolder(indexFolder_ripe);
                ::remove(con->tmp_fn.c_str());
            }

            // Did user hit control+c?
            if (shouldQuit())
                return true;

            blknum_t nRecords = fileSize(con->newStage) / 59;
            blknum_t chunkSize = min(blknum_t(nRecords), blknum_t(MAX_ROWS));

            LOG_INDEX8(con->tmpFile, " staging completed");
            LOG_INDEX8(con->tmp_fn, " staging completed");
            LOG_INDEX8(con->oldStage, " staging completed");
            LOG_INDEX8(con->newStage, " staging completed not yet consolidated");
            LOG8("nRecords: ", nRecords);
            LOG8("chunkSize: ", chunkSize);
            con->write_chunks(chunkSize, true);

            //            cerr << "WE WROTE THE FILE AND WILL NOW RETURN FALSE. HIT ENTER";
            //            getchar();

            // We act as if we didn't complete scanning, but we have in fact created and written a chunk.
            // Returning false here will cause the caller to clean up the unripe and ripe folders and
            // next time we will start at the next block after the snap
            return false;
        }
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

//--------------------------------------------------------------------------
string_q CConsolidator::Format(void) {
    ostringstream os;
    os << "pin: " << pin << endl;
    os << "distFromHead: " << distFromHead << endl;
    os << "prevBlock: " << prevBlock << endl;
    os << "blazeStart: " << blazeStart << endl;
    os << "blazeRipe: " << blazeRipe << endl;
    os << "blazeCnt: " << blazeCnt << endl;
    os << "oldStage: " << oldStage << endl;
    os << "newStage: " << newStage << endl;
    os << "tmpFile: " << tmpFile << endl;
    os << "tmp_fn: " << tmp_fn << endl;
    return os.str();
}
