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
#include "options.h"

//--------------------------------------------------------------
void doOneTest(const string_q& cmd, const string_q& prog) {
    ostringstream os;
    os << cmd << " " << prog;
    string_q result = doCommand(os.str().c_str());
    replaceAny(result, "0123456789", "");
    if (contains(result, getHomeFolder())) {
        replace(result, "cd ", "`cd `");
        replace(result, " >", "` >`");
        replaceAll(result, "/test/gold/", "/test/`gold/");
        replaceAll(result, "/test/working/", "/test/`working/");
        CStringArray splits;
        explode(splits, result, '`');
        result = splits[0] + " " + splits[1] + " " + splits[3] + " " + splits[4] + splits[6];
    }
    cout << cmd << " " << prog << ": [" << endl << result << endl << "]" << string_q(80, '-') << endl;
}

//--------------------------------------------------------------
void doTest(const string_q& prog) {
    doOneTest("pgrep -i", prog);
    doOneTest("pgrep -f", prog);
    doOneTest("pgrep -l", prog);
    doOneTest("pgrep -fl", prog);
    doOneTest("pgrep -a", prog);
    doOneTest("pgrep -fla", prog);
    doOneTest("pgrep -x", prog);
    doOneTest("pgrep -flax", prog);
    doOneTest("pgrep -x", prog);
    doOneTest("pgrep -flx", prog);
    doOneTest("pgrep -x", prog);
    doOneTest("pgrep -fx", prog);
    doOneTest("pgrep -x", prog);
    doOneTest("pgrep -lx", prog);
}

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
#if 0
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        doTest("notRunning");
        cout << endl;

        doTest("runningTest");
        cout << endl;

        cout << "isRunning(notRunning): " << isRunning("notRunning") << endl;
        cout << "isRunning(runningTest): " << isRunning("runningTest") << endl;
        cout << "isRunning(testRunner): " << isRunning("testRunner") << endl;
    }
#endif
    return 0;
}
