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
        string_q format = expContext().fmtMap["nick"];  // order matters

        if (options.exportFmt & (TXT1|CSV1) && options.items.size() == 0) {
            LOG_INFO("No matches");

        } else {
            for (size_t a = 0 ; a < options.items.size() ; a++) {
                if (a == 0)
                    cout  << exportPreamble(options.exportFmt, format, GETRUNTIME_CLASS(CAccountName));
                if (options.exportFmt & (TXT1|CSV1))
                    cout << options.items[a].Format(format) << endl;
                else {
                    if (a > 0)
                        cout << "," << endl;
                    cout << "  "; incIndent();
                    options.items[a].doExport(cout);
                    decIndent();
                }
            }
        }
    }
    cout << exportPostamble(options.exportFmt);

    return 0;
}
