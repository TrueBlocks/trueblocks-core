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

        CToml toml(options.filename);
        cout << options.filename << "\n" << toml << "\n";
        cout << "name: " << "\t\t" << toml.getConfigStr("settings", "name", "NONAME") << "\n";
        cout << "max_width: " << "\t" << toml.getConfigInt("display", "max_width", 10000) << "\n";
        cout << "version: " << "\t\t" << toml.getConfigStr("version", "version", "0.0.0") << "\n";
        if (!toml.getConfigStr("display", "fmt_whale_field", "").empty())
            cout << "whale: " << "\t\t" << toml.getConfigStr("display", "fmt_whale_field", "") << "\n";

        string_q contents;
        asciiFileToString(options.filename, contents);
        cout << endl << "File contents as read" << endl;
        cout << contents << endl;

        string_q junkFile = options.filename + ".junk";
        toml.setFilename(junkFile);
        toml.writeFile();
        asciiFileToString(junkFile, contents);
        cout << endl << "File contents after write" << endl;
        cout << contents << endl;
        ::remove(junkFile.c_str());
    }

    if (options.filename == "empty.toml") {
        const char* STR_THING =
        "{\n"
        "\t{ \"thing1\": \"Value 1 with spaces\" }\n"
        "\t{ \"thing2\": \"Value 2 with spaces\" }\n"
        "}";

        string_q str = STR_THING;
        cout << str << endl;
        cout << str << endl;
    }

    return 0;
}
