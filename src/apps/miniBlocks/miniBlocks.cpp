/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"
#include "reporter.h"

extern bool checkMiniBlocks(CMiniBlock& block, const CMiniTrans *trans, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    cout << bGreen << "Starting miniBlocks...\n" << cOff;
    while (!options.commandList.empty()) {

        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        while (!options.mode.empty()) {

            string_q mode = nextTokenClear(options.mode, '|');
            CVisitor reporter(options.startBlock, min(options.maxBlocks, getLatestBlockFromCache() - options.startBlock));

            if (mode == "check") {
                checkMiniBlocks(options, reporter);

            } else if (mode == "list") {

                {  // creates a stack frame, do not remove
                    uint64_t cnt = 0;
                    CArchive miniBlkCache(READING_ARCHIVE);  // we only ever read the current cache
                    if (miniBlkCache.Lock(miniBlockCache, binaryReadOnly, LOCK_NOWAIT)) {
                        bool done = false;
                        while (!done && !miniBlkCache.Eof()) {
                            CMiniBlock block;

                            // read returns the number of bytes read (sizeof(CMiniBlock) == 32)
                            if (miniBlkCache.Read(&block, sizeof(CMiniBlock), 1) != sizeof(CMiniBlock))
                                done = true;

                            if (!done && !(cnt++ % options.skip)) {
                                if (isTestMode())
                                    cout << block.blockNumber << "\t" << block.firstTrans << "\n";
                                else
                                    cout << (cnt-1) << " : " << block.Format() << "\n";
                                cout.flush();
                            }
                        }
                        miniBlkCache.Release();
                    }
                }

                if (!isTestMode()) {  // only when not testing, otherwise way too many
                    uint64_t cnt = 0;
                    CArchive miniTxCache(READING_ARCHIVE);  // we only ever read the current cache
                    if (miniTxCache.Lock(miniTransCache, binaryReadOnly, LOCK_NOWAIT)) {
                        bool done = false;
                        while (!done && !miniTxCache.Eof()) {
                            CMiniTrans trans;

                            // read returns the number of bytes read (sizeof(CMiniBlock) == 32)
                            if (miniTxCache.Read(&trans, sizeof(CMiniTrans), 1) != sizeof(CMiniTrans))
                                done = true;

                            if (!done && !(cnt++ % (options.skip*10))) {
                                cout << (cnt-1) << " : " << trans.Format() << "\n";
                                cout.flush();
                            }
                        }
                        miniTxCache.Release();
                    }
                }

            } else if (mode == "freshen") {

                string_q openMode = (fileExists(miniBlockCache) ? "a+" : binaryWriteCreate);
                if (!reporter.miniB.Lock(miniBlockCache, openMode, LOCK_WAIT))
                    return usage(reporter.miniB.LockFailure());

                openMode = (fileExists(miniTransCache) ? "a+" : binaryWriteCreate);
                if (!reporter.miniT.Lock(miniTransCache, openMode, LOCK_WAIT))
                    return usage(reporter.miniT.LockFailure());

                if (openMode == "a+")
                    reporter.getLastBlock(options.maxBlocks);
                forEveryNonEmptyBlockOnDisc(buildMiniBlock, &reporter, reporter.firstBlock(), reporter.size());
                reporter.miniB.Release();
                reporter.miniT.Release();
                if (reporter.size())
                    cerr << "\n";
                reporter.finalReport();

                uint64_t oSize = sizeof(CMiniBlock);
                uint64_t fSize = fileSize(miniBlockCache);
                cerr << "  miniBlocks: " << (fSize/oSize) << " fileSize:" << fSize << " itemSize:" << oSize << "\n";

                oSize = sizeof(CMiniTrans);
                fSize = fileSize(miniTransCache);
                cerr << "  miniTrans: " << (fSize/oSize) << " fileSize:" << fSize << " itemSize:" << oSize << "\n";
            }
        }
    }
    cout << bGreen << "...done\n" << cOff;
    return 0;
}

//-----------------------------------------------------------------------------------------------
void CVisitor::getLastBlock(blknum_t maxBlocks) {

    nBlocks = fileSize(miniBlockCache) / sizeof(CMiniBlock);
    if (!miniB.isOpen() || nBlocks == 0) {

        m_firstBlock = 0;
        nTrans = 0;

    } else {

        miniB.Seek((int)sizeof(CMiniBlock) * -1, SEEK_END);
        CMiniBlock mini;
        miniB.Read(&mini, sizeof(CMiniBlock), 1);
        miniB.Seek(0, SEEK_END);
        m_firstBlock = mini.blockNumber + 1;
        nTrans = mini.firstTrans + mini.nTrans;
    }

    m_cnt = min(maxBlocks, (getLatestBlockFromCache() - m_firstBlock));
    if ((m_firstBlock + m_cnt) < 50000) {
        m_cnt = 50000 - m_firstBlock;  // so we get at least this far the first time
    }
}

//-----------------------------------------------------------------------------------------------
bool buildMiniBlock(CBlock& block, void *data) {

    CVisitor *reporter = reinterpret_cast<CVisitor *>(data);

    // Write the mini block
    CMiniBlock mini(&block);
    mini.firstTrans = reporter->nTrans;  // The first transaction for this block
    mini.nTrans = block.transactions.size();  // How many?
    lockSection(true);
    reporter->miniB.Write(&mini, 1, sizeof(CMiniBlock));
    reporter->miniB.flush();
    lockSection(false);
    reporter->sizeB += (uint64_t)sizeof(CMiniBlock);

    // Write the transactions
    for (uint32_t i = 0 ; i < block.transactions.size() && !shouldQuit() ; i++) {
        CMiniTrans mini2(&block.transactions[i]);
        lockSection(true);
        reporter->miniT.Write(&mini2, 1, sizeof(CMiniTrans));
        reporter->miniT.flush();
        lockSection(false);
        reporter->sizeT += (uint64_t)sizeof(CMiniTrans);
    }

    // Keep track of the start of the next block
    // Order matters!
    reporter->nBlocks++;
    reporter->nTrans += block.transactions.size();

    reporter->interumReport(mini);

    return !shouldQuit();
}

//-----------------------------------------------------------------------------------------------
void CVisitor::interumReport(const CMiniBlock& block) const {
    cerr << "  " << cYellow << (block.blockNumber-firstBlock()) << cOff;
    cerr << " of ";
    cerr << cYellow << size() << cOff;
    cerr << " (bn: " << cTeal << block.blockNumber << cOff << ") ";
    cerr << " ("
            << "firstTrans: " << cTeal << block.firstTrans << cOff << " "
            << "for " << cTeal << padNum4T(block.nTrans) << cOff << " txs) "
            << "nBlocks: " << bBlue << nBlocks << cOff << " (" << sizeB << ") "
            << "nTrans: " << bBlue << nTrans << cOff << " (" << sizeT << ")         \r ";
    cout.flush();
}

//-----------------------------------------------------------------------------------------------
void CVisitor::finalReport(void) const {
    double endTime  = qbNow();
    double duration = (endTime - startTime);
    cerr.precision(17);
    cerr
            << "  Found: "
            << cGreen << padNum8T(nTrans) << cOff << " transactions in "
            << cGreen << padNum8T(nBlocks) << cOff << " blocks\n  "
            << cRed << duration << cOff << " seconds. "
            << asYellow(nBlocks/(duration)) << " b/s. (" << sizeB << ") "
            << asYellow(nTrans/(duration))  << " t/s. (" << sizeT << ")"
            << "\n";
    cerr.flush();
}
