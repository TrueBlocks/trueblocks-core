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
    return "```[plaintext]\n" + doCommand("chifra " + route + " --help", true /* stderr */) + "\n```";
}

extern const char* STR_CONFIG;
//------------------------------------------------------------------------------------------------------------
string_q get_config_usage(const CCommandOption& ep) {
    string_q docFn = getDocsPathReadmes(substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".config");
    return substitute(STR_CONFIG, "[{CONFIGS}]", asciiFileToString(docFn));
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_readmes(void) {
    CToml config(rootConfigToml_makeClass);
    bool enabled = config.getConfigBool("enabled", "readmes", false);
    if (!enabled) {
        LOG_WARN("Skipping readmes...");
        return true;
    }

    LOG_INFO(cYellow, "handling readmes...", cOff);

    map<string_q, string_q> items;
    map<string_q, uint64_t> weights;
    uint32_t weight = 1100;
    for (auto ep : endpointArray) {
        if (!ep.api_route.empty()) {
            if (weights[ep.group] == 0) {
                weights[ep.group] = weight;
                weight += 200;
            }
            items[ep.group] += ep.api_route + ",";
            // cerr << "X: " << ep.group << " " << ep.api_route << " " << ep.tool << endl;
            string_q justTool = ep.tool;
            justTool = nextTokenClear(justTool, ' ');

            string_q docFn = substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".md";
            string_q dSource = getDocsPathTemplates("readme-intros/" + docFn);
            string_q contents = asciiFileToString(dSource);

            replaceAll(contents, "[{NAME}]", "chifra " + ep.api_route);
            replaceAll(contents, "[{USAGE}]", get_usage(ep.api_route));
            replaceAll(contents, "[{CONFIG}]", get_config_usage(ep));

            string_q dFooter;
            dFooter =
                "\n**Source code**: "
                "[`[{FILE}]`](https://github.com/TrueBlocks/trueblocks-core/tree/master/src/apps/chifra/"
                "[{FILE}])\n";
            replaceAll(dFooter, "[{FILE}]", "internal/" + ep.api_route);
            string_q dReadme = getDocsPathReadmes(docFn);
            string_q dContents = substitute(contents, "[{FOOTER}]", dFooter);
            writeIfDifferent(dReadme, dContents);

            string_q sFooter;
            sFooter = "\n" + trim(asciiFileToString(getDocsPathTemplates("readme-intros/README.footer.md")), '\n');
            if (ep.is_visible_docs) {
                string_q sContents = substitute(contents, "[{FOOTER}]", sFooter);
                string_q tReadme = getPathToSource("apps/chifra/internal/" + ep.api_route + "/README.md");
                replaceAll(tReadme, "//", "/");
                writeIfDifferent(tReadme, sContents);
            }
        }
    }

    for (auto item : items) {
        string_q group = item.first;
        string_q tool = item.second;

        string_q front = STR_YAML_FRONTMATTER;
        replace(front, "[{TITLE}]", firstUpper(toLower(group)));
        replace(front, "[{WEIGHT}]", uint_2_Str(weights[group]));
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
            cerr << group << "-" << pp << ": " << fileExists(pp) << endl;
            os << asciiFileToString(pp);
        }

        string_q outFn = getDocsPathContent("docs/chifra/" + group + ".md");
        writeIfDifferent(outFn, os.str(), Now());
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

const char* STR_CONFIG =
    "\n"
    "\n"
    "### configuration\n"
    "\n"
    "Each of the following additional configurable command line options are available.\n"
    "\n"
    "**Configuration file:** `$CONFIG/$CHAIN/blockScrape.toml`  \n"
    "**Configuration group:** `[settings]`  \n"
    "\n"
    "[{CONFIGS}]\n"
    "\n"
    "These items may be set in three ways, each overridding the preceeding method:\n"
    "\n"
    "-- in the above configuration file under the `[settings]` group,  \n"
    "-- in the environment by exporting the configuration item as UPPER&lowbar;CASE, without "
    "underbars, and prepended with TB_SETTINGS&lowbar;, or  \n"
    "-- on the command line using the configuration item with leading dashes (i.e., `--name`).  ";