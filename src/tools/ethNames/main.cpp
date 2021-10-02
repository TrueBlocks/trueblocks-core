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
    nodeNotRequired();
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    bool first = true;
    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            continue;

        bool isText = (expContext().exportFmt & (TXT1 | CSV1 | NONE1));
        if (isText && options.items.size() == 0) {
            cout << "No results for " << command << endl;

        } else if (options.items.size() == 0) {
            cout << "{ \"data\": [ ";  // TODO: We used to send this, but it probably breaks something to not send it --
                                       // TODO: {\"results\" \"none\"} ";

        } else {
            for (auto item : options.items) {
                if (first)
                    cout << exportPreamble(expContext().fmtMap["header"], item.second.getRuntimeClass());
                if (isText) {
                    cout << trim(item.second.Format(expContext().fmtMap["format"]), '\t') << endl;
                } else {
                    if (!first)
                        cout << "," << endl;
                    cout << "  ";
                    indent();
                    item.second.toJson(cout);
                    unindent();
                }
                first = false;
            }
        }
        cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);
    }

    etherlib_cleanup();
    return 0;
}
