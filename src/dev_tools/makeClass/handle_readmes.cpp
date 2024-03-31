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
#include "options.h"

//------------------------------------------------------------------------------------------------------------
string_q get_usage(const string_q& route) {
    string_q plainText = doCommand("chifra " + route + " --help", true /* stderr */);
    return "```[plaintext]\n" + plainText + "\n```";
}

extern const char* STR_CONFIG;
extern const char* STR_README_BEGPARTS;
extern const char* STR_README_ENDPARTS;
//------------------------------------------------------------------------------------------------------------
string_q get_config_usage(const CCommandOption& ep) {
    string_q n = "readme-intros/" + substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".config.md";
    string_q docFn = "../templates/"+n;
    return fileExists(docFn) ? substitute(STR_CONFIG, "[{CONFIGS}]", asciiFileToString(docFn)) : "";
}

//------------------------------------------------------------------------------------------------------------
string_q get_readme_notes(const CCommandOption& ep) {
    string_q n = "readme-intros/" + substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".notes.md";
    string_q docFn = "../templates/"+n;
    return fileExists(docFn) ? "\n\n" + trim(asciiFileToString(docFn), '\n') : "";
}

//------------------------------------------------------------------------------------------------------------
string_q get_links(const CCommandOption& ep) {
    string_q ret =
        "\n"
        "\n"
        "Links:\n"
        "\n"
        "- [api docs](/api/#operation/[{GROUP}]-[{ROUTE}])\n"
        "- [source code]([{SOURCEURL}]/[{ROUTE}])\n"
        "- [tests]([{TESTURL}]/[{API_GROUP}]/[{TOOL}].csv)";

    // cout << "X: " << ep.api_route << endl;
    if (ep.api_route == "daemon" || ep.api_route == "explore") {
        // cout << "X: 1: " << ret << endl;
        replace(ret, "- [api docs](/api/#operation/[{GROUP}]-[{ROUTE}])", "- no api for this command");
        // cout << "X: 2: " << ret << endl;
        if (ep.api_route == "daemon") {
            // cout << "X: 3: " << ret << endl;
            replace(ret, "- [tests]([{TESTURL}]/[{API_GROUP}]/[{TOOL}].csv)", "- no tests for this command");
        }
        // cout << "X: 4: " << ret << endl;
    }

    string_q gitUrl = "https://github.com/TrueBlocks/trueblocks-core/tree/master/src/";
    string_q sourceUrl = gitUrl + "apps/chifra/internal";
    string_q testUrl = gitUrl + "dev_tools/testRunner/testCases";
    replace(ret, "[{TESTURL}]", testUrl);
    replace(ret, "[{SOURCEURL}]", sourceUrl);

    replaceAll(ret, "[{GROUP}]", toLower(substitute(ep.group, " ", "")));
    replaceAll(ret, "[{API_GROUP}]", toLower(substitute(ep.api_group, " ", "")));
    replaceAll(ret, "[{ROUTE}]", ep.api_route);
    replaceAll(ret, "[{TOOL}]", substitute(ep.tool, " --appearances", ""));

    return ret;
}

//------------------------------------------------------------------------------------------------------------
void get_models(const CClassDefinitionArray& models, CStringArray& result, const string_q& route) {
    ostringstream os;
    for (auto model : models) {
        if (contains(model.doc_producer, toLower(route))) {
            string_q type = toLower(model.base_name);
            string_q item = substitute(toLower(model.doc_group), " ", "") + "|" + type;
            result.push_back(item);
        }
    }
    return;
}

