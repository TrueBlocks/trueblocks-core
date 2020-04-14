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
#include "utillib.h"
#include "options.h"

extern bool visitField(const CFieldData& field, void* data);

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js(CToml& toml, const CClassDefinition& classDef) {
    CPage page;
    page.longName = classDef.base_lower;
    page.properName = classDef.base_proper;
    page.twoName = toLower(page.longName.substr(0, 2));
    page.sevenName = padRight(page.longName.substr(0, 7), 7, '_');
    page.dest_path = toml.getConfigStr("settings", "dest_path", "./src/pages/") + page.properName + "/";

    CStringArray reserved = {"in"};
    for (auto r : reserved)
        if (page.twoName == r)
            page.twoName = toLower(page.longName.substr(0, 3));

    CSubpage item;
    string_q subpages = toml.getConfigJson("subpages", "list", "");
    while (item.parseJson3(subpages)) {
        replaceAll(item.options, "_EQ_", "=");
        replaceAll(item.extract, "_0", "[0]");
        page.subpages.push_back(item);
        item = CSubpage();
    }

    if (page.longName != "separator") {
        establishFolder(page.dest_path);
        string_q cssFile = page.dest_path + page.properName + ".css";
        if (!fileExists(cssFile))
            stringToAsciiFile(cssFile, "/* add custom css code for the " + page.properName + " compnent here. */\n");
    }

    pageMap[page.longName] = page;

    return false;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_json_export(void) {
    for (auto classDef : classDefs) {
        if (classDef.short_fn != "app") {
            CBaseNode* item = createObjectOfType(classDef.short_fn);
            if (item) {
                CRuntimeClass* pClass = item->getRuntimeClass();
                pClass->forEveryField(visitField, &cout);
            } else {
                CToml toml("");
                toml.readFile(classDef.input_path);
                handle_generate(toml, classDef, nspace, true);
            }
        }
    }

    handle_generate_js_menus();
    //    handle_generate_js_skins();

    return false;  // we're done processing
}

/*
//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_skins(void) {
    string_q dataFile = "../../skins/skins.csv";
    CStringArray lines;
    asciiFileToLines(dataFile, lines);
    CStringArray fields;
    CSkinArray skins;
    for (auto line : lines) {
        if (fields.empty()) {
            explode(fields, line, ',');
        } else {
            CSkin skin;
            skin.parseCSV(fields, line);
            skins.push_back(skin);
        }
    }

    for (auto skin : skins) {
        ostringstream os;
        os << skin.Format(STR_SKIN_EXPORT) << endl;
        string_q thisFile = substitute(dataFile, "skins.csv", skin.name + ".jsx");
        string_q orig = asciiFileToString(thisFile);
        if (os.str() != orig) {
            stringToAsciiFile(thisFile, os.str());
            cerr << "Wrote " << thisFile << endl;
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
const char* STR_SKIN_EXPORT =
    "const [{NAME}] = {\n"
    "  colorBgPrimary: '[{bgPrimary}]',\n"
    "  colorBgSecondary: '[{bgSecondary}]',\n"
    "  colorTextPrimary: '[{textPrimary}]',\n"
    "  colorBorderPrimary: '[{borderPrimary}]',\n"
    "\n"
    "  colorTableBgPrimary: '[{tableBgPrimary}]',\n"
    "  colorTableBgSecondary: '[{tableBgSecondary}]',\n"
    "  colorTableTextPrimary: '[{tableTextPrimary}]',\n"
    "  colorTableBorderPrimary: '[{tableBorderPrimary}]',\n"
    "\n"
    "  colorBgHover: '[{bgHover}]',\n"
    "  colorTextHover: '[{textHover}]'\n"
    "};\n"
    "\n"
    "export default [{NAME}];\n";

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_file(const CPage& page, const string_q& folder, const string_q& source) {
    string_q destFile = folder + substitute(source, "blank-", "");
    string_q sourceFile = "./" + source;
    string_q code = asciiFileToString(sourceFile);
    replaceAll(code, "[{ ", "[{");  // remove weird editor spacing
    replaceAll(code, " }]", "}]");  // remove weird editor spacing

    uint32_t cnt = 0;
    ostringstream commands, menu_items, text_imports, text_code;
    map<string, string> extractMap;
    for (auto item : page.subpages) {
        replaceAll(code, "[{SUBPAGE}]", item.subpage);
        replaceAll(code, "[{QUERY_URL}]", item.route);
        replaceAll(code, "[{QUERY_OPTS}]", item.options);
        replaceAll(code, "[{QUERY_EXTRACT}]", item.extract);
        if (!menu_items.str().empty())
            menu_items << ",";
        menu_items << endl;
        if (!item.isSeparator) {
            menu_items << "    { ";
            menu_items << "subpage: '" << substitute(toLower(item.subpage), "_", " ") << "'";
            menu_items << ", route: '" << item.route << "'";
            menu_items << ", query: " << page.twoName << "." << toUpper(item.subpage);
            menu_items << (item.icon.empty() ? "" : (", icon: '" + item.icon + "'"));
            menu_items << " }";
            cnt++;
            string_q curVal = extractMap[item.extract];
            extractMap[item.extract] = (toUpper(item.subpage) + "|" + curVal);
            if (!commands.str().empty())
                commands << endl;
            commands << "export const " << toUpper(item.subpage) << " = '" << item.options << "';";
            if (item.route.empty()) {
                if (text_imports.str() != "")
                    text_imports << endl;
                text_imports << substitute(STR_TEXT_IMPORTS, "[{SUBPAGE}]", item.subpage);
                if (text_code.str() != "")
                    text_code << endl;
                text_code << substitute(substitute(STR_TEXT_CODE, "[{SUBPAGE}]", item.subpage), "[{SP_UPPER}]",
                                        toUpper(item.subpage));
            }
        } else {
            menu_items << "    { subpage: 'separator' }";
        }
    }
    if (cnt == 0) {
    } else {
        while (cnt < 7) {
            if (!menu_items.str().empty())
                menu_items << "," << endl;
            menu_items << "    { subpage: '" << page.twoName << "-" << padNum4(cnt) << "' }";
            cnt++;
        }
        menu_items << endl;
    }
    if (contains(code, "[{COMMANDS}]")) {
        replaceAll(code, "[{COMMANDS}];", commands.str());
        if (cnt == 0) {
            while (endsWith(code, "\n"))
                replaceReverse(code, "\n", "");
        }
    }
    replaceAll(code, "[{MENU_ITEMS}]", menu_items.str());
    replaceAll(code, "items: [  ],", "items: [],");

    replaceAll(code, "[{TEXT_ACTIONS}]", (page.has_text ? STR_TEXT_ACTIONS : ""));
    replaceAll(code, "[{TEXT_IMPORTS}]", (page.has_text ? text_imports.str() : ""));
    string_q tc = text_code.str();
    replace(tc, "} else ", "");  // remove one
    replaceAll(code, "[{TEXT_CODE}]", (page.has_text ? trim(tc) + "\n    }" : ""));

    uint32_t count = 0;
    ostringstream reducers;
    for (auto e : extractMap) {
        string_q extract = e.first;
        CStringArray cmds;
        explode(cmds, e.second, '|');
        for (auto c : cmds) {
            reducers << "    case " << page.twoName << "." << c << ":" << endl;
        }
        string_q s = substitute(STR_EXTRACT_CASE, "_EXTRACT_", e.first);
        if (page.longName != "digests" && page.longName != "settings")
            s = substitute(s, "types[0].fields", "types[" + uint_2_Str(count++) + "].fields");
        reducers << s;
    }
    replaceAll(code, "[{REDUCERS}]", reducers.str());

    string_q thing = "";  // page.subpage;
    replaceAll(code, "[{STATE_FIELDS_2}]", nextTokenClear(thing, ':') + ": value");

    if (page.menuType == "DashMenu") {
        replaceAll(code, "[{MENU_TYPE}]", page.menuType);
        replaceAll(code, "[{MENU_CLICK}]", "changePage={this.changePage}");
        replaceAll(code, "[{MENU_FILE}]", "dash-menu");
        replaceAll(code, "[{MENU_COMMENT}]", "");
    }

    CStringArray lines;
    explode(lines, code, '\n', false);
    code = "";
    for (auto line : lines) {
        bool include = true;
        if (page.no_dash && contains(line, "[{NO_DASH}]"))
            include = false;
        if (page.no_error && contains(line, "[{NO_ERROR}]"))
            include = false;
        if (page.no_data && contains(line, "[{NO_DATA}]"))
            include = false;
        if (!page.dat_table && contains(line, "[{NO_DT}]"))
            include = false;
        if (!page.obj_table && contains(line, "[{NO_OBJ}]"))
            include = false;
        if (!page.has_text && contains(line, "[{NO_TEXT}]"))
            include = false;
        if (contains(line, "[{MENU_"))
            include = false;
        if (include)
            code += (line + '\n');
    }
    replaceAll(code, "[{NO_DASH}]", "");
    replaceAll(code, "[{NO_ERROR}]", "");
    replaceAll(code, "[{NO_DATA}]", "");
    replaceAll(code, "[{NO_DT}]", "");
    replaceAll(code, "[{NO_OBJ}]", "");
    replaceAll(code, "[{NO_TEXT}]", "");

    replaceAll(code, "[{CONNECT}]", page.polling ? STR_EXPORT_2 : STR_EXPORT_1);
    replaceAll(code, "[{LOWER}]", page.longName);
    replaceAll(code, "[{SEVEN}]", page.sevenName);
    replaceAll(code, "[{TWO}]", page.twoName);
    replaceAll(code, "[{PROPER}]", page.properName);
    replaceAll(code, "[{COLOR}]", page.color);
    replaceAll(code, "[{POLLING}]", page.polling ? STR_POLLING : "");
    replaceAll(code, STR_EMPTY_INNER, STR_EMPTY_INNER_COLLAPSE);
    if (contains(destFile, "inner")) {
        string_q orig = asciiFileToString(destFile);
        if (!contains(substitute(orig, ", { Fragment }", ""), "Fragment"))
            replace(code, ", { Fragment }", "");
    }

    // returns true or false depending on if it WOULD HAVE written the file. If 'test'
    // is true, it doesn't actually write the file
    bool wouldHaveWritten = writeTheCode(codewrite_t(destFile, code, nspace, 2, test, false, force));
    if (wouldHaveWritten) {
        if (test) {
            cerr << "File '" << destFile << "' changed but was not written because of testing." << endl;
        } else {
            counter.nProcessed++;
        }
    }
    return false;
}
*/

//---------------------------------------------------------------------------------------------------
bool visitField(const CFieldData& field, void* data) {
    ostream* pOs = reinterpret_cast<ostream*>(data);
    *pOs << "<Row ";
    *pOs << "name=\"" << field.getName() << "\" ";
    *pOs << "type=\"string\" ";
    *pOs << "value={item." << field.getName() << "} ";
    *pOs << "display={item." << field.getName() << "} ";
    *pOs << "route=\"\" ";
    *pOs << "/>" << endl;
    return true;
}

//---------------------------------------------------------------------------------------------------
void doReplace(string_q& str, const string_q& type, const string_q& rep) {
    string_q autotag = "  // auto-generate: " + type;
    string_q ontag = "<" + type + ">";
    string_q offtag = "</" + type + ">";

    ostringstream os;
    os << autotag << endl;
    os << rep;
    os << autotag; // new line not needed

    replace(str, autotag, ontag);
    replace(str, autotag, "  " + offtag);

    replace(str, offtag, offtag + os.str());
    snagFieldClear(str, type);
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_menus(void) {
    CToml toml("./classDefinitions/app.txt");
    string_q pageList = toml.getConfigStr("settings", "pages", "");
    CStringArray pagesStrs;
    explode(pagesStrs, pageList, '|');

    string_q indexFile = "./src/pages/index.jsx";
    string_q contents = asciiFileToString(indexFile);

    ostringstream importStream;
    ostringstream pageStream;
    ostringstream menuStream;
    menuStream << "  items: [" << endl;

    for (auto pageStr : pagesStrs) {
        CPage page = pageMap[pageStr];
        cerr << page.longName << endl;
        bool isSeparator = page.longName == "separator";
        if (isSeparator) {
            menuStream << "    { label: '" << page.properName << "' }," << endl;
        } else {
            importStream
                << "import { "
                << page.properName
                << " from ./"
                << page.properName
                << "/"
                << page.properName
                << "';"
                << endl;

            menuStream << "    {" << endl;
            menuStream << "      label: '" << page.properName << "'," << endl;
            menuStream << "      exact: true," << endl;
            if (page.subpages.size() == 1 && page.subpages[0].route == "/")
                menuStream << "      route: '/'," << endl;
            if (page.subpages.size() > 1)
                menuStream << "      items: [" << endl;

            for (auto sub : page.subpages) {
                if (!sub.isSeparator) {
                    pageStream
                        << "  '"
                        << page.longName
                        << "/"
                        << (sub.route.empty() ? toLower(sub.subpage) : sub.route == "/" ?  "" : sub.route)
                        << "': { component: <"
                        << page.properName
                        << " /> },"
                        << endl;
                }

                if (sub.isSeparator)
                    menuStream << "        { label: 'Separator' }," << endl;
                else if (sub.subpage != "") {
                    menuStream << "        { label: '" << sub.subpage << "'";
                    if (!sub.route.empty())
                        menuStream << ", route: '" << sub.route << "'";
                    menuStream << " }," << endl;
                }
            }
            pageStream << "  //" << endl;
            if (page.subpages.size() > 1)
                menuStream << "      ]" << endl;
            menuStream << "    }," << endl;
        }
    }
    menuStream << "  ]" << endl;

    doReplace(contents, "imports", importStream.str());
    doReplace(contents, "pages", pageStream.str());
    doReplace(contents, "menus", menuStream.str());

    stringToAsciiFile(indexFile, contents);

    return true;
}
