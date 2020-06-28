/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "options.h"

//-------------------------------------------------------------------------
extern const char* STR_MSG_OKAY;
extern const char* STR_MSG_REPAIRED;
extern const char* STR_MSG_INTERRUPT;
extern const char* STR_MSG_READERROR;

//-------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    nodeNotRequired();
    namesNotRequired();
    acctlib_init(defaultQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        for (auto monitor : options.monitors) {
            CAppearanceArray items;
            if (!monitor.loadAndSort(items)) {
                LOG4("Could not load monitor for address ", monitor.address);

            } else {
                // LOG_INFO("Processing cache for address ", monitor.address);
                CAppearanceArray fixed;
                for (auto item : items) {
                    if (shouldQuit())
                        break;

                    // Assumes sorted array
                    bool isDup = fixed.size() > 0 && (fixed.back().bn == item.bn && fixed.back().tx == item.tx);
                    if (isDup) {
                        LOG_INFO("Dup removed: ", item.bn, ".", item.tx);
                    } else {
                        fixed.push_back(item);
                    }
                }

                // lastVisited stores the last checked block for this address (so we can start on the next
                // block. This may be much later than the last block with a transaction of interest.
                blknum_t lastVisited = monitor.getLastVisitedBlock();

                string_q msg = STR_MSG_REPAIRED;
                if (fixed.size() == items.size()) {
                    msg = STR_MSG_OKAY;
                    if (lastVisited < fixed.back().bn) {
                        // Update the last visited block if later than we think it is (probably won't ever happen)
                        lastVisited = fixed.back().bn;
                        monitor.writeLastBlock(lastVisited);
                    }

                } else {
                    // We need to repair the file. Do this in staging in case user quits on us...
                    monitor.fm_mode = FM_STAGING;
                    string_q fn = getMonitorPath(monitor.address, FM_STAGING);
                    CArchive txCache(WRITING_ARCHIVE);
                    if (!txCache.Lock(fn, modeWriteCreate, LOCK_NOWAIT)) {
                        msg = STR_MSG_READERROR;

                    } else {
                        for (auto fix : fixed) {
                            lockSection(true);
                            txCache << uint32_t(fix.bn) << uint32_t(fix.tx);
                            lockSection(false);
                            if (shouldQuit()) {
                                msg = STR_MSG_INTERRUPT;
                                continue;
                            }
                        }
                        txCache.Release();
                    }

                    if (msg == STR_MSG_REPAIRED) {
                        monitor.moveToProduction();
                        if (lastVisited < fixed.back().bn) {
                            // If we're repaird the file, to be cautious, we start the next search at the
                            // latest block in the file.
                            lastVisited = fixed.back().bn;
                            monitor.writeLastBlock(lastVisited);
                        }
                    }
                }
                msg = substitute(msg, "[{BLOCK}]", (isTestMode() ? "Block" : uint_2_Str(lastVisited)));
                msg = monitor.Format(msg);
                // TODO(tjayrush): Can we safely remove msg?
                // LOG_INFO(msg);
            }
        }
    }

    acctlib_cleanup();

    return 0;
}

//-------------------------------------------------------------------------
const char* STR_MSG_OKAY = "Cache for address [{ADDRESS}] okay to block [{BLOCK}]";
const char* STR_MSG_REPAIRED = "Cache for address [{ADDRESS}] repaired to block [{BLOCK}]";
const char* STR_MSG_INTERRUPT = "Repair of cache for address [{ADDRESS}] interrupted.";
const char* STR_MSG_READERROR = "Could not open cache for address [{ADDRESS}].";
