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

bool processPair(uint64_t blockNum, void* data);
//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    acctlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool once = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            continue;

        if (once)
            cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CTokenBalanceRecord));

        for (auto holder : options.holders) {
            for (auto token : options.tokens) {
                options.curToken.holder = holder;
                options.curToken.address = token;
                options.blocks.forEveryBlockNumber(processPair, &options);
            }
        }
        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

        once = false;
    }

    if (options.requestsHistory() && !nodeHasBalances(true))
        LOG_WARN("Your node does not report historical state. The results presented above are incorrect.");

    return 0;
}

//-----------------------------------------------------------------------
typedef struct {
    tokstate_t bits;
    string_q field;
} marker_t;
const vector<marker_t> base = {
    {TOK_NAME, "name"}, {TOK_SYMBOL, "symbol"}, {TOK_DECIMALS, "decimals"}, {TOK_TOTALSUPPLY, "totalSupply"}};
const vector<marker_t> bals = {{TOK_BALANCE, "balanceOf"}};

//-----------------------------------------------------------------------
bool processPair(uint64_t blockNum, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    opt->curToken.blockNumber = blockNum;
    if (opt->tsMemMap && opt->tsCnt > blockNum) {
        opt->curToken.date = ts_2_Date((timestamp_t)opt->tsMemMap[(blockNum * 2) + 1]).Format(FMT_JSON);
    }
    if ((opt->modeBits & TOK_TOTALSUPPLY) || !opt->getNamedAccount(opt->curToken, opt->curToken.address)) {
        for (auto marker : base) {
            if (opt->modeBits & marker.bits) {
                string_q value = getTokenState(opt->curToken, marker.field, opt->abi_spec, blockNum);
                opt->curToken.setValueByName(marker.field, value);
                if ((marker.field == "name" || marker.field == "symbol") && !value.empty())
                    opt->curToken.is_erc20 = true;
                if (marker.field == "decimals" && str_2_Uint(value) > 0)
                    opt->curToken.is_erc20 = true;
            }
        }
    }

    static bool first = true;
    if (opt->modeBits & TOK_BALANCE) {
        string_q val = getTokenBalanceOf(opt->curToken, opt->curToken.holder, blockNum);
        if (val == "0" && opt->no_zero) {
            LOG_INFO("Skipping: ", opt->curToken.holder, " at ", blockNum, "\r");
            return !shouldQuit();
        }
        opt->curToken.setValueByName("balance", val);
    }
    bool isText = expContext().exportFmt == TXT1 || expContext().exportFmt == CSV1;
    if (isText) {
        cout << opt->curToken.Format(expContext().fmtMap["format"]) << endl;
    } else {
        if (!first)
            cout << "," << endl;
        cout << "  ";
        indent();
        opt->curToken.toJson(cout);
        unindent();
        first = false;
    }

    return !shouldQuit();
}
