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
#if 0
    string_q fields =
        "CBlock:blockHash,blockNumber|"
        "CTransaction:to,from,blockHash,blockNumber|"
        "CReceipt:to,from,blockHash,blockNumber,transactionHash,transactionIndex,cumulativeGasUsed,root|"
        "CLogEntry:blockHash,blockNumber,transactionHash,transactionIndex,transactionLogIndex,removed,type";
    manageFields(fields, true);
    string_q result;
    queryRawLogs(result, trans.blockNumber, trans.blockNumber);
    if (opt->isVeryRaw) {
        opt->rawItems.push_back(result);
        return true;
    }
    CRPCResult generic;
    generic.parseJson3(result);
    CBlock bl;
    CTransaction tt; tt.pBlock = &bl;
    CReceipt receipt; receipt.pTrans = &tt;
    CLogEntry log; log.pReceipt = &receipt;
    while (log.parseJson3(generic.result)) {
        if (log.getValueByName("transactionIndex") == uint_2_Str(trans.transactionIndex)) {
            opt->rawItems.push_back(log.Format());
        }
        log = CLogEntry();
        log.pReceipt = &receipt;
    }
    if (opt->rawItems.size() == 0) {
        CReceipt rr;
        receipt.pTrans = &trans;
        CLogEntry ll;
        ll.pReceipt = &rr;
        ll.address = "not_an_real_log";
        opt->rawItems.push_back(ll.Format());
    }
#endif
