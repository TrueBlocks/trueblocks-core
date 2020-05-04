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
    page.dataUrl = toml.getConfigStr("settings", "dataUrl", "");
    page.dataQuery = toml.getConfigStr("settings", "dataQuery", "");
    page.cmdUrl = toml.getConfigStr("settings", "cmdUrl", "");
    if (page.cmdUrl.empty())
        page.cmdUrl = page.dataUrl;

    page.dest_path = toml.getConfigStr("settings", "dest_path", "./pages/") + page.properName + "/";
    page.schema = toml.getConfigStr("settings", "schema", "./" + page.longName + ".csv");
    page.defaultSort = toml.getConfigStr("settings", "defaultSort", "");
    page.defaultSearch = toml.getConfigStr("settings", "defaultSort", "");
    string_q rec = substitute("," + toml.getConfigStr("settings", "recordIcons", "") + ",", ",,", "");
    replace(rec, "editing,", "header-Add,Delete/Undelete,Edit/Remove,");
    replace(rec, "viewing,", "ExternalLink,");
    replace(rec, "view,", "View/None,");
    replace(rec, "follow,", "AddMonitor/None/View,");
    replace(rec, "exporting,", "footer-CSV,footer-TXT,");
    replace(rec, "importing,", "footer-Import,");
    page.recordIcons = trim(rec, ',');

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
bool COptions::handle_export_js(void) {
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
    handle_generate_js_schemas();
    handle_generate_js_help();
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
 replaceAll(code, "[{LONG}]", page.longName);
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
void doReplace(string_q& str, const string_q& type, const string_q& rep, const string_q& spaces) {
    string_q autotag = spaces + "// auto-generate: " + type;
    string_q ontag = "<" + type + ">";
    string_q offtag = "</" + type + ">";

    ostringstream os;
    os << autotag << endl;
    os << rep;
    os << autotag;  // new line not needed

    replace(str, autotag, ontag);
    replace(str, autotag, "  " + offtag);
    replace(str, offtag, offtag + os.str());
    snagFieldClear(str, type);
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_menus(void) {
    cerr << cYellow << "Generating Menus..." << cOff << endl;

    CToml toml("./classDefinitions/app.toml");
    string_q pageList = toml.getConfigStr("settings", "pages", "");
    string_q sourceList = toml.getConfigStr("settings", "from_source", "");

    CStringArray sourceStrs;
    explode(sourceStrs, sourceList, '|');

    for (auto sourceStr : sourceStrs) {
        CStringArray parts;
        explode(parts, sourceStr, '-');

        CPage page = pageMap[parts[0]];
        if (page.longName != "separator") {
            string_q codeFile = "./pages/" + page.properName + "/" + page.properName +
                                (parts.size() > 1 ? toProper(parts[1]) : "") + ".jsx";

            cerr << "\tProcessing " << page.longName << "..."
                 << "\r";
            string_q templateFile = "./classDefinitions/templates/page-template.jsx";
            if (parts[0] == "explorer") {
                templateFile = "./classDefinitions/templates/page-explorer-template.jsx";
                page.longName = parts[1];
                page.properName = toProper(parts[1]);
                page.twoName = toLower(page.longName.substr(0, 2));
                page.sevenName = padRight(page.longName.substr(0, 7), 7, '_');
                CToml t("./classDefinitions/" + parts[0] + ".toml");
                page.dataQuery = t.getConfigStr("settings", "query_" + parts[1], "");
            }
            string_q contents = asciiFileToString(templateFile);
            replaceAll(contents, "[{DATAURL}]", page.dataUrl);
            replaceAll(contents, "[{DATAQUERY}]", page.dataQuery);
            replaceAll(contents, "[{CMDURL}]", page.cmdUrl);
            replaceAll(contents, "[{LONG}]", page.longName);
            replaceAll(contents, "[{PROPER}]", page.properName);
            string_q singular = page.properName;
            replaceReverse(singular, "s", "");
            replaceAll(contents, "[{SINGULAR}]", singular);

            // returns true or false depending on if it WOULD HAVE written the file. If 'test'
            // is true, it doesn't actually write the file
            bool wouldHaveWritten = writeTheCode(codewrite_t(codeFile, contents, nspace, 2, test, false, force));
            if (wouldHaveWritten) {
                if (test) {
                    cerr << "File '" << codeFile << "' changed but was not written because of testing." << endl;
                } else {
                    counter.nProcessed++;
                }
            }
        }
    }

    CStringArray pagesStrs;
    explode(pagesStrs, pageList, '|');

    ostringstream importStream;
    ostringstream pageStream;
    ostringstream menuStream;
    ostringstream appImportsStream;
    ostringstream appReducersStream;
    ostringstream appStateStream;
    ostringstream appDefaultStream;
    ostringstream allSchemasStream;
    ostringstream useSchemasStream;

    menuStream << "  items: [" << endl;
    for (auto pageStr : pagesStrs) {
        CPage page = pageMap[pageStr];
        cerr << "\tProcessing " << page.longName << "..."
             << "\r";
        bool isSeparator = page.longName == "separator";
        if (isSeparator) {
            menuStream << "    { label: '" << page.properName << "' }," << endl;
        } else {
            importStream << "import { " << page.properName << " from ./" << page.properName << "/" << page.properName
                         << "';" << endl;

            menuStream << "    {" << endl;
            menuStream << "      label: '" << page.properName << "'," << endl;
            menuStream << "      exact: true," << endl;
            if (page.subpages.size() == 1 && page.subpages[0].route == "/")
                menuStream << "      route: '/'," << endl;
            if (page.subpages.size() > 1)
                menuStream << "      items: [" << endl;

            for (auto sub : page.subpages) {
                if (!sub.isSeparator) {
                    pageStream << "  '" << page.longName << "/"
                               << (sub.route.empty() ? toLower(sub.subpage) : sub.route == "/" ? "" : sub.route)
                               << "': { component: <" << page.properName << " /> }," << endl;
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
                menuStream << "      ]," << endl;
            menuStream << "    }," << endl;

            appImportsStream << "import { " << page.longName << "Default, " << page.longName << "Reducer } from 'pages/"
                             << page.properName << "/" << page.properName << "';" << endl;

            appReducersStream << "  const [" << page.longName << "State, " << page.longName << "Dispatch] = useReducer("
                              << page.longName << "Reducer, defaultData['" << page.longName << "']);" << endl;

            appStateStream << "    " << page.longName << ": { " << page.longName << ": " << page.longName
                           << "State, dispatch: " << page.longName << "Dispatch }," << endl;

            appDefaultStream << "  " << page.longName << ": " << page.longName << "Default," << endl;

            allSchemasStream << "import { " << page.longName << "Schema } from 'pages/" << page.properName << "/" << page.properName << "';" << endl;

            useSchemasStream << "    { group: 'pages_', name: '" << page.longName << "Schema', schema: " << page.longName << "Schema }," << endl;
        }
    }
    menuStream << "  ]," << endl;

    {
        string_q indexFile = "./pages/index.jsx";
        string_q contents = asciiFileToString(indexFile);
        string_q orig = contents;
        doReplace(contents, "imports", importStream.str(), "  ");
        doReplace(contents, "pages", pageStream.str(), "  ");
        doReplace(contents, "menus", menuStream.str(), "  ");
        if (orig != contents) {
            LOG_INFO("Writing: ", cTeal, indexFile, cOff);
            stringToAsciiFile(indexFile, contents);
        }
    }

    {
        string_q appFile = "./App.jsx";
        string_q contents = asciiFileToString(appFile);
        string_q orig = contents;
        doReplace(contents, "imports", appImportsStream.str(), "");
        doReplace(contents, "reducers", appReducersStream.str(), "  ");
        doReplace(contents, "state", appStateStream.str(), "    ");
        doReplace(contents, "defaults", appDefaultStream.str(), "  ");
        if (orig != contents) {
            LOG_INFO("Writing: ", cTeal, appFile, cOff);
            stringToAsciiFile(appFile, contents);
        }
    }

    {
        string_q systemFile = "./pages/Settings/SettingsSchemas.jsx";
        string_q contents = asciiFileToString(systemFile);
        string_q orig = contents;
        doReplace(contents, "all-schemas", allSchemasStream.str(), "    ");
        cout << useSchemasStream.str() << endl;
        doReplace(contents, "use-schemas", useSchemasStream.str(), "    ");
        if (orig != contents) {
            LOG_INFO("Writing: ", cTeal, systemFile, cOff);
            stringToAsciiFile(systemFile, contents);
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
bool visitHelpFile(const string& path, void* data) {
    if (endsWith(path, "/")) {
        return forEveryFileInFolder(path + "*", visitHelpFile, data);
    } else {
        CStringArray* array = (CStringArray*)data;
        array->push_back(path);
    }
    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_help(void) {
    cout << endl << cYellow << "Checking help files..." << cOff << endl;

    string_q helpFolder = "./api/help";
    CStringArray files;
    forEveryFileInFolder(helpFolder, visitHelpFile, &files);
    string_q str = "|";
    CStringArray tomls;
    for (auto file : files) {
        str += (file + "|");
        string_q f = substitute(substitute(file, helpFolder, ""), ".md", "");
        replace(f, "/", "");
        if (countOf(f, '/') == 0)
            tomls.push_back(f);
    }
    str = substitute(substitute(str, helpFolder + "/", ""), ".md", "");
    // cout << "files: " << str << endl;

    string_q out;
    for (auto t : tomls) {
        CPage page = pageMap[t];
        for (auto sub : page.subpages) {
            string_q route =
                page.longName + "/" + (sub.route.empty() ? toLower(sub.subpage) : sub.route == "/" ? "" : sub.route);
            if (endsWith(route, "/"))
                replaceReverse(route, "/", "");
            if (!contains(str, "|" + route + "|") && !contains(route, "separator"))
                out += (route + "|");
            replace(str, "|" + route + "|", "|");
        }
    }
    str = trim(str, '|');

    cerr << "\tExtraneous files: " << (str.empty() ? "none" : (cRed + substitute(str, "|", ", ") + cOff)) << endl;
    cerr << "\tMissing files: " << (out.empty() ? "none" : (cRed + substitute(out, "|", ", ") + cOff)) << endl;
    cerr << endl;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_schemas(void) {
    CSchema::registerClass();
    cerr << cYellow << "\nGenerating Schemas..." << cOff << endl;

    CToml toml("./classDefinitions/app.toml");
    string_q pageList = toml.getConfigStr("settings", "schemas", "");

    CStringArray pagesStrs;
    explode(pagesStrs, pageList, '|');

    for (auto pageStr : pagesStrs) {
        CStringArray parts;
        explode(parts, pageStr, '-');
        CPage page = pageMap[parts[0]];
        if (parts[0] == "menu") {
            CPage pp;
            pp.properName = "Menu";
            pp.longName = "menu";
            page = pp;
        }
        if (page.longName != "separator") {
            string_q codeFile = "./pages/" + page.properName + "/" + page.properName +
                                (parts.size() > 1 ? toProper(parts[1]) : "") + ".jsx";
            if (page.longName == "menu")
                codeFile = "./pages/index.jsx";
            string_q contents = asciiFileToString(codeFile);
            string_q orig = contents;
            if (contains(contents, "auto-generate")) {
                ostringstream schemaStream;
                cerr << "\tProcessing " << page.longName << (parts.size() > 1 ? "-" + parts[1] : "") << "..."
                     << "\r";
                string_q schemaFile =
                    "./classDefinitions/schemas/" + page.longName + (parts.size() > 1 ? "-" + parts[1] : "") + ".csv";
                string_q data = asciiFileToString(schemaFile);
                CSchemaArray schemas;
                CStringArray lines;
                explode(lines, data, '\n');

                CStringArray fields;
                for (auto line : lines) {
                    if (fields.empty())
                        explode(fields, line, ',');
                    else {
                        CSchema schema;
                        schema.parseCSV(fields, line);
                        schemas.push_back(schema);
                    }
                }
                if (!page.recordIcons.empty()) {
                    CSchema schema;
                    string_q line = "Icons,icons,icons";
                    schema.parseCSV(fields, line);
                    schemas.push_back(schema);
                }
                schemaStream << "export const " << (parts.size() > 1 ? parts[1] : page.longName) << "Schema = [";
                bool first = true;
                for (auto schema : schemas) {
                    if (!first)
                        schemaStream << ",";
                    schemaStream << endl << "  ";
                    expContext().quoteKeys = false;
                    expContext().endingCommas = true;
                    ostringstream os;
                    expContext().lev++;
                    schema.doExport(os);
                    expContext().lev--;
                    string_q str = os.str();
                    replaceAll(str, "\"", "'");
                    replaceAll(str, "'getFieldValue'", "getFieldValue");
                    replaceAll(str, "'useFieldValue'", "useFieldValue");
                    if (contains(str, "onValidate:")) {
                        replaceAll(str, "'validateUserInput'", "validateUserInput");
                        replaceAll(str, "_C_", ",");
                    }
                    if (contains(str, "function:")) {
                        replaceAll(str, "function: '(record)", "function: (record)");
                        replaceAll(str, "\\n}'", "\\n}");
                        replaceAll(str, "\\n", "\n    ");
                        replaceAll(str, "\\t", "  ");
                        replaceAll(str, "_C_", ",");
                    }
                    schemaStream << str;
                    first = false;
                }
                schemaStream << (schemas.size() > 0 ? ",\n" : "") << "];" << endl;
                doReplace(contents, "schema", schemaStream.str(), "");

                ostringstream dataStream;

                if (!page.recordIcons.empty()) {
                    CStringArray icons;
                    explode(icons, page.recordIcons, ',');
                    dataStream << "const recordIconList = [" << endl;
                    for (auto icon : icons)
                        dataStream << "  '" << icon << "'," << endl;
                    dataStream << "  //\n];" << endl;
                }

                if (!page.defaultSort.empty()) {
                    CStringArray sorts;
                    explode(sorts, page.defaultSort, ',');
                    dataStream << "const defaultSort = [";
                    first = true;
                    for (auto sort : sorts) {
                        if (!first)
                            dataStream << ", ";
                        dataStream << "'" << sort << "'";
                        first = false;
                    }
                    dataStream << "];" << endl;
                }

                if (!page.defaultSearch.empty()) {
                    CStringArray searches;
                    explode(searches, page.defaultSearch, ',');
                    dataStream << "const defaultSearch = [";
                    first = true;
                    for (auto search : searches) {
                        if (!first)
                            dataStream << ", ";
                        dataStream << "'" << search << "'";
                        first = false;
                    }
                    dataStream << "];" << endl;
                }

                doReplace(contents, "page-settings", dataStream.str(), "");
                if (orig != contents) {
                    LOG_INFO("Writing: ", cTeal, codeFile, cOff);
                    stringToAsciiFile(codeFile, contents);
                }
            }
        }
    }

    return true;
}
