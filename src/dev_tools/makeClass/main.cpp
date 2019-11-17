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

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
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

        for (auto classDef : options.classDefs) {
            if (!fileExists(classDef.inputPath)) {
                return options.usage("No class definition file found at " + classDef.inputPath + "\n");

            } else {
                CToml toml(classDef.inputPath);
                toml.readFile(classDef.inputPath);

                if (options.mode & LIST) {
                    if (verbose) {
                        cout << string_q(80, '-') << "\nFile (dest): " << classDef.inputPath << "\n";
                        cout << toml << "\n";

                    } else {
                        cout << "\t" << toml.getConfigStr("settings", "class", "") << "\n";
                    }

                } else if (options.mode & EDIT) {
                    editFile(classDef.inputPath);

                } else {
                    ASSERT(options.mode & RUN);
                    if (isTestMode())
                        cout << "Would run class definition file: " << classDef.className << " (not run, testing)\n";
                    else if (verbose)
                        cerr << "Running class definition file '" << classDef.className << "'\n";

                    if (!isTestMode()) {
                        if (toml.getConfigBool("settings", "disabled", false)) {
                            if (verbose)
                                cerr << "Disabled class not processed " << classDef.className << "\n";
                        } else {
                            options.handle_generate(toml, classDef, options.nspace);
                        }
                    }
                }
            }
        }
    }

    acctlib_cleanup();
    return 0;
}
