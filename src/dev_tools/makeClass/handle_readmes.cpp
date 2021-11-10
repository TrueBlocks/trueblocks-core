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
string_q get_usage(const string_q& route) {
    return "```[plaintext]\n" + doCommand2("chifra " + route + " --help") + "\n```";
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

    for (auto ep : endpointArray) {
        if (!ep.api_route.empty()) {
            string_q justTool = ep.tool;
            justTool = nextTokenClear(justTool, ' ');

            string_q grp = substitute(toLower(ep.group), " ", "");
            string_q docFn = grp + "-" + ep.api_route + ".md";

            string_q dSource = getDocsPathTemplates("readme-intros/" + docFn);

            string_q contents = asciiFileToString(dSource);
            replaceAll(contents, "[{NAME}]", "chifra " + ep.api_route);
            replaceAll(contents, "[{USAGE}]", get_usage(ep.api_route));

            string_q dFooter, sFooter;
            if (!goPortNewCode(justTool)) {
                dFooter = "\n**Source code**: [`[{GROUP}]/[{TOOL}]`]([{LOCATION}][{GROUP}]/[{TOOL}])\n";
                replaceAll(dFooter, "[{GROUP}]", ep.api_group);
                replaceAll(dFooter, "[{TOOL}]", justTool);
                replaceAll(dFooter, "[{LOCATION}]", "https://github.com/TrueBlocks/trueblocks-core/tree/master/src/");
                sFooter = "\n" + trim(asciiFileToString(getDocsPathTemplates("readme-intros/README.footer.md")), '\n');
            } else {
                dFooter = "\n**Source code**: [`[{FILE}]`]([{LOCATION}]apps/chifra/cmd/[{FILE}])\n";
                replaceAll(dFooter, "[{FILE}]", ep.api_route + ".go");
                replaceAll(dFooter, "[{LOCATION}]", "https://github.com/TrueBlocks/trueblocks-core/tree/develop/src/");
            }

            string_q dContents = substitute(contents, "[{FOOTER}]", dFooter);
            string_q dReadme = getDocsPathReadmes(docFn);
            writeIfDifferent(dReadme, dContents);
            if (ep.is_visible_docs && !goPortNewCode(justTool)) {
                string_q sContents = substitute(contents, "[{FOOTER}]", sFooter);
                string_q tReadme = substitute(getSourcePath(ep.api_group + "/" + justTool + "/README.md"), "//", "/");
                writeIfDifferent(tReadme, sContents);
            }
        }
    }

    // TODO: this should be generated from the data
    CStringArray items = {"Accounts:list,export,monitors,names,abis",
                          "Chain Data:blocks,trans,receipts,logs,traces,when", "Chain State:state,result,tokens",
                          "Admin:cacheStatus,serve,scrape,chunks,init,pins", "Other:quotes,explore,ethslurp"};
    uint32_t weight = 1100;
    for (auto item : items) {
        CStringArray parts;
        explode(parts, item, ':');
        string_q group = parts[0];
        string_q tool = parts[1];

        string_q front = STR_YAML_FRONTMATTER;
        replace(front, "[{TITLE}]", firstUpper(toLower(group)));
        replace(front, "[{WEIGHT}]", uint_2_Str(weight));
        replace(front, "[{M1}]", "docs:");
        replace(front, "[{M2}]", "parent: \"chifra\"");
        group = substitute(toLower(group), " ", "");

        ostringstream os;
        os << front;
        os << asciiFileToString(getDocsPathTemplates("readme-groups/" + group + ".md"));

        CStringArray paths;
        explode(paths, tool, ',');
        for (auto p : paths) {
            string_q pp = getDocsPathReadmes(group + "-" + p + ".md");
            os << asciiFileToString(pp);
        }

        string_q outFn = getDocsPathContent("docs/chifra/" + group + ".md");
        // cerr << "Out: " << fileExists(outFn) << endl;
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
