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

// //------------------------------------------------------------------------------------------------------------
// bool writeReadmes(const string_q& templatePath, void* data) {
//     if (endsWith(templatePath, "/")) {
//         return forEveryFileInFolder(templatePath + "*", writeReadmes, data);

//     } else {
//         if (!endsWith(templatePath, ".md") || contains(templatePath, "README"))
//             return true;

//         COptions* opts = (COptions*)data;

//         CStringArray parts;
//         explode(parts, templatePath, '/');
//         string_q folder = parts[4];
//         string_q tool = substitute(parts[5], ".md", "");
//         string_q toolPath = folder + "/" + tool;

//         string_q tempCode = asciiFileToString(templatePath);
//         replaceAll(tempCode, "[{NAME}]", path_2_Cmd(tool));
//         replaceAll(tempCode, "~/Library/Application Support/TrueBlocks/", "$CONFIG/");

//         string_q cmd = tool;
//         if (tool != "chifra" && tool != "makeClass" && tool != "testRunner")
//             cmd = getCommandPath(tool);
//         if (system((cmd + " --readme >file 2>&1").c_str())) {
//         }  // Don't remove cruft. Silences compiler warnings
//         string_q usage = trim(asciiFileToString("./file"), '\n');
//         ::remove("./file");

//         replace(tempCode, "[{USAGE}]", usage);
//         string_q srcCode = tempCode;
//         replaceAll(srcCode, "{{<td>}}\n", "");
//         replaceAll(srcCode, "{{</td>}}\n", "");

//         string_q docCode = substitute(tempCode, "## Usage", "## usage");
//         replaceAll(docCode, "]  \n", "]\n");
//         replaceAll(docCode, ":`  \n", ":`\n");
//         replaceAll(docCode, ": \n", ":\n");
//         replaceAll(docCode, "\n`Purpose", "  \n`Purpose");

//         string_q tail =
//             "\n**Source code**: "
//             "[`[{TOOL_PATH}]`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/[{TOOL_PATH}])\n";
//         replaceAll(tail, "[{TOOL_PATH}]", toolPath);
//         replaceAll(tail, "[{SOURCE_LOCATION}]", "https://github.com/TrueBlocks/trueblocks-core/tree/master/src/");

//         string_q footer = "\n" + trim(asciiFileToString(getDocsPathTemplates("readme-intros/README.footer.md")),
//         '\n'); replaceAll(docCode, "[{FOOTER}]", tail); replaceAll(srcCode, "[{FOOTER}]", footer);

//         string_q docPath = getDocsPathReadmes(toolPath + "/README.md");
//         string_q srcPath = getSourcePath(toolPath + "/README.md");
//         if (!contains(docPath, "chifra")) {
//             bool c1 = writeIfDifferent(docPath, docCode);
//             bool c2 = writeIfDifferent(srcPath, srcCode);
//             opts->counter.nProcessed += (c1 || c2);
//         }

//         opts->counter.nVisited++;
//     }
//     return true;
// }

//------------------------------------------------------------------------------------------------------------
string_q get_usage(const string_q& route) {
    string_q ret = doCommand2("chifra " + route + " --help");
    if (contains(ret, "/"))
        return ret;
    return "";
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_readmes(void) {
    CToml config(getConfigPath("makeClass.toml"));
    bool enabled = config.getConfigBool("enabled", "readmes", false);
    if (!enabled) {
        LOG_WARN("Skipping readmes...");
        return true;
    }

    LOG_INFO(cYellow, "handling readmes...", cOff);
#if 1
    for (auto ep : endpointArray) {
        if (!ep.api_route.empty()) {
            string_q justTool = ep.tool;
            justTool = nextTokenClear(justTool, ' ');

            cerr << string_q(120, '=') << endl;
            cerr << ep << endl;

            cerr << string_q(120, '-') << endl;
            cerr << get_usage(ep.api_route) << endl;

            string_q grp = substitute(toLower(ep.group), " ", "");
            string_q docFn = grp + "-" + ep.api_route + ".md";

            cerr << string_q(120, '=') << endl;
            string_q dSource = getDocsPathTemplates("readme-intros/" + docFn);
            cerr << "d-INTRO:  " << fileExists(dSource) << "-0: " << dSource << endl;

            string_q dReadme = getDocsPathReadmes(docFn);
            cerr << "d-README: " << fileExists(dReadme) << "-1: " << dReadme << endl;

            string_q tReadme = substitute(getSourcePath(ep.api_group + "/" + justTool + "/README.md"), "//", "/");
            cerr << "t-README: " << fileExists(tReadme) << "-" << ep.is_visible_docs << ": " << tReadme << endl;
            getchar();
        }
    }
    return true;
#endif

    // #if 1
    //     counter = CCounter();  // reset
    //     forEveryFileInFolder(getDocsPathTemplates("readme-intros/"), writeReadmes, this);

    //     CStringArray items = {
    //         "Accounts:apps/list,apps/acctExport,apps/monitors,tools/ethNames,tools/grabABI",
    //         "Chain
    //         Data:tools/getBlocks,tools/getTrans,tools/getReceipts,tools/getLogs,tools/getTraces,tools/whenBlock",
    //         "Chain State:tools/getState,tools/getTokens",
    //         "Admin:apps/cacheStatus,apps/serve,apps/blockScrape,apps/init,apps/pinMan",
    //         "Other:tools/getQuotes,apps/fireStorm,tools/ethslurp"};
    //     uint32_t weight = 1100;
    //     for (auto item : items) {
    //         CStringArray parts;
    //         explode(parts, item, ':');
    //         string_q tool = parts[1];
    //         string_q group = parts[0];

    //         string_q front = STR_YAML_FRONTMATTER;
    //         replace(front, "[{TITLE}]", group);
    //         replace(front, "[{WEIGHT}]", uint_2_Str(weight));
    //         replace(front, "[{M1}]", "docs:");
    //         replace(front, "[{M2}]", "parent: \"chifra\"");
    //         string fn = substitute(toLower(group), " ", "");

    //         ostringstream os;
    //         os << front;
    //         os << asciiFileToString(getDocsPathTemplates("readme-groups/" + fn + ".md"));

    //         CStringArray paths;
    //         explode(paths, tool, ',');
    //         for (auto p : paths) {
    //             string_q pp = getDocsPathReadmes(p + "/README.md");
    //             os << asciiFileToString(pp);
    //         }

    //         string_q outFn = getDocsPathContent("docs/chifra/" + fn + ".md");
    //         writeIfDifferent(outFn, os.str(), Now());

    //         weight += 200;
    //     }

    //     LOG_INFO(cYellow, "makeClass --readmes", cOff, " processed ", counter.nVisited, " files (changed ",
    //              counter.nProcessed, ").", string_q(40, ' '));
    // #endif
    //     return true;
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
