/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2016, 2021 TrueBlocks, LLC (http://trueblocks.io)
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

//----------------------------------------------------------------
static bool chunkVisitFunc(const string_q& path, void* data) {
    // LOG_WARN(path);
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", chunkVisitFunc, data);

    } else {
        //        LOG_WARN(path);
        if (!endsWith(path, ".bin"))
            return true;

        blknum_t endBlock = NOPOS;
        blknum_t startBlock = path_2_Bn(path, endBlock);

        COptions* opts = (COptions*)data;
        //        cerr << opts->blocks.start << ":" << opts->blocks.stop << endl;
        if (opts->blocks.start != NOPOS && !inRange(opts->blocks.start, startBlock, endBlock)) {
            LOG_PROG("Skipped: " + path + "\r");
            return true;
        }

        ostringstream os;
        os << "./out/" << padNum9(startBlock) << '-' << padNum9(endBlock) << ".txt";
        string_q outFile = os.str();
        ofstream output;
        if (opts->save) {
            output.open(outFile, ios::out | ios::trunc);
            if (!output.is_open()) {
                LOG_ERR("Could not create file ", outFile, ". Quitting...");
                return false;
            }
            LOG_INFO("File opened for writing");
        }

        CIndexArchive index(READING_ARCHIVE);
        if (index.ReadIndexFromBinary(path)) {
            if (opts->save) {
                output << "start: " << startBlock << endl;
                output << "end: " << endBlock << endl;
                output << "fileSize: " << fileSize(path) << endl;
                output << "bloomSize: "
                       << fileSize(substitute(substitute(path, "finalized", "blooms"), ".bin", ".bloom")) << endl;
                output << "nAddrs: " << index.header->nAddrs << endl;
                output << "nRows: " << index.header->nRows << endl;
            }
            cout << "start: " << startBlock << endl;
            cout << "end: " << endBlock << endl;
            cout << "fileSize: " << fileSize(path) << endl;
            cout << "bloomSize: " << fileSize(substitute(substitute(path, "finalized", "blooms"), ".bin", ".bloom"))
                 << endl;
            cout << "nAddrs: " << index.header->nAddrs << endl;
            cout << "nRows: " << index.header->nRows << endl;
            for (uint32_t a = 0; a < index.nAddrs; a++) {
                CIndexedAddress* aRec = &index.addresses[a];
                if (opts->save) {
                    output << bytes_2_Addr(aRec->bytes) << endl;
                    for (uint32_t b = aRec->offset; b < (aRec->offset + aRec->cnt); b++) {
                        CIndexedAppearance* bRec = &index.appearances[b];
                        if (opts->save) {
                            output << "\t" << bRec->blk << "\t" << bRec->txid << endl;
                        }
                    }
                }
                LOG_INFO(a, " of ", index.nAddrs, " ", bytes_2_Addr(aRec->bytes) + "\r");
            }
            if (opts->save) {
                output.close();
                LOG_INFO("Wrote ", fileSize(outFile), " bytes to ", outFile);
            } else {
                LOG_PROG("Processed: " + path);
            }
        }
    }
    return true;
}

//----------------------------------------------------------------
bool COptions::handle_extract() {
    if (extract == "blooms") {
        return handle_extract_blooms();
    } else {
        establishFolder("out");
        return forEveryFileInFolder(indexFolder_finalized, chunkVisitFunc, this);
    }
    LOG_PROG("Finished");
    return true;
}

#if 0
string_q result;
queryRawBlockTrace(result, uint_2_Hex(bn));
cout << results << endl;

CLog Filter log Filter;
log Filter.fromBlock = bn;
log Filter.toBlock = bn;
queryRawLogs(result, log Filter);
cout << results << endl;

CBlock block;
getBlock(block);
for (auto tx : block.transactions) {
    queryRawReceipt(results, tx.hash);
    cout << results << endl;
}

// json.Marshal(RPCPayload{"2.0", "trace_block", RPCParams{fmt.Sprintf("0x%x", blockNum)}, 2})
// bool queryRawBlockTrace(string_q& blockStr, const string_q& hexNum) {

// payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getLogs", RPCParams{LogFilter{fmt.Sprintf("0x%x", blockNum), fmt.Sprintf("0x%x", blockNum)}}, 2})
// bool queryRawLogs(string_q& results, const CLog Filter& query) {

// payloadBytes, err := json.Marshal(RPCPayload{"2.0", "eth_getTransactionReceipt", RPCParams{hash}, 2})
// bool queryRawReceipt(string_q& results, const hash_t& txHash) {
#endif
