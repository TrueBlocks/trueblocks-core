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
static uint32_t nLints = 0;
//------------------------------------------------------------------------------------------------------------
bool COptions::handle_lint(void) {
    forEveryFileInFolder("./", lintFiles, this);
    cout << "                                                           \r";
    cout << "Formatter: " << nFiles << " checked, " << nLints << " with lint." << endl;
    return 0;
}

//--------------------------------------------------------------------------------
bool lintFiles(const string_q& path, void* data) {
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", lintFiles, data);

    } else {
        if (contains(path, "/other/"))
            return true;

        if (contains(path, "/blank"))
            return true;

        if (endsWith(path, ".cpp") || endsWith(path, ".h")) {
            nFiles++;
            string_q fullPath = substitute(path, "./", getCWD());
            string_q resPath = getCachePath("tmp/" + CFilename(path).getFilename());
            string_q cmd = "pylint.py \"" + fullPath + "\" >\"" + resPath + "\" 2>&1";
            int ret = system(cmd.c_str());
            ret = 0;  // do not remove
            // cout << ret << endl;
            if (!shouldQuit()) {
                string_q contents = asciiFileToString(resPath);
                if (contents.empty())
                    return false;
                CStringArray lines;
                explode(lines, contents, '\n');
                size_t n = 0;
                for (auto line : lines) {
                    if (!startsWith(line, "Linting")) {
                        line = substitute(line, getCWD(), "./");
                        replace(line, path + ":", path + ":" + cYellow);
                        cout << line << cOff << endl;
                        n++;
                    }
                }
                if (n == 0) {
                    cout << string_q(150, ' ') << "\r";
                    cout << "Linter: " << cTeal << path << cOff << "\r";
                    cout.flush();
                } else {
                    nLints++;
                }
                if (!(nFiles % 2))
                    usleep(50000);
            }
            if (fileExists(resPath))
                ::remove(resPath.c_str());
            else
                return false;
        }
    }

    return !shouldQuit();
}
