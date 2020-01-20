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

extern const char* STR_EXPORT_1;
extern const char* STR_EXPORT_2;
extern const char* STR_POLLING;
extern const char* STR_EXTRACT_CASE;
extern const char* STR_TEXT_ACTIONS;
extern const char* STR_TEXT_IMPORTS;
extern const char* STR_TEXT_CODE;
extern const char* STR_NAVLINK;
extern const char* STR_EMPTY_INNER;
extern const char* STR_EMPTY_INNER_COLLAPSE;
extern bool visitField(const CFieldData& field, void* data);

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_frontend(CToml& toml, const CClassDefinition& classDef) {
    CPage page;
    page.longName = classDef.base_lower;
    page.properName = classDef.base_proper;
    page.pageNotes = toml.getConfigStr("settings", "page_notes", "");
    page.twoName = toLower(page.longName.substr(0, 2));
    page.sevenName = padRight(page.longName.substr(0, 7), 7, '_');
    page.polling = toml.getConfigBool("settings", "polling", false);
    page.files = toml.getConfigStr("settings", "files", "index|actions|dispatchers|inner|reducers");
    page.menuType = toml.getConfigStr("settings", "menuType", "LocalMenu");
    page.no_error = toml.getConfigBool("settings", "no_error", false);
    page.no_data = page.no_error || toml.getConfigBool("settings", "no_data", false);
    page.no_dt = page.no_data || toml.getConfigBool("settings", "no_dt", false);
    page.has_text = false;
    page.color = toml.getConfigStr("settings", "color", "''");

    CStringArray reserved = {"in"};
    for (auto r : reserved)
        if (page.twoName == r)
            page.twoName = toLower(page.longName.substr(0, 3));

    CSubpage item;
    string_q subpages = toml.getConfigJson("subpages", "list", "");
    while (item.parseJson3(subpages)) {
        replaceAll(item.options, "_EQ_", "=");
        replaceAll(item.extract, "_0", "[0]");
        if (item.route.empty())
            page.has_text = true;
        page.subpages.push_back(item);
        item = CSubpage();
    }

    string_q destFolder = "../" + page.longName + "/";
    establishFolder(destFolder);
    CStringArray files;
    explode(files, page.files, '|');
    for (auto file : files)
        handle_one_frontend_file(page, destFolder, (file == "index" ? file : "blank-" + file) + ".js");
    string_q ccsFile = destFolder + page.longName + ".css";
    if (!fileExists(ccsFile))
        copyFile("./blank.css", ccsFile);
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
    handle_generate_frontend_app();
    return false;  // we're done processing
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_one_frontend_file(const CPage& page, const string_q& folder, const string_q& source) {
    string_q destFile = folder + substitute(source, "blank-", "");
    string_q sourceFile = "./" + source;
    string_q code = asciiFileToString(sourceFile);
    replaceAll(code, "[{ ", "[{");  // remove weird editor spacing
    replaceAll(code, " }]", "}]");  // remove weird editor spacing

    uint32_t cnt = 0;
    ostringstream commands, menu_items, text_imports, text_code;
    map<string, string> extractMap;
    for (auto item : page.subpages) {
        replaceAll(code, "[{DT_TYPE}]", item.table_type == "object" ? "ObjectTable" : "DataTable");
        replaceAll(code, "[{SUBPAGE}]", item.subpage);
        replaceAll(code, "[{QUERY_URL}]", item.route);
        replaceAll(code, "[{QUERY_OPTS}]", item.options);
        replaceAll(code, "[{QUERY_EXTRACT}]", item.extract);
        commands << "export const " << toUpper(item.subpage) << " = '" << item.options << "';" << endl;
        if (!menu_items.str().empty())
            menu_items << ",";
        menu_items << endl;
        menu_items << "    { subpage: '" << substitute(toLower(item.subpage), "_", " ");
        menu_items << "', route: '" + item.route + "', query: " << page.twoName << "." << toUpper(item.subpage) << " }";
        cnt++;
        string_q curVal = extractMap[item.extract];
        extractMap[item.extract] = (toUpper(item.subpage) + "|" + curVal);

        if (item.route.empty()) {
            if (text_imports.str() != "")
                text_imports << endl;
            text_imports << substitute(STR_TEXT_IMPORTS, "[{SUBPAGE}]", item.subpage);
            if (text_code.str() != "")
                text_code << endl;
            text_code << substitute(substitute(STR_TEXT_CODE, "[{SUBPAGE}]", item.subpage), "[{SP_UPPER}]",
                                    toUpper(item.subpage));
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
    replaceAll(code, "[{COMMANDS}];", commands.str());
    replaceAll(code, "[{MENU_ITEMS}]", menu_items.str());

    replaceAll(code, "[{TEXT_ACTIONS}]", (page.has_text ? STR_TEXT_ACTIONS : ""));
    replaceAll(code, "[{TEXT_IMPORTS}]", (page.has_text ? text_imports.str() : ""));
    string_q tc = text_code.str();
    replace(tc, "} else ", "");  // remove one
    replaceAll(code, "[{TEXT_CODE}]", (page.has_text ? tc + "\n    }" : ""));

    ostringstream reducers;
    for (auto e : extractMap) {
        string_q extract = e.first;
        CStringArray cmds;
        explode(cmds, e.second, '|');
        for (auto c : cmds) {
            reducers << "    case " << page.twoName << "." << c << ":" << endl;
        }
        reducers << substitute(STR_EXTRACT_CASE, "_EXTRACT_", e.first);
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

    bool hasEar = contains(asciiFileToString(destFile), "innerEar");
    replace(code, "[IE]", (hasEar ? "innerEar={this.innerEar} " : ""));

    CStringArray lines;
    explode(lines, code, '\n', false);
    code = "";
    for (auto line : lines) {
        bool include = true;
        if (page.no_error && contains(line, "[{NO_ERROR}]"))
            include = false;
        if (page.no_data && contains(line, "[{NO_DATA}]"))
            include = false;
        if (page.no_dt && contains(line, "[{NO_DT}]"))
            include = false;
        if (!page.has_text && contains(line, "[{NO_TEXT}]"))
            include = false;
        if (contains(line, "[{MENU_"))
            include = false;
        if (include)
            code += (line + '\n');
    }
    replaceAll(code, "[{NO_ERROR}]", "");
    replaceAll(code, "[{NO_DATA}]", "");
    replaceAll(code, "[{NO_DT}]", "");
    replaceAll(code, "[{NO_TEXT}]", "");

    replaceAll(code, "[{CONNECT}]", page.polling ? STR_EXPORT_2 : STR_EXPORT_1);
    replaceAll(code, "[{LOWER}]", page.longName);
    replaceAll(code, "[{SEVEN}]", page.sevenName);
    replaceAll(code, "[{TWO}]", page.twoName);
    replaceAll(code, "[{PAGENOTES}]", substitute(page.pageNotes, "|", " \\\n        "));
    replaceAll(code, "[{PROPER}]", page.properName);
    replaceAll(code, "[{COLOR}]", page.color);
    replaceAll(code, "[{POLLING}]", page.polling ? STR_POLLING : "");
    replaceAll(code, STR_EMPTY_INNER, STR_EMPTY_INNER_COLLAPSE);

    // writeTheCode returns true or false depending on if it WOULD HAVE written the file. If 'test'
    // is true, it doesn't actually write the file
    bool wouldHaveWritten = writeTheCode(destFile, code, nspace, 2, test);
    if (wouldHaveWritten) {
        if (test) {
            cerr << "File '" << destFile << "' changed but was not written because of testing." << endl;
        } else {
            counter.nProcessed++;
        }
    }
    return false;
}

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
bool COptions::handle_generate_frontend_app(void) {
    string_q app_import = "import [{PROPER}], { [{LOWER}]_menu } from './pages/[{LOWER}]';\n";
    string_q route = "          <Route component={[{PROPER}]} [{EXACT}]path=\"[{PATH}]\" />\n";
    string_q red_import = "import reducer_[{PROPER}] from './pages/[{LOWER}]/reducers';\n";
    string_q reducer = "  reducer_[{PROPER}],\n";

    CToml toml("./classDefinitions/app.txt");
    string_q str = toml.getConfigStr("settings", "pages", "");
    CStringArray pages;
    explode(pages, str, '|');
    ostringstream app_imports, red_imports, routes, reducers, navlinks;
    for (auto exact : pages) {
        string_q lower = nextTokenClear(exact, ',');
        string_q proper = toProper(lower);
        string_q path = nextTokenClear(exact, ',');
        string_q path2 = path;
        string_q navpath = path.empty() ? "" : "path=\"" + path + "\" ";
        if (path.empty()) {
            path = "/[{LOWER}]/:subpage?/:query?";
            path2 = "/[{LOWER}]";
        }
        if (!exact.empty())
            exact += "={true} ";
        app_imports << substitute(substitute(app_import, "[{PROPER}]", proper), "[{LOWER}]", lower);
        replace(exact, "={true}", "");
        routes << substitute(
            substitute(substitute(substitute(route, "[{EXACT}]", exact), "[{PATH}]", path), "[{PROPER}]", proper),
            "[{LOWER}]", lower);
        red_imports << substitute(substitute(red_import, "[{PROPER}]", proper), "[{LOWER}]", lower);
        reducers << substitute(reducer, "[{PROPER}]", proper);
        navlinks << substitute(substitute(STR_NAVLINK, "[{PROPER}]", proper), "[{LOWER}]", lower) << endl;
    }

    string_q contents = asciiFileToString("./blank-app.js");
    string_q orig = asciiFileToString("../../App.js");
    replaceAll(contents, "[{IMPORTS}]", app_imports.str());
    replaceAll(contents, "[{NAVLINKS}]", (navlinks.str()));
    replaceAll(contents, "[{ROUTES}]", routes.str());
    if (orig != contents) {
        stringToAsciiFile("../../App.js", contents);
        LOG_INFO("Writing: ", cTeal, "./App.js", cOff);
    }

    contents = asciiFileToString("./blank-root-reducers.js");
    orig = asciiFileToString("../../root-reducers.js");
    replaceAll(contents, "[{IMPORTS}]", red_imports.str());
    replaceAll(contents, "[{REDUCERS}]", trim(trim(reducers.str(), '\n'), ','));
    if (orig != contents) {
        stringToAsciiFile("../../root-reducers.js", contents);
        LOG_INFO("Writing: ", cTeal, "./root-reducers.js", cOff);
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
const char* STR_EXPORT_1 =
    "export default connect(\n"
    "\tmapStateToProps,\n"
    "\tmapDispatchToProps\n"
    ")([{PROPER}]Inner);\n";

//---------------------------------------------------------------------------------------------------
const char* STR_EXPORT_2 =
    "export default polling(dispatcher_[{PROPER}], poll_timeout)(\n"
    "\tconnect(\n"
    "\t\tmapStateToProps,\n"
    "\t\tmapDispatchToProps\n"
    "\t)([{PROPER}]Inner)\n"
    ");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_POLLING =
    "\n"
    "\n"
    "import { polling } from '../../components/polling';\n"
    "import { poll_timeout } from '../../config.js';\n";

//---------------------------------------------------------------------------------------------------
const char* STR_EXTRACT_CASE =
    "      return {\n"
    "        ...state,\n"
    "        data: action.payload.data_EXTRACT_,\n"
    "        fieldList: action.payload.fieldList,\n"
    "        meta: action.payload.meta,\n"
    "        isLoading: false,\n"
    "        error: null\n"
    "      };\n\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TEXT_ACTIONS = "import * as [{TWO}] from './actions';";

//---------------------------------------------------------------------------------------------------
const char* STR_TEXT_IMPORTS = "import { [{SUBPAGE}]Text } from './text/[{SUBPAGE}]';";

//---------------------------------------------------------------------------------------------------
const char* STR_TEXT_CODE =
    "    } else if (this.state.cur_submenu.query === [{TWO}].[{SP_UPPER}]) {\n"
    "      return [{SUBPAGE}]Text();";

//---------------------------------------------------------------------------------------------------
const char* STR_NAVLINK = "mainMenu.push([{LOWER}]_menu);";

//---------------------------------------------------------------------------------------------------
const char* STR_EMPTY_INNER =
    "    return (\n"
    "      <Fragment>\n"
    "        {this.getInnerMost()}\n"
    "      </Fragment>\n"
    "    );\n";

//---------------------------------------------------------------------------------------------------
const char* STR_EMPTY_INNER_COLLAPSE = "    return <Fragment>{this.getInnerMost()}</Fragment>;\n";
