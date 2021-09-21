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
bool visitReadme(const string_q& templatePath, void* data) {
    if (endsWith(templatePath, "/")) {
        return forEveryFileInFolder(templatePath + "*", visitReadme, data);

    } else {
        if (!endsWith(templatePath, ".md") || contains(templatePath, "README"))
            return true;

        COptions* opts = (COptions*)data;

        CStringArray parts;
        explode(parts, templatePath, '/');
        string_q folder = parts[4];
        string_q tool = substitute(parts[5], ".md", "");
        string_q docPath = getDocsPathReadmes(folder + "/" + tool + "/README.md");
        string_q srcPath = "../src/" + folder + "/" + tool + "/README.md";

        string_q source = asciiFileToString(templatePath);
        string_q cmd = tool;
        if (tool != "chifra" && tool != "makeClass" && tool != "testRunner")
            cmd = getCommandPath(tool);
        if (system((cmd + " -th >file 2>&1").c_str())) {
        }  // Don't remove cruft. Silences compiler warnings
        string_q usage = trim(asciiFileToString("./file"), '\n');
        ::remove("./file");
        replace(source, "[{USAGE_TABLE}]", usage);

        string_q docCode = substitute(source, "## Usage", "## usage");
        replaceAll(docCode, "]  \n", "]\n");
        replaceAll(docCode, ":`  \n", ":`\n");
        replaceAll(docCode, ": \n", ":\n");
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
        replaceAll(docCode, "\n`Purpose", "  \n`Purpose");

        bool c1 = writeIfDifferent(docPath, docCode + "\n");
        bool c2 =
            writeIfDifferent(srcPath, substitute(substitute(srcCode, "{{<td>}}\n", ""), "{{</td>}}\n", "") + "\n");
        opts->counter.nVisited++;
        opts->counter.nProcessed += (c1 || c2);
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& outFn, const string_q& codeIn, const time_q& now) {
    static uint32_t cnt = 1;
    string_q code = substitute(codeIn, "date: $DATE\n", "");
    stringToAsciiFile("../build/one/" + uint_2_Str(cnt) + ".txt", code);
    string_q existingIn = asciiFileToString(outFn);
    CStringArray lines;
    explode(lines, existingIn, '\n', false);
    ostringstream existing;
    for (auto line : lines) {
        if (!startsWith(line, "date: ")) {
            existing << line << endl;
        }
    }
    stringToAsciiFile("../build/two/" + uint_2_Str(cnt) + ".txt", existing.str());
    cnt++;

    if (existing.str() != code) {
        string_q out = substitute(codeIn, "$DATE", now.Format(FMT_EXPORT));
        stringToAsciiFile(outFn, out);
        cerr << cGreen << "\tProcessed " << cOff << outFn << endl;
        return true;
    }
    return false;
}

//------------------------------------------------------------------------------------------------------------
bool writeIfDifferent(const string_q& outFn, const string_q& code) {
    string_q existing = asciiFileToString(outFn);
    if (existing != code) {
        stringToAsciiFile(outFn, code);
        cerr << cGreen << "\tProcessed " << cOff << outFn << endl;
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
    forEveryFileInFolder(getDocsPathTemplates("readme-intros/"), findReplacements, this);
    forEveryFileInFolder(getDocsPathTemplates("readme-intros/"), visitReadme, this);

    CStringArray items = {
        "Accounts:apps/list,apps/acctExport,apps/monitors,tools/ethNames,tools/grabABI",
        "Chain Data:tools/getBlocks,tools/getTrans,tools/getReceipts,tools/getLogs,tools/getTraces,tools/whenBlock",
        "Chain State:tools/getState,tools/getTokens",
        "Admin:apps/cacheStatus,apps/serve,apps/blockScrape,apps/init,apps/pinMan",
        "Other:tools/getQuotes,apps/fireStorm,tools/ethslurp"};
    uint32_t weight = 1100;
    for (auto item : items) {
        CStringArray parts;
        explode(parts, item, ':');

        string_q front = STR_YAML_FRONTMATTER;
        replace(front, "[{TITLE}]", parts[0]);
        replace(front, "[{WEIGHT}]", uint_2_Str(weight));
        replace(front, "[{M1}]", "docs:");
        replace(front, "[{M2}]", "parent: \"chifra\"");
        string fn = substitute(toLower(parts[0]), " ", "");

        ostringstream os;
        os << front;
        os << asciiFileToString(getDocsPathTemplates("readme-groups/" + fn + ".md"));

        CStringArray paths;
        explode(paths, parts[1], ',');
        for (auto p : paths) {
            string_q pp = getDocsPathReadmes(p + "/README.md");
            os << asciiFileToString(pp);
        }

        string_q outFn = getDocsPathContent("docs/chifra/" + fn + ".md");
        writeIfDifferent(outFn, os.str(), Now());

        weight += 200;
    }

    LOG_INFO(cYellow, "makeClass --readmes", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));
    return true;
}

//------------------------------------------------------------------------------------------------------------
const char* STR_YAML_FRONTMATTER =
    "---\n"
    "title: \"[{TITLE}]\"\n"
    "description: \"\"\n"
    "lead: \"\"\n"
    "date: $DATE\n"
    "lastmod:\n"
    "  - :git\n"
    "  - lastmod\n"
    "  - date\n"
    "  - publishDate\n"
    "draft: false\n"
    "images: []\n"
    "menu:\n"
    "  [{M1}]\n"
    "    [{M2}]\n"
    "weight: [{WEIGHT}]\n"
    "toc: true\n"
    "---\n";
