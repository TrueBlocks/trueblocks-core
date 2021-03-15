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
#include "etherlib.h"
#include "record.h"

extern void reportRecord(const CRecord& record);
extern const char* STR_OUTPUT;
//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    CRecord::registerClass();
    CBalance::registerClass();
   
    blknum_t latest = getBlockProgress(BP_CLIENT).client;
    CAccountNameArray grants;
    string_q contents = asciiFileToString("./grants.json");
    CAccountName name;
    while (name.parseJson3(contents)) {
        name.address = toLower(name.address);
        grants.push_back(name);
        name = CAccountName();
    }

    //string_q types[] = {"txs", "logs", "traces"};
    uint32_t cnt = 1;
    cout << "export const grantsData = [\n";
    for (auto grant : grants) {
        string_q fn = getCachePath("monitors/" + toLower(grant.address) + ".acct.bin");
        bool exists = fileExists(fn);

        CRecord record;
        record.key = cnt++;
        record.date = (exists ? fileLastModifyDate(fn).Format(FMT_JSON) : "n/a");
        record.type = "logs"; // types[cnt % 3];
        record.grant_id = (uint32_t)str_2_Uint(substitute(grant.name, "Grant ", ""));
        nextTokenClear(grant.name, ' ');
        nextTokenClear(grant.name, ' ');
        record.name = substitute(grant.name.substr(0, 50), "'", "&#39;");
        record.address = grant.address;
        record.slug = grant.source;
        record.cnt = (uint32_t)(exists ? (fileSize(fn) / sizeof(CAppearance_base)) : 0);
        wei_t balance = getBalanceAt(grant.address, latest);
        CBalance bal;
        bal.asset = "ETH";
        bal.balance = padRight(wei_2_Ether(balance), 18, '0');
        if (balance == 0)
            bal.balance = "0." + bal.balance;
        record.bals["ETH"] = bal;
        record.has_data =
            fileExists("./data/" + record.address + ".json") || fileExists("./data/" + record.address + ".csv");
        cout << substitute(record.Format(STR_OUTPUT), "[{KEY}]", uint_2_Str(cnt)) << endl;

        cerr << fn << " : " << exists << " : " << (record.has_data ? "true" : "false") << endl;
    }
    cout << "]";

    etherlib_cleanup();
    return 1;
}

const char* STR_OUTPUT =
    "  {\n"
    "    key: [{KEY}],\n"
    "    date: '[{DATE}]',\n"
    "    type: '[{TYPE}]',\n"
    "    grant_id: [{GRANT_ID}],\n"
    "    address: '[{ADDRESS}]',\n"
    "    name: '[{NAME}]',\n"
    "    slug: '[{SLUG}]',\n"
    "    cnt: [{CNT}],\n"
    "    bals: [{BALS}],\n"
    "    has_data: [{HAS_DATA}],\n"
    "  },";
