/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//----------------------------------------------------------------------------------
// Walks through the blocks already stored on disc with blockMan, and builds a
// new-line-delimited index of non-empty blocks by block number.
//----------------------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    cout << bGreen << "Starting blockMan...\n" << cOff;
    CStringArray commands;
    explode(commands, options.commandList, '\n');
    for (auto command : commands) {
        if (!options.parseArguments(command))
            return 0;

        // There can be more than one thing to do...
        while (!options.mode.empty()) {
            string_q mode = nextTokenClear(options.mode, '|');
            if (mode == "check") {
                // The user wants to check that all blocks marked empty are
                // indeed empty, and that all blocks that we think contain
                // transactions do actually contain transactions.
                if (options.deepCheck) {
                    expContext().hexNums = true;
                    expContext().hashesOnly = true;
                }
                uint64_t top = getLatestBlockFromClient();
                if (!options.noEmpty) {
                    forEveryEmptyBlockOnDisc(visitEmptyBlock, &options, options.startBlock, top);
                }
                if (!options.emptyOnly) {
                    forEveryNonEmptyBlockOnDisc(visitNonEmptyBlock, &options, options.startBlock, top);
                }

            } else if (mode == "list") {
                // The user simply wants a list of non-empty blocks.
                uint64_t maxV = 0, maxEnd = 0, run = 0, curRun = 0, runEnd = 0, lastBlock = 0;
                FILE *fp = fopen(fullBlockIndex.c_str(), binaryReadOnly);
                if (fp) {
                    uint64_t nItems = (uint64_t)fileSize(fullBlockIndex) / sizeof(uint64_t);
                    uint64_t *contents = new uint64_t[nItems];
                    if (fread(contents, nItems, sizeof(uint64_t), fp)) {} // leave this here--avoids a warning
                    uint64_t prev = 0;
                    if (verbose) cout << "\n";
                    for (uint64_t i = 0 ; i < nItems ; i++) {
                        uint64_t diff = (contents[i]-prev);
                        if (verbose) {
                            uint64_t val = (i % (uint64_t)options.skip);
                            if (!val) {
                                cout << "\t" << contents[i] << " " << diff << " " << curRun << "       " << "\n";
                                cout.flush();
                            }
                        }
                        lastBlock = max(lastBlock, contents[i]);
                        if (contents[i] == prev + 1) {
                            curRun++;
                            if (curRun > run) {
                                runEnd = contents[i];
                                run = curRun;
                            }
                        } else {
                            curRun = 0;
                        }
                        prev = contents[i];
                        if (diff != 46147) {  // the first 46147 blocks are empty
                            if (diff > maxV) {
                                maxEnd = contents[i];
                                maxV = diff;
                            }
                        }
                    }
                    maxV -= 1; run += 1;

                    float pct = ((float)(((nItems*1.)/(lastBlock*1.)) + 0.005)) * (float)100.;
                    if (verbose) cout << "                    \r";
                    cout << "\n";
                    cout << cGreen << "\tfullBlocks listing:\n" << cOff;
                    cout << "\t" << bBlue << "stats:" << cOff << "\t"
                        << "Non-empty blocks: " << asYellow(nItems) << " (" << double_2_Str(pct, 2).c_str() << "%)\n\t\t"
                        << "Empty blocks:\t  " << asYellow(lastBlock-nItems) << " (" << double_2_Str(100.-pct, 2).c_str() << "%)\n\t\t"
                        << "Total blocks:\t  " << asYellow(lastBlock) << "\n";
                    if (verbose) {
                        cout << "\t" << bBlue << "runs:" << cOff << "\t"
                            << asYellow(maxV) << " empty blocks between ["
                            << asYellow(maxEnd-maxV) << ", " << asYellow(maxEnd) << ")\n\t\t"
                            << asYellow(run) << " non-empty blocks between ["
                            << asYellow(runEnd+1-run) << ", " << asYellow(runEnd+1) << ")\n";
                    }
                    delete [] contents;
                    fclose(fp);
                }

            }
        }
        cout << bGreen << "...done\n" << cOff;
    }

    // Not strictly needed, but can't hurt.
    etherlib_cleanup();
}
