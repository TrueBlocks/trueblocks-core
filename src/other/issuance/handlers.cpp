/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)
 *
 * This program is free software: you may redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version. This program is
 * distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details. You should have received a copy of the GNU General
 * Public License along with this program. If not, see http://www.gnu.org/licenses/.
 *-------------------------------------------------------------------------------------------*/
#include "options.h"

const string_q uncleBlocks = "./data/uncle_blocks.csv";
const string_q resultsFile = "./data/results.csv";
//--------------------------------------------------------------
bool COptions::model_issuance(void) {
    if (!fileExists(uncleBlocks))
        return usage("Cannot open list of uncle blocks. Run with --uncles option first.");

    blknum_t prev = 0;
    CUintArray blocks;

    string_q contents = asciiFileToString(uncleBlocks);
    explode(blocks, contents, '\n');

    for (auto block : blocks) {
        for (blknum_t bn = prev; bn < block; bn++) {
            CRewardReconcilation rec;
            rec.blockNumber = bn;
            rec.baseReward = getBlockReward(bn, false);
            rec.timestamp = getBlockTimestamp(bn);
            if (bn == prev) {
                // This block has an uncle
                rec.baseAddReward = getBlockReward(bn, true) - rec.baseReward;
                uint64_t count = getUncleCount(bn);
                for (size_t i = 0; i < count; i++) {
                    CBlock uncle;
                    getUncle(uncle, bn, i);
                    rec.uncleReward += getUncleReward(bn, uncle.blockNumber);
                }
            }
            cout << rec.Format(CSV_DISPLAY_REWARDRECONCILATION) << endl;
        }
        prev = block;
    }
    return true;
}

//--------------------------------------------------------------
bool COptions::audit_issuance(void) {
    return true;
}

//--------------------------------------------------------------
bool COptions::show_uncle_blocks(void) {
    blknum_t latest = getLatestBlock_client();
    for (size_t bn = 0 ; bn < latest ; bn++) {
        if (getUncleCount(bn))
            cout << bn << endl;
    }
    return true;
}

//--------------------------------------------------------------
bool COptions::loadResults(CRewardReconcilationArray& recs) {

    if (!fileExists(resultsFile))
        return usage("Cannot open results file. Run with --uncles first, then --generate option.");

    cerr << "Loading results file ";
    string_q binFile = substitute(resultsFile, ".csv", ".bin");
    if (fileExists(binFile)) {
        time_q textDate = fileLastModifyDate(resultsFile);
        time_q binDate = fileLastModifyDate(binFile);
        if (binDate > textDate) {
            cerr << "from cache" << endl;
            CArchive archive(READING_ARCHIVE);
            if (!archive.Lock(binFile, modeReadOnly, LOCK_WAIT))
                return usage("Cannot open archive file " + binFile);
            archive >> recs;
            archive.Release();
            return true;
        }
    }

    cerr << "from " << resultsFile << endl;
    string_q contents = asciiFileToString(resultsFile);

    cerr << "Exploding lines" << endl;
    CStringArray lines, fields;
    explode(lines, contents, '\n');

    size_t lineNo = 0;
    for (auto line: lines) {
        if (lineNo == 0) {
            explode(fields, line, ',');
        } else {
            CRewardReconcilation rec;
            rec.parseCSV(fields, line);
            recs.push_back(rec);
        }
        cerr << "Parsing: " << lineNo << "\r";
        cerr.flush();
        lineNo++;
    }

    CArchive archive(WRITING_ARCHIVE);
    if (!archive.Lock(binFile, modeWriteCreate, LOCK_NOWAIT))
        return usage("Cannot open archive file " + binFile);
    archive << recs;
    archive.Release();

    return true;
}

//--------------------------------------------------------------
bool COptions::summary_by_period(void) {
    CRewardReconcilationArray recs;
    if (!loadResults(recs))
        return false;

    string_q per = (by_period == BY_DAY ? "[{DAY}]" : "[{MONTH}]");
    string_q not_per = (by_period == BY_DAY ? "[{MONTH}]" : "[{DAY}]");
    string_q fmt = substitute(CSV_DISPLAY_REWARDRECONCILATION, (","+not_per), "");
    time_q prev(2015,7,30,0,0,0);
    CRewardReconcilation accumulator;
    accumulator.timestamp = blockZeroTs;
    for (auto rec : recs) {
        time_q cur = str_2_Date(rec.Format(per));
        if (!isSamePeriod(by_period, prev, cur)) {
            prev = cur;
            cout << accumulator.Format(fmt) << endl;
            if (discrete) {
                CRewardReconcilation reset;
                accumulator = reset;
            }
            accumulator.blockNumber = rec.blockNumber;
            accumulator.timestamp = rec.timestamp;
        }
        accumulator += rec;
    }

    return true;
}

