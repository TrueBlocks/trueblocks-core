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

extern bool writeIfDifferent(const string_q& path, const string_q& code);
//------------------------------------------------------------------------------------------------------------
bool visitReadme(const string_q& templatePath, void* data) {
    if (endsWith(templatePath, "/")) {
        return forEveryFileInFolder(templatePath + "*", visitReadme, data);

    } else {
        if (!endsWith(templatePath, ".md") || contains(templatePath, "README"))
            return true;

        COptions* opts = (COptions*)data;

        CStringArray parts;
        explode(parts, templatePath, '/');
        string_q folder = parts[3];
        string_q tool = substitute(parts[4], ".md", "");
        string_q docPath = "../docs/readmes/" + folder + "/" + tool + "/README.md";
        string_q srcPath = "../src/" + folder + "/" + tool + "/README.md";

        string_q source = asciiFileToString(templatePath);
        if (system((tool + " -th >file 2>&1").c_str())) {
        }  // Don't remove cruft. Silences compiler warnings
        string_q usage = trim(asciiFileToString("./file"), '\n');
        ::remove("./file");
        replace(source, "[{USAGE_TABLE}]", usage);

        string_q docCode = source;
        string_q srcCode = source;
        for (auto rep : opts->counter.replacements) {
            CStringArray parts2;
            explode(parts2, rep, '/');
            string_q token = substitute(substitute(parts2[parts2.size() - 1], "README.", ""), ".md", "");
            string_q code = trim(asciiFileToString(rep), '\n');
            if (token != "footer")
                replace(docCode, "[{" + toUpper(token) + "}]", "\n" + code);
            else
                replace(docCode, "[{" + toUpper(token) + "}]\n", "");
            replace(srcCode, "[{" + toUpper(token) + "}]", "\n" + code);
        }
        replaceAll(docCode, "[{TOOL_PATH}]", folder + "/" + tool);
        replaceAll(srcCode, "[{TOOL_PATH}]", folder + "/" + tool);
        replaceAll(docCode, "~/Library/Application Support/TrueBlocks/", "$CONFIG/");
        replaceAll(srcCode, "~/Library/Application Support/TrueBlocks/", "$CONFIG/");
        replaceAll(docCode, "[{NAME}]", progNameMap[tool].empty() ? opts->getProgName() : progNameMap[tool]);
        replaceAll(srcCode, "[{NAME}]", progNameMap[tool].empty() ? opts->getProgName() : progNameMap[tool]);

        bool c1 = writeIfDifferent(docPath, docCode + "\n");
        bool c2 = writeIfDifferent(srcPath, srcCode + "\n");
        opts->counter.nVisited++;
        opts->counter.nProcessed += (c1 || c2);
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& path, const string_q& code) {
    string_q existing = asciiFileToString(path);
    if (existing != code) {
        stringToAsciiFile(path, code);
        cerr << cGreen << "\tProcessed " << cOff << path << endl;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool findReplacements(const string_q& templatePath, void* data) {
    if (endsWith(templatePath, "/")) {
        return forEveryFileInFolder(templatePath + "*", findReplacements, data);

    } else {
        if (contains(templatePath, "README.") && endsWith(templatePath, ".md") &&
            !contains(templatePath, "README.md")) {
            COptions* opts = (COptions*)data;
            opts->counter.replacements.push_back(templatePath);
        }
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_readmes(void) {
    CToml config(configPath("makeClass.toml"));
    bool enabled = config.getConfigBool("enabled", "readmes", false);
    if (!enabled) {
        LOG_WARN("Skipping readmes...");
        return true;
    }

    LOG_INFO(cYellow, "handling readmes...", cOff);
    counter = CCounter();  // reset
    if (!test) {
        forEveryFileInFolder("../docs/readme_templates", findReplacements, this);
        forEveryFileInFolder("../docs/readme_templates", visitReadme, this);
    }
    LOG_INFO(cYellow, "makeClass --readmes", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));
    return true;
}