//------------------------------------------------------------------------------------------------------------
string_q get_models(const CClassDefinitionArray& models, const string_q& route) {
    CStringArray types;
    get_models(models, types, route);

    ostringstream os;
    for (auto type : types) {
        string_q group = nextTokenClear(type, '|');
        os << "- [" << type << "](/data-model/" << group << "/#" << type << ")" << endl;
    }

    return "\n\nData models produced by this tool:\n\n" + (os.str().empty() ? "- none" : trim(os.str(), '\n'));
}

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_readmes(void) {
    map<string_q, string_q> groupParts;
    map<string_q, uint64_t> weights;
    uint32_t weight = 1000;
    for (auto ep : endpointArray) {
        if (!ep.api_route.empty()) {
            if (ep.is_visible_docs) {
                if (weights[ep.group] == 0) {
                    weights[ep.group] = weight;
                    weight += 200;
                }
                groupParts[ep.group] += ep.api_route + ",";

                string_q docFn = substitute(toLower(ep.group), " ", "") + "-" + ep.api_route + ".md";
                string_q docSource = "../templates/readme-intros/" + docFn;
                string_q docContents = STR_README_BEGPARTS + asciiFileToString(docSource) + STR_README_ENDPARTS;

                replaceAll(docContents, "[{USAGE}]", get_usage(ep.api_route));
                replaceAll(docContents, "[{CONFIG}]", get_config_usage(ep));
                replaceAll(docContents, "[{NOTES}]", get_readme_notes(ep));
                replaceAll(docContents, "[{MODELS}]", get_models(dataModels, ep.api_route));
                replaceAll(docContents, "[{NAME}]", "chifra " + ep.api_route);
                string_q noTicks = substitute(docContents, "'", "'");
                writeIfDifferent("../readmes/"+docFn,
                                 substitute(substitute(noTicks, "[{LINKS}]", get_links(ep)), "[{FOOTER}]", "\n"));

                string_q footerFn = "../templates/readme-intros/README.footer.md";
                string_q sourceFooter = "\n\n" + trim(asciiFileToString(footerFn), '\n') + "\n";
                string_q sourceReadme =
                    substitute(getPathToSource("apps/chifra/internal/" + ep.api_route + "/README.md"), "//", "/");
                writeIfDifferent(sourceReadme,
                                 substitute(substitute(docContents, "[{FOOTER}]", sourceFooter), "[{LINKS}]", ""));
            }
        }
    }

    for (auto part : groupParts) {
        string_q group = part.first;
        string_q tool = part.second;

        string_q front = STR_YAML_FRONTMATTER;
        replace(front, "[{TITLE}]", firstUpper(toLower(group)));
        replace(front, "[{WEIGHT}]", uint_2_Str(weights[group]));
        replace(front, "[{M1}]", "chifra:");
        replace(front, "[{M2}]", "parent: commands");
        replace(front, "[{HUGO_ALIASES}]", getAliases(this, "chifra", group));
        group = substitute(toLower(group), " ", "");

        ostringstream os;
        os << front;
        os << endl << asciiFileToString("../templates/readme-groups/" + group + ".md") << endl;

        CStringArray paths;
        explode(paths, tool, ',');
        for (auto p : paths) {
            string_q pp = "../readmes/" + group + "-" + p + ".md";
            os << asciiFileToString(pp);
        }

        string_q outFn = "../content/chifra/" + group + ".md";
        string_q out = os.str();
        writeIfDifferent(outFn, out);
    }
    return true;
}

//------------------------------------------------------------------------------------------------------------
string_q getAliases(COptions* opts, const string_q& group, const string_q& route) {
    string_q path = toLower(substitute("/" + group + "/" + route, " ", ""));
    string_q value = opts->hugoAliasMap[path];
    if (value.empty())
        return "";

    string_q ret = "aliases:\n[{LIST}]";
    ostringstream os;
    CStringArray parts;
    explode(parts, value, ',');
    for (auto part : parts) {
        os << " - \"" << part << "\"" << endl;
    }
    return substitute(ret, "[{LIST}]", os.str());
}

//------------------------------------------------------------------------------------------------------------
const char* STR_YAML_FRONTMATTER =
    "---\n"
    "title: \"[{TITLE}]\"\n"
    "description: \"\"\n"
    "lead: \"\"\n"
    "lastmod:\n"
    "  - :git\n"
    "  - lastmod\n"
    "  - publishDate\n"
    "draft: false\n"
    "[{HUGO_ALIASES}]menu:\n"
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
    "**Configuration file:** `trueBlocks.toml`  \n"
    "**Configuration group:** `[scrape.<chain>]`\n"
    "\n"
    "[{CONFIGS}]\n"
    "Note that for Ethereum mainnet, the default values for appsPerChunk and firstSnap are 2,000,000 and 2,300,000 "
    "respectively. See the specification for a justification of these values.\n"
    "\n"
    "These items may be set in three ways, each overriding the preceding method:\n"
    "\n"
    "-- in the above configuration file under the `[scrape.<chain>]` group,  \n"
    "-- in the environment by exporting the configuration item as UPPER&lowbar;CASE (with underbars removed) and "
    "prepended with TB_SCRAPE&lowbar;CHAIN&lowbar;, or  \n"
    "-- on the command line using the configuration item with leading dashes and in snake case (i.e., `--snake_case`).";

const char* STR_README_BEGPARTS = "## [{NAME}]\n\n";
const char* STR_README_ENDPARTS = "\n[{USAGE}][{MODELS}][{LINKS}][{CONFIG}][{NOTES}][{FOOTER}]\n";
