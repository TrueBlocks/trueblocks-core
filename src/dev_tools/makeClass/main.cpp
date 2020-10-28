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

        options.counter = CCounter();  // reset
        LOG_INFO(cYellow, "handling generate...", cOff);
        for (auto classDef : options.classDefs) {
            CToml toml(classDef.input_path);
            toml.readFile(classDef.input_path);
            if (options.mode & EDIT) {
                editFile(classDef.input_path);

            } else {
                ASSERT(options.mode & RUN);
                if (verbose > 3)
                    cout << "Running class definition file '" << classDef.input_path << "'" << endl;
                else if (verbose)
                    cout << "Running class definition file '" << classDef.short_fn << "'" << endl;
                options.handle_generate(toml, classDef, options.nspace, false);
            }
        }
        LOG_INFO(cYellow, "makeClass --run", cOff, " processed ", options.counter.nVisited, " files (changed ",
                 options.counter.nProcessed, ").", string_q(40, ' '));
    }

    acctlib_cleanup();
    return 0;
}
