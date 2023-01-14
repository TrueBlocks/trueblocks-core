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
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CLogEntry));
        forEveryTransaction(visitTransaction, &options, options.transList.queries);
        once = false;
    }
    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//--------------------------------------------------------------
bool visitTransaction(CTransaction& trans, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    bool isText = (expContext().exportFmt & (TXT1 | CSV1));

    if (contains(trans.hash, "invalid")) {
        string_q hash = nextTokenClear(trans.hash, ' ');
        opt->errors.push_back("transaction " + hash + " not found");
        return true;  // continue even with an invalid item
    }

    if (opt->isRaw) {
        string_q result;
        CLogFilter query;
        query.blockHash = trans.blockHash;
        queryRawLogs(result, query);
        if (!isText && !opt->firstOut)
            cout << ",";
        cout << substitute(result, "mined", "");
        opt->firstOut = false;
        return true;
    }

    //////////////////////////////////////////////////////
    //////////////////////////////////////////////////////

    if (opt->articulate) {
        opt->abi_spec.loadAbiFromEtherscan(trans.to);
        opt->abi_spec.articulateTransaction(&trans);
    }
    for (auto log : trans.receipt.logs) {
        log.timestamp = trans.timestamp;
        if (opt->articulate) {
            opt->abi_spec.loadAbiFromEtherscan(log.address);
            opt->abi_spec.articulateLog(&log);
        }
        manageFields("CFunction:message", !log.articulatedLog.message.empty());

        if (isText) {
            cout << trim(log.Format(expContext().fmtMap["format"]), '\t') << endl;
        } else {
            if (!opt->firstOut)
                cout << ",";
            cout << "  ";
            indent();
            log.toJson(cout);
            unindent();
            opt->firstOut = false;
        }
    }
    return true;
}

/*
TODO: One user (thanks Matt!) pointed to a very common use case:
TODO: Give me all the events with topic emitted by address
TODO: He uses ethers.js and it returns the query very quickly (less than a minute for a very popular address).
TODO: TrueBlocks takes hours when using chifra export --emitter <address>. This is because TrueBlocks does not use the
TODO: filtering function the node provides for eth_getLogs. We kind of knew this path was slow for us, but have been
TODO: ignoring it since we always query for full transactional details and/or traces, but it is a very clear primary use
TODO: case for most people. We should bite the bullet and implement eth_getLogs with filtering. chifra logs pretty easy
TODO: speedup when querying logs hasno dependencies
TODO:
TODO:
TODO: ethers.js code:
TODO: const pairCreatedAbi = ["event PairCreated(address indexed token0, address indexed token1, address pair, uint)"];
TODO: const provider = new JsonRpcProvider(process.env.ETH_RPC_URL);
TODO: const factory = new Contract(UNISWAP_V2_FACTORY_ADDRESS, pairCreatedAbi, provider);
TODO: const pairCreatedFilter = factory.filters.PairCreated();
TODO: const logs = await factory.queryFilter(pairCreatedFilter, 10000834, 10500000);
TODO: const parsedLogs = logs.map(factory.interface.parseLog);
TODO: 10000834, 10500000 is the block range
TODO: You can leave out the 10500000 input if you want to query until the current block
TODO: Version 2.0 of the Uniswap Factory: 0x5c69bee701ef814a2b6a3edd4b1652cb9cc5aa6f
TODO: Version 3.0: 0x1f98431c8ad98523631ae4a59f267346ea31f984
*/
