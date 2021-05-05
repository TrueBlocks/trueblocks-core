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
#include "utillib.h"

//--------------------------------------------------------------
int main(int argc, const char* argv[]) {
    CDefaultOptions options;
    options.minArgs = 0;
    if (!options.prepareArguments(argc, argv))
        return 0;

    size_t recurse = (argc == 2 && contains(string_q(argv[1]), "recurse"));
    size_t first = (argc == 2 && contains(string_q(argv[1]), "first"));
    size_t last = (argc == 2 && contains(string_q(argv[1]), "last"));
    size_t copy = (argc == 2 && contains(string_q(argv[1]), "copy"));
    size_t rm = (argc == 2 && contains(string_q(argv[1]), "rm"));
    size_t newest = (argc == 2 && contains(string_q(argv[1]), "newest"));

    string_q newFilename = "./new.txt";
    string_q oldFilename = "./old.txt";
    string_q nwsFilename = "./new with spaces.txt";

    if (last) {
        string_q lastFile = getLastFileInFolder("../", recurse);
        cout << "Last file in folder: " << lastFile << endl;

    } else if (first) {
        string_q firstFile = getFirstFileInFolder("../", recurse);
        cout << "First file in folder: " << firstFile << endl;

    } else if (copy) {
        stringToAsciiFile(oldFilename, "This is the string");
        copyFile(oldFilename, newFilename);
        copyFile(oldFilename, nwsFilename);
        cout << "old file: " << asciiFileToString(oldFilename) << " - " << fileExists(oldFilename) << endl;
        cout << "new file: " << asciiFileToString(newFilename) << " - " << fileExists(newFilename) << endl;
        cout << "n w s file: " << asciiFileToString(nwsFilename) << " - " << fileExists(nwsFilename) << endl;

    } else if (rm) {
        ::remove(oldFilename.c_str());
        ::remove(newFilename.c_str());
        ::remove(nwsFilename.c_str());
        cout << "old file: " << asciiFileToString(oldFilename) << " - " << fileExists(oldFilename) << endl;
        cout << "new file: " << asciiFileToString(newFilename) << " - " << fileExists(newFilename) << endl;
        cout << "n w s file: " << asciiFileToString(nwsFilename) << " - " << fileExists(nwsFilename) << endl;

    } else if (newest) {
        stringToAsciiFile(oldFilename, "oldText");
        sleep(1.);
        stringToAsciiFile(newFilename, "oldText");
        fileInfo info = getNewestFileInFolder("./");
        cout << "old file: " << asciiFileToString(oldFilename) << " - " << fileExists(oldFilename) << endl;
        cout << "new file: " << asciiFileToString(newFilename) << " - " << fileExists(newFilename) << endl;
        cout << info.fileName << endl;
        sleep(1.);
        stringToAsciiFile("./old.txt", "oldText");
        info = getNewestFileInFolder("./");
        cout << "old file: " << asciiFileToString(oldFilename) << " - " << fileExists(oldFilename) << endl;
        cout << "new file: " << asciiFileToString(newFilename) << " - " << fileExists(newFilename) << endl;
        cout << info.fileName << endl;

    } else {
        string_q tests[] = {"Non-recursive", "Recursive"};
        bool vals[] = {false, true};

        cout << tests[recurse] << " from ../" << endl;
        CStringArray files;
        size_t found = listFilesInFolder(files, "../", vals[recurse]);
        cout << "Found " << found << " (" << files.size() << ") files" << endl;
        for (auto file : files)
            cout << file << endl;
    }

    return 0;
}
