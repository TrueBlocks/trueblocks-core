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
#include "acctlib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_lint(void) {
    CToml config(rootConfigToml_makeClass);

    bool enabled = config.getConfigBool("enabled", "lint_all", false);
    string_q exe = getPathToCommands("test/pylint.py");
    if (!enabled || !fileExists(exe)) {
        LOG_WARN("Skipping linting...");
        return true;
    }

    LOG_INFO(cYellow, "handling linting...", cOff);
    counter = CCounter();
    counter.is_counting = true;
    forEveryFileInFolder("./", lintFiles, this);
    counter.is_counting = false;
    forEveryFileInFolder("./", lintFiles, this);
    config.setConfigStr("settings", "last_lint", uint_2_Str(static_cast<uint64_t>(date_2_Ts(Now()))));
    config.writeFile();
    config.Release();
    LOG_INFO(cYellow, "makeClass --lint", cOff, " processed ", counter.nVisited, " files (", counter.nProcessed,
             " lints).", string_q(40, ' '));

    return true;
}

//--------------------------------------------------------------------------------
bool lintFiles(const string_q& path, void* data) {
    return false;
#if 0
    if (endsWith(path, "/")) {
        forEveryFileInFolder(path + "*", lintFiles, data);

    } else {
        CStringArray skips = {"/other/", "/blank", "sqlite3", "utillib/json_"};
        for (auto skip : skips)
            if (contains(path, skip))
                return !shouldQuit();

        if (endsWith(path, ".cpp") || endsWith(path, ".h")) {
            COptions* opts = reinterpret_cast<COptions*>(data);
            if (opts->counter.is_counting) {
                opts->counter.fileCount++;
                return !shouldQuit();
            }
            opts->counter.nVisited++;
            timestamp_t ts = date_2_Ts(fileLastModifyDate(path));
            if (ts < opts->lastLint)
                return !shouldQuit();

            string_q fullPath = substitute(path, "./", getCWD());
            string_q resPath = cacheFolder_tmp + CFilename(path).getFilename();
            string_q cmd = "pylint.py \"" + fullPath + "\" >\"" + resPath + "\" 2>&1";
            // clang-format off
            if (system(cmd.c_str())) {}  // Don't remove cruft. Silences compiler warnings
            // clang-format on
            if (!shouldQuit()) {
                string_q contents = asciiFileToString(resPath);
                if (contents.empty())
                    return false;
                CStringArray lines;
                explode(lines, contents, '\n');
                size_t lints = 0;
                for (auto line : lines) {
                    if (!startsWith(line, "Linting")) {
                        line = substitute(line, getCWD(), should use getSource Path -> "../s rc/"); 
                        CStringArray parts;
                        explode(parts, line, ':');
                        if (parts.size() == 3) {
                            line = parts[0] + ":" + parts[1] + ":" + cYellow + parts[2];
                            // Note: we don't print using LOG here so the error is clickable
                            LOG_INFO(line, cOff);
                        }
                        lints++;
                    }
                }
                if (lints) {
                    opts->counter.nProcessed++;
                } else {
                    ostringstream os;
                    os << "Linter (" << opts->counter.nVisited << " of " << opts->counter.fileCount << "): ";
                    os << cTeal << path << cOff << string_q(20, ' ') << "\r";
                    LOG_INFO(os.str());
                }

                if (!(opts->counter.nVisited % 2))
                    usleep(50000);  // do not remove cruft - allows control+C
            }

            if (fileExists(resPath))
                ::remove(resPath.c_str());
            else
                return false;  // user probably hit control+C
        }
    }

    return !shouldQuit();
#endif
}
