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
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired(); // This command will run without a node
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.isEdit) {
            editFile(options.namesFile.getFullPath());

        } else if (options.filtered.size() == 0) {
            LOG_INFO("No matches found");

        } else {

            string_q fmt = expContext().fmtMap["nick"];
            bool isJson = fmt.empty();

            if (isJson && options.filtered.size() > 1)
                cout << "[";
            uint64_t cnt = 0;
            for (auto acct : options.filtered) {
                if (isJson && cnt++ > 0)
                    cout << "," << endl;
                cout << acct.Format(fmt);
                if (!isJson)
                    cout << endl;
            }
            if (isJson)
                cout << endl;
            if (isJson && options.filtered.size() > 1)
                cout << "]";
        }
    }

    return 0;
}
