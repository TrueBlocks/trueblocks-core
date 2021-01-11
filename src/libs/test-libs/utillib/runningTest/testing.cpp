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
    cout << cmd << " " << prog << ": [" << endl
         << doCommand(os.str().c_str()) << endl
         << "]" << string_q(80, '-') << endl;
}

//--------------------------------------------------------------
void doTest(const string_q& prog) {
    doOneTest("pgrep -i", prog);
    doOneTest("pgrep -f", prog);
    doOneTest("pgrep -l", prog);
    doOneTest("pgrep -fl", prog);
    doOneTest("pgrep -fl", toLower(prog));
    doOneTest("pgrep -fli", toLower(prog));
    doOneTest("pgrep -a", prog);
    doOneTest("pgrep -fla", prog);
    doOneTest("pgrep -fla", toLower(prog));
    doOneTest("pgrep -flia", toLower(prog));
    //    cout << "pgrep -i " << prog << ": " << doCommand("pgrep -i " + prog) << endl;
    //    cout << "pgrep -f " << prog << ": " << doCommand("pgrep -f " + prog) << endl;
    //    cout << "pgrep -l " << prog << ": " << doCommand("pgrep -l " + prog) << endl;
    //    cout << "pgrep -fl " << prog << ": " << doCommand("pgrep -fl " + prog) << endl;
    //    cout << "pgrep -fl " << toLower(prog) << ": " << doCommand("pgrep -fl " + toLower(prog)) << endl;
    //    cout << "pgrep -fil " << toLower(prog) << ": " << doCommand("pgrep -fli " + toLower(prog)) << endl;
    //    cout << "ps -ef | grep -i " << toLower(prog) << " | grep -v grep: " << doCommand("ps -ef | grep -i " +
    //    toLower(prog) + " | grep -v grep | grep -v 'sh -c'") << endl;
}

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
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

        //        cout << "isRunning(\"not running\"): " << isRunning("Not running") << endl;
        //        cout << "isRunning(\"runningTest\", true): " << isRunning("runningTest") << endl;
        //        cout << "isRunning(\"testRunner\", true): " << isRunning("testRunner") << endl;
        //        cout << "nRunning(\"runningTest\"): " << nRunning("runningTest") << endl;
        //        cout << "nRunning(\"testRunner\"): " << nRunning("testRunner") << endl;
        //        cout << "isRunning_better(\"runningTest\"): " << isRunning_better("runningTest") << endl;
        //        cout << "isRunning_better(\"testRunner\"): " << isRunning_better("testRunner") << endl;
    }
    return 0;
}
