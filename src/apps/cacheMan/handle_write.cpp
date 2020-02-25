/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
bool COptions::handleWrite(const string_q& outFilename, const CAppearanceArray_base& dataArray,
                           APPEARANCEFILTERFUNC filterFunc) const {
    cerr << "\tWriting...";

    address_t address = substitute(outFilename, ".acct.bin", "");
    blknum_t currentLastItem = str_2_Uint(asciiFileToString(getMonitorLast(address)));

    CArchive txCache(WRITING_ARCHIVE);
    if (!txCache.Lock(outFilename, modeWriteCreate, LOCK_WAIT)) {
        string_q name = outFilename;
        if (isTestMode())
            name = substitute(name, getMonitorPath(""), "$CACHE/");
        return usage("Could not open merge file: " + name + ". Quitting.");
    }

    // Now that we know we can write to the file, we can make a write array. We do this for two reasons: filtering
    // removals and removing duplicates. It's just easier that way.
    blknum_t newLastItem = currentLastItem;
    CAppearanceArray_base writeArray;
    for (size_t i = 0; i < dataArray.size() && !shouldQuit(); i++) {
        // filterFunc (if present) returns true if we should include the record
        if (!filterFunc || (*filterFunc)(((COptions*)this)->removals, dataArray[i])) {  // NOLINT
            if (i == 0 || dataArray[i - 1].blk != dataArray[i].blk ||
                dataArray[i - 1].txid != dataArray[i].txid) {  // removes dups
                if (dataArray[i].blk > currentLastItem)        // update last item
                    newLastItem = dataArray[i].blk;
                writeArray.push_back(dataArray[i]);
                cerr << (!(writeArray.size() % 5000) ? "." : "");
            }
        }
    }

    // Now write the entire array in a single write to the hard drive
    if (!shouldQuit()) {
        lockSection(true);
        txCache.Write(writeArray.data(), sizeof(CAppearance_base), writeArray.size());
        if (!filterFunc) {  // we only write the last block marker if we're not removing records
            CAccountWatch monitor;
            monitor.address = address;
            monitor.writeLastBlock(newLastItem);
        }
        lockSection(false);
        cerr << cYellow << writeArray.size() << cOff << " records written, ";
        cerr << cYellow << (dataArray.size() - writeArray.size()) << cOff << " records "
             << (filterFunc ? "removed" : "ignored") << ".\n";
        string_q dName = outFilename;
        if (isTestMode())
            dName = substitute(dName, getMonitorPath(""), "$CACHE/");
        cerr << "\tWritten to " << cTeal << dName << cOff << "\n";
    } else {
        cerr << "\tRecords not written\n";
    }
    txCache.Release();

    return !shouldQuit();
}
