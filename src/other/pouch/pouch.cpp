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

//----------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);
    CRecord::registerClass();

    CAccountNameArray grants;
    string_q contents = asciiFileToString("./grants.json");
    CAccountName name;
    while (name.parseJson3(contents)) {
        grants.push_back(name);
        name = CAccountName();
    }

    string_q types[] = { "txs", "logs", "traces" };
    uint32_t cnt = 1;
    cout << "[";
    for (auto grant : grants) {
        if (cnt != 1)
            cout << ",";
        CRecord record;
        record.key = cnt++;
        record.date = Now().Format(FMT_JSON);
        record.type = types[cnt%3];
        record.name = grant.name;
        record.address = grant.address;
        record.cnt = cnt;
        record.csv = "Download CSV";
        record.json = "Download JSON";
        cout << record << endl;
    }
    cout << "]";

    etherlib_cleanup();
    return 1;
}

