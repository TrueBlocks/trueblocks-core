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
int main(int argc, const char *argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        string_q cmd = "acctScrape";
        cout << "pgrep -i " << cmd << ": " << doCommand("pgrep -i " + cmd) << endl;
        cout << "pgrep -f " << cmd << ": " << doCommand("pgrep -f " + cmd) << endl;
        cout << "pgrep -l " << cmd << ": " << doCommand("pgrep -l " + cmd) << endl;
        cout << "pgrep -fl " << cmd << ": " << doCommand("pgrep -fl " + cmd) << endl;
        cout << "pgrep -fl " << toLower(cmd) << ": " << doCommand("pgrep -fl " + toLower(cmd)) << endl;
        cout << "pgrep -fil " << toLower(cmd) << ": " << doCommand("pgrep -fli " + toLower(cmd)) << endl;
        cout << "ps -ef | grep -i " << toLower(cmd) << " | grep -v grep: " << doCommand("ps -ef | grep -i " + toLower(cmd) + " | grep -v grep | grep -v 'sh -c'") << endl;
        cout << endl;

        cmd = "runningTest";
        cout << "pgrep -i " << cmd << ": " << doCommand("pgrep -i " + cmd) << endl;
        cout << "pgrep -f " << cmd << ": " << doCommand("pgrep -f " + cmd) << endl;
        cout << "pgrep -l " << cmd << ": " << doCommand("pgrep -l " + cmd) << endl;
        cout << "pgrep -fl " << cmd << ": " << doCommand("pgrep -fl " + cmd) << endl;
        cout << "pgrep -fl " << toLower(cmd) << ": " << doCommand("pgrep -fl " + toLower(cmd)) << endl;
        cout << "pgrep -fil " << toLower(cmd) << ": " << doCommand("pgrep -fli " + toLower(cmd)) << endl;
        cout << "ps -ef | grep -i " << toLower(cmd) << " | grep -v grep: " << doCommand("ps -ef | grep -i " + toLower(cmd) + " | grep -v grep | grep -v 'sh -c'") << endl;

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
