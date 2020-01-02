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

    CStringArray reserved = { "in" };
    for (auto r : reserved)
        if (page.twoName == r)
            page.twoName = toLower(page.longName.substr(0, 3));

    CSubpage item;
    string_q subpages = toml.getConfigJson("subpages", "list", "");
    while (item.parseJson3(subpages)) {
        replaceAll(item.options, "_EQ_", "=");
        replaceAll(item.extract, "_0", "[0]");
        if (item.from_text)
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
    map<string, string> extactorMap;
    string_q def_menu;
    for (auto item : page.subpages) {
        replaceAll(code, "[{SUBPAGE}]", item.subpage);
        replaceAll(code, "[{QUERY_URL}]", item.route);
        replaceAll(code, "[{QUERY_OPTS}]", item.options);
        replaceAll(code, "[{QUERY_EXTRACT}]", item.extract);
        commands << "export const " << toUpper(item.subpage) << " = '" << item.route << "/" << item.options << "';" << endl;
        if (!menu_items.str().empty())
            menu_items << "," << endl;
        menu_items << "      { header: '" << toProper(item.subpage) << "', value: 'VAL', action: " << toUpper(item.subpage) << " }";
        if (cnt == 0)
            def_menu = (item.route + "/" + item.options);
        cnt++;
        string_q curVal = extactorMap[item.extract];
        extactorMap[item.extract] = (toUpper(item.subpage) + "|" + curVal);

        if (item.from_text) {
            if (text_imports.str() != "")
                text_imports << endl;
            text_imports << substitute(STR_TEXT_IMPORTS, "[{SUBPAGE}]", item.subpage);
            if (text_code.str() != "")
                text_code << endl;
            text_code << substitute(substitute(STR_TEXT_CODE, "[{SUBPAGE}]", item.subpage), "[{SP_UPPER}]", toUpper(item.subpage));
        }
    }

    while (cnt < 7) {
        if (!menu_items.str().empty())
            menu_items << "," << endl;
        menu_items << "      { header: '" << page.twoName << "-" << padNum4(cnt) << "' }";
        cnt++;
    }
    menu_items << endl;
    replaceAll(code, "[{COMMANDS}];", commands.str());
    replaceAll(code, "[{MENU_ITEMS}]", menu_items.str());
    replaceAll(code, "[{DEFAULT_MENU}]", def_menu);

    replaceAll(code, "[{TEXT_ACTIONS}]", (page.has_text ? STR_TEXT_ACTIONS : ""));
    replaceAll(code, "[{TEXT_IMPORTS}]", (page.has_text ? text_imports.str() : ""));
    string_q tc = text_code.str();
    replace(tc, "} else ", "");  // remove one
    replaceAll(code, "[{TEXT_CODE}]", (page.has_text ? tc : ""));

    ostringstream red;
    for (auto e : extactorMap) {
        string_q extract = e.first;
        CStringArray cmds;
        explode(cmds, e.second, '|');
        for (auto c : cmds) {
            red << "    case " << page.twoName << "." << c << ":" << endl;
        }
        red << substitute(STR_EXTRACT_CASE, "_EXTRACT_", e.first);
    }
    replaceAll(code, "[{REDUCERS}]", red.str());

    string_q thing = "";  // page.subpage;
    replaceAll(code, "[{STATE_FIELDS_2}]", nextTokenClear(thing, ':') + ": value");

    CStringArray lines;
    explode(lines, code, '\n', false);
    code = "";
    for (auto line : lines) {
        bool include = true;
        if (page.no_error && contains(line, "[{NO_ERROR}]")) include = false;
        if (page.no_data && contains(line, "[{NO_DATA}]")) include = false;
        if (page.no_dt && contains(line, "[{NO_DT}]")) include = false;
        if (!page.has_text && contains(line, "[{NO_TEXT}]")) include = false;
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
    replaceAll(code, "[{PAGENOTES}]", substitute(page.pageNotes, "|", " \n            "));
    replaceAll(code, "[{PROPER}]", page.properName);
    replaceAll(code, "[{COLOR}]", page.color);
    replaceAll(code, "[{MENU_TYPE}]", page.menuType);
    replaceAll(code, "[{MENU_CLICK}]", (page.menuType == "DashMenu" ? "changePage={this.changePage}" : "innerEar={this.innerEar}"));
    replaceAll(code, "[{MENU_FILE}]", page.menuType == "LocalMenu" ? "local-menu" : "dash-menu");
    replaceAll(code, "[{POLLING}]", page.polling ? STR_POLLING : "");

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
    "        isLoading: false,\n"
    "        error: null,\n"
    "        data: action.payload_EXTRACT_\n"
    "      };\n\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TEXT_ACTIONS =
    "import * as [{TWO}] from './actions';";

//---------------------------------------------------------------------------------------------------
const char* STR_TEXT_IMPORTS =
    "import { [{SUBPAGE}]Text } from './text-[{SUBPAGE}]';";

//---------------------------------------------------------------------------------------------------
const char* STR_TEXT_CODE =
"    } else if (this.state.subpage === [{TWO}].[{SP_UPPER}]) {\n"
"      return [{SUBPAGE}]Text();\n"
"    }";
