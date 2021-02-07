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

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;
        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CTransaction));
        forEveryTransactionInList(visitTransaction, &options, options.transList.queries);
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//----------------------------------------------------------------
bool visitAddrs(const CAppearance& item, void* data) {
    // We do not account for zero addresses or the addresses found in the zeroth trace since
    // it's identical to the transaction itself
    if (item.tc == 10 || isZeroAddr(item.addr))
        return !shouldQuit();
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));
    if (isText) {
        cout << trim(item.Format(expContext().fmtMap["format"]), '\t') << endl;
    } else {
        if (!opt->first)
            cout << ",";
        cout << "  ";
        indent();
        item.toJson(cout);
        unindent();
        opt->first = false;
    }
    return !shouldQuit();
}

//----------------------------------------------------------------
// Return 'true' if we want the caller NOT to visit the traces of this transaction
bool transFilter(const CTransaction* trans, void* data) {
    if (!ddosRange(trans->blockNumber))
        return false;
    return (getTraceCount(trans->hash) > 250);
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        opt->errors.push_back("Transaction " + hash + " not found.");
        return true;  // continue even with an invalid item
    }

    if (opt->uniq) {
        trans.forEveryUniqueAppearanceInTxPerTx(visitAddrs, transFilter, opt);
        return true;
    }

    if (opt->isRaw || opt->isVeryRaw) {
        string_q result;
        queryRawTransaction(result, trans.getValueByName("hash"));
        if (!isText && !opt->first)
            cout << ",";
        cout << result;
        opt->first = false;
        return true;
    }

    //////////////////////////////////////////////////////
    if (opt->trace)
        getTraces(trans.traces, trans.getValueByName("hash"));
    //////////////////////////////////////////////////////

    if (opt->articulate) {
        opt->abi_spec.loadAbiFromEtherscan(trans.to);
        for (auto log : trans.receipt.logs)
            opt->abi_spec.loadAbiFromEtherscan(log.address);
        opt->abi_spec.articulateTransaction(&trans);
    }

    manageFields("CFunction:message", !trans.articulatedTx.message.empty());
    if (true) {
        if (isText) {
            cout << trim(trans.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            if (!opt->first)
                cout << ",";
            cout << "  ";
            indent();
            trans.toJson(cout);
            unindent();
            opt->first = false;
        }
    }

    if (opt->force) {
        string_q txFilename = getBinaryCacheFilename(CT_TXS, trans.blockNumber, trans.transactionIndex);
        if (!fileExists(txFilename)) {
            CBlock block;
            getBlock_light(block, trans.blockNumber);
            trans.timestamp = block.timestamp;
            trans.receipt.status = NO_STATUS;
            trans.pBlock = &block;
            if (trans.blockNumber >= byzantiumBlock)
                getReceipt(trans.receipt, trans.hash);
            writeTransToBinary(trans, txFilename);
        }
    }

    return true;
}
