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
#include "options.h"
#include "reporter.h"

double rate1 = 1.0;
double rate2 = 1.0;
extern bool visitMini(CMiniBlock& block, const CMiniTrans *trans, void *data);
extern bool visitBlock(CBlock& block, void *data);
//--------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init();

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    // while (!options.commandList.empty())
    {
        string_q command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        // Load the cache first so it doesn't effect timing
        verbose = true;
        forEveryMiniBlockInMemory(NULL, NULL, 0, 1);
        cOff = cTeal;
        cerr << "\n" << cOff;
        verbose = false;

        string_q msgs[] = {
            "Press enter to get 50 blocks from Infura",
            "...50 blocks from Parity",
            "...50 blocks from QBlocks",
            "...1,000 blocks from Infura",
            "...1,000 blocks from Parity",
            "...1,000 blocks from QBlocks",
            "...10,000 blocks from Parity",
            "...10,000 blocks from QBlocks",
            "...10,000 blocks ignoring empty blocks",
            "...10,000 blocks ignoring data input",
            "...100,000 blocks from QBlocks",
        };
        int cnt=0;

        endMsg(msgs[cnt++]);
        { CSpeedTest rep("remote", options.startBlock,N_BLOCKS / 20);   forEveryBlockFromClient    (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("local",  options.startBlock,N_BLOCKS / 20);   forEveryBlockFromClient    (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("binary", options.startBlock,N_BLOCKS / 20);   forEveryBlockFromClient    (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("remote", options.startBlock,N_BLOCKS);        forEveryBlockFromClient    (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("local",  options.startBlock,N_BLOCKS);        forEveryBlockFromClient    (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("binary", options.startBlock,N_BLOCKS);        forEveryBlockOnDisc        (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("local",  options.startBlock,N_BLOCKS * 10);   forEveryBlockFromClient    (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("binary", options.startBlock,N_BLOCKS * 10);   forEveryBlockOnDisc        (visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("binary", options.startBlock,N_BLOCKS * 10);   forEveryNonEmptyBlockOnDisc(visitBlock, &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("binary", options.startBlock,N_BLOCKS * 10);   forEveryMiniBlockInMemory  (visitMini,  &rep); rep.finalReport(msgs[cnt++]); }
        { CSpeedTest rep("binary", options.startBlock,N_BLOCKS * 100);  forEveryMiniBlockInMemory  (visitMini,  &rep); rep.finalReport(); }
    }

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool visitBlock(CBlock& block, void *data) {
    CSpeedTest *reporter = reinterpret_cast<CSpeedTest*>(data);

    reporter->nBlocksVisited++;
    reporter->totalGasUsed += biguint_t(block.gasUsed);
    for (uint32_t tr = 0; tr < block.transactions.size(); tr++) {
        reporter->nTransVisited++;
        reporter->totalWei += block.transactions[tr].value;
    }
    reporter->interumReport(block.blockNumber, block.transactions.size());

    return !shouldQuit();
}

//-----------------------------------------------------------------------
bool visitMini(CMiniBlock& block, const CMiniTrans *trans, void *data) {
    CSpeedTest *reporter = reinterpret_cast<CSpeedTest*>(data);

    ASSERT(trans);
    reporter->nBlocksVisited++;
    for (uint32_t tr = (uint32_t)block.firstTrans; tr < block.firstTrans+block.nTrans; tr++) {
        const CMiniTrans *mini = &trans[tr];
        reporter->nTransVisited++;
        reporter->totalWei += str_2_Wei(mini->value);
        reporter->totalGasUsed += biguint_t(mini->gasUsed);
    }
    reporter->interumReport(block.blockNumber, block.nTrans);

    return !shouldQuit();
}

//-----------------------------------------------------------------------
void CSpeedTest::interumReport(blknum_t curBlock, blknum_t tThis) {
    if (!(nBlocksVisited % 2)) {
        duration = max(0.1, (qbNow() - startTime));
        cout << padRight(src+":",7) << cWhite;
        cout.flush();
        progressBar(nBlocksVisited, size(), duration, "");
    }
}

//-----------------------------------------------------------------------
void CSpeedTest::finalReport(const string_q& msg) const {

    ((CSpeedTest*)this)->duration = getProgBarVal();
    cout << padRight(src+":",7) << cWhite;
    cout.flush();
    progressBar(size(), size(), duration, "");

    cout << "\n" << "  "
        << bWhite << nBlocksVisited << cOff << " of " << bWhite << size() << cOff << " blocks | "
        << bWhite << nTransVisited << cOff << " trans | "
        << bWhite << totalWei << cOff << " wei | "
        << bWhite << totalGasUsed << cOff << " gas | "
        << bWhite << nBlocksVisited/(duration) << cOff << " b/s | "
        << bWhite << nTransVisited/(duration) << cOff << " t/s ";

    string_q speedup;
    if (rate1 == 1.0) {
        rate1 = (nBlocksVisited/duration);
    } else {
        double r = (nBlocksVisited/duration);
        double t = (r/rate1);
        int rr = int(t * 100.);
        double d = (rr/100.);
        cout << "| speedup: " << cGreen << d << cOff;
        if (rate2 == 1.0) {
            rate2 = (nBlocksVisited/duration);
        } else {
            r = (nBlocksVisited/duration);
            t = (r/rate2);
            rr = int(t * 100.);
            d = (rr/100.);
            cout << " / " << cGreen << d << cOff;
        }
    }
    endMsg(msg);
}

//-----------------------------------------------------------------------
void endMsg(const string_q& msg) {
    cout << "\n\n";
    cout << bMagenta << msg << ": " << cOff;
    cout.flush();
    int c = getchar();
    if (c == 'q')
        exit(0);
}
