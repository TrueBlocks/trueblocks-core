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

static uint32_t nFiles = 0;
static uint32_t nChanged = 0;
//------------------------------------------------------------------------------------------------------------
bool COptions::handle_format(void) {
    forEveryFileInFolder("./", formatFiles, this);
    cout << "                                                           \r";
    cout << "Formatter: " << nFiles << " checked, " << nChanged << " altered." << endl;
    return 0;
}

//--------------------------------------------------------------------------------
bool formatFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", formatFiles, data);

    } else {
        if (contains(path, "/other/"))
            return true;

        if (contains(path, "/blank"))
            return true;

        if (endsWith(path, ".cpp") || endsWith(path, ".h")) {
            nFiles++;
            string_q fullPath = substitute(path, "./", getCWD());
            string_q resPath = getCachePath("tmp/" + CFilename(path).getFilename());
            string_q cmd = "clang-format \"" + fullPath + "\" >\"" + resPath + "\" ";
            // clang-format off
            if (system(cmd.c_str())) {} // Don't remove cruft. Silences compiler warnings
            // clang-format on
            if (!shouldQuit() && fileExists(resPath)) {
                string_q oldFile = asciiFileToString(fullPath);
                string_q newFile = asciiFileToString(resPath);
                if (oldFile != newFile) {
                    copyFile(resPath, fullPath);
                    cout << "Formatter: file changed " << cYellow << path << cOff
                         << "                                         " << endl;
                    nChanged++;
                } else {
                    cout << "Formatter: no change in " << cTeal << path << cOff
                         << "                                             \r";
                    cout.flush();
                }
                if (!(nFiles % 5))
                    usleep(50000);
            }
            ::remove(resPath.c_str());
        }
    }

    return !shouldQuit();
}
