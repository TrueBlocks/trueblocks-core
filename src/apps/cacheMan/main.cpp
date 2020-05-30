/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();

    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto monitor : options.monitors) {
            CAppearanceArray items;
            if (!monitor.loadMonitor(items)) {
                LOG_INFO("Could not load monitor for address ", monitor.address);

            } else if (items.size() == 0) {
                LOG_INFO("No transactions in cache for address ", monitor.address);

            } else {
                LOG_INFO("Processing cache for address ", monitor.address);
                CAppearanceArray fixed;
                for (auto item : items) {
                    if (shouldQuit())
                        break;

                    // Assumes a sorted array
                    bool isDup = fixed.size() > 0 && (fixed.back().bn == item.bn && fixed.back().tx == item.tx);
                    if (isDup) {
                        LOG_INFO("Dup removed: ", item.bn, ".", item.tx);
                    } else {
                        fixed.push_back(item);
                    }
                }

                const char* STR_MSG_OKAY = "Cache for address [{ADDRESS}] okay to block [{BLOCK}]";
                const char* STR_MSG_REPAIRED = "Cache for address [{ADDRESS}] repaired to block [{BLOCK}]";
                const char* STR_MSG_INTERRUPT = "Repair of cache for address [{ADDRESS}] interrupted.";
                const char* STR_MSG_READERROR = "Could not open cache for address [{ADDRESS}].";

                // We may have visited blocks for this address that do not contain txs of interest, so don't lost that
                string_q msg = STR_MSG_REPAIRED;
                blknum_t currentLastBlock = monitor.getLastVisitedBlock();
                if (fixed.size() == items.size()) {
                    msg = STR_MSG_OKAY;
                    if (currentLastBlock < fixed.back().bn) {
                        monitor.writeLastBlock(fixed.back().bn);
                        currentLastBlock = fixed.back().bn;
                    }

                } else {
                    string_q fn = getMonitorPath(monitor.address, FM_STAGING);  // do this in staging in case user quits
                    CArchive txCache(WRITING_ARCHIVE);
                    if (!txCache.Lock(fn, modeWriteCreate, LOCK_NOWAIT)) {
                        msg = STR_MSG_READERROR;

                    } else {
                        for (auto item : fixed) {
                            txCache << uint32_t(item.bn) << uint32_t(item.tx);
                            if (shouldQuit()) {
                                msg = STR_MSG_INTERRUPT;
                                continue;
                            }
                        }
                        txCache.Release();
                    }

                    if (msg == STR_MSG_REPAIRED) {
                        monitor.moveToProduction();
                        if (currentLastBlock < fixed.back().bn) {
                            monitor.writeLastBlock(fixed.back().bn);
                            currentLastBlock = fixed.back().bn;
                        }
                    }
                }
                msg = substitute(msg, "[{BLOCK}]", uint_2_Str(currentLastBlock));
                msg = monitor.Format(msg);
                LOG_INFO(msg);
            }
        }
    }

    acctlib_cleanup();

    return 0;
}
