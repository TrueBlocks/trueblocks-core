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

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_format(void) {
    CToml config(configPath("makeClass.toml"));

    bool enabled = config.getConfigBool("settings", "enabled", false);
    string_q res = doCommand("which clang-format");
    if (!enabled || res.empty()) {
        LOG_WARN("Skipping formatting...");
        return true;
    }

    LOG_INFO(cYellow, "handling formatting...", cOff);
    counter = CCounter();
    counter.is_counting = true;
    forEveryFileInFolder("./", formatFiles, this);
    counter.is_counting = false;
    forEveryFileInFolder("./", formatFiles, this);
    LOG_INFO(cYellow, "makeClass --format", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    config.setConfigStr("settings", "lastFormat", uint_2_Str(static_cast<uint64_t>(date_2_Ts(Now()))));
    config.writeFile();
    config.Release();
    return true;
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
            COptions* opts = reinterpret_cast<COptions*>(data);
            if (opts->counter.is_counting) {
                opts->counter.fileCount++;
                return true;
            }
            opts->counter.nVisited++;
            timestamp_t ts = date_2_Ts(fileLastModifyDate(path));
            if (ts < opts->lastFormat)
                return true;

            string_q fullPath = substitute(path, "./", getCWD());
            establishFolder(getCachePath("tmp/"));
            string_q resPath = getCachePath("tmp/" + CFilename(path).getFilename());
            string_q cmd = "clang-format \"" + fullPath + "\" >\"" + resPath + "\" ";
            // clang-format off
            if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
            // clang-format on
            if (!shouldQuit() && fileExists(resPath)) {
                string_q oldFile = asciiFileToString(fullPath);
                string_q newFile = asciiFileToString(resPath);
                if (oldFile != newFile) {
                    opts->counter.nProcessed++;
                    copyFile(resPath, fullPath);
                    ostringstream os;
                    os << "Formatting: ";
                    os << cTeal << path << cOff << string_q(50, ' ');
                    LOG_INFO(os.str());

                } else {
                    ostringstream os;
                    os << "Formatter (" << opts->counter.nVisited << " of " << opts->counter.fileCount
                       << "): no change ";
                    os << cTeal << path << cOff << string_q(20, ' ') << "\r";
                    LOG_INFO(os.str());
                }
                if (!(opts->counter.nVisited % 5))
                    usleep(50000);  // do not remove cruft - allows control+C
            }
            ::remove(resPath.c_str());
        }
    }

    return !shouldQuit();
}
