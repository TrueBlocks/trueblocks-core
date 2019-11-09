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
#include "acctlib.h"
#include "options.h"

extern void handle_generate(const COptions& options, CToml& toml, const string_q& dataFile, const string_q& ns);
//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {
    nodeNotRequired();
    acctlib_init(quickQuitHandler);

    CParameter::registerClass();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        if (options.mode & LIST)
            cout << "Classes found in the classDefinitions folder:\n";

        while (!options.classNames.empty()) {

            string_q className = nextTokenClear(options.classNames, '|');
            string_q fileName = "./classDefinitions/" + toLower(className) + ".txt";
            if (!fileExists(fileName)) {
                return options.usage("No class definition file found at " + fileName + "\n");

            } else {
                CToml toml(fileName);
                toml.readFile(fileName);

                if (options.mode & LIST) {
                    if (verbose) {
                        cout << string_q(80, '-') << "\nFile (dest): " << fileName << "\n";
                        cout << toml << "\n";

                    } else if (!toml.getConfigBool("settings", "disabled", false)) {
                        cout << "\t" << toml.getConfigStr("settings", "class", "") << "\n";
                    }

                } else if (options.mode & EDIT) {
                    editFile(fileName);

                } else {
                    ASSERT(options.mode & RUN);
                    if (isTestMode())
                        cout << "Would run class definition file: " << className << " (not run, testing)\n";
                    else if (verbose)
                        cerr << "Running class definition file '" << className << "'\n";

                    if (!isTestMode()) {
                        if (toml.getConfigBool("settings", "disabled", false)) {
                            if (verbose)
                                cerr << "Disabled class not processed " << className << "\n";
                        } else {
                            handle_generate(options, toml, fileName, options.nspace);
                        }
                    }
                }
            }
        }
    }

    acctlib_cleanup();
    return 0;
}
