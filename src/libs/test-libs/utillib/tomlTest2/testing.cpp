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

// namespace qblocks { extern string_q collapseArrays(const string_q& inStr); }  // namespace qblocks
// extern const char* STR_TEST_TOML;
//--------------------------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    // Parse command line, allowing for command files
    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    for (auto command : options.commandLines) {
        if (!options.parseArguments(command))
            return 0;

        CToml toml(options.filename);
        cout << options.filename << endl;
        cout << toml;
    }

    //    cout << string_q(120,'-') << endl;
    //    cout << "Prior to collapsing arrays" << endl;
    //    cout << string_q(120,'-') << endl;
    //    cout << STR_TEST_TOML << endl;
    //    cout << string_q(120,'-') << endl;
    //    cout << "After collapsing arrays" << endl;
    //    cout << collapseArrays(STR_TEST_TOML) << endl;
    //    cout << string_q(120,'-') << endl;

    return 0;
}

////--------------------------------------------------------------------------------------
// const char* STR_TEST_TOML =
// "[settings]\n"
// "junk = 1\n"
// "[[folders]]\n"
// "list = [\n"
// "        { \"folder\": \"/Users/jrush/Development/trueblocks-core/src\" },\n"
// "        ]\n"
// "[[changes]]\n"
// "list = [\n"
// "        {\n"
// "        \"from\": \"\n    // Return field values\n\",\n"
// "        \"to\": \"\n// EXISTING_CODE\n// EXISTING_CODE\n    // Return field values\n\",\n"
// "        \"includes\": [ \".cpp\" ],\n"
// "        \"enabled\": true\n"
// "        }\n"
// "        ]\n";
