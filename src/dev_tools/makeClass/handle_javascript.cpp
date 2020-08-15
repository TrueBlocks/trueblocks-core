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

//---------------------------------------------------------------------------------------------------
bool COptions::handle_export_js(void) {
    for (auto classDef : classDefs) {
        CToml toml("");
        toml.readFile(classDef.input_path);
        classDef = CClassDefinition(toml);
        handle_initialize_js(toml, classDef);
    }

    handle_generate_js_pages();
    handle_generate_js_menus();
    handle_generate_js_help();
    handle_generate_js_skins();

    return false;  // we're done processing
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_pages(void) {
    CToml toml("./classDefinitions/app.toml");

    cerr << cYellow << "\nGenerating Pages..." << cOff << endl;
    CStringArray sourceStrs;
    string_q sourceList = toml.getConfigStr("settings", "from_source", "");
    explode(sourceStrs, sourceList, '|');
    for (auto sourceStr : sourceStrs) {
        CStringArray parts;
        explode(parts, sourceStr, '-');
        CPage page = pageMap[parts[0]];
        if (page.longName != "separator") {
            string_q codeSource = "./pages/" + page.properName + "/" + page.properName +
                                  (parts.size() > 1 ? toProper(parts[1]) : "") + ".jsx";
            cerr << "\tProcessing " << page.longName << "...\r";
            string_q templateFile = "./classDefinitions/templates/page-template.jsx";
            if (parts[0] == "explorer") {
                templateFile = "./classDefinitions/templates/page-explorer-template.jsx";
                page.longName = parts[1];
                page.properName = toProper(parts[1]);
                page.singular = page.properName;
                replaceReverse(page.singular, "s", "");
                CToml t("./classDefinitions/" + parts[0] + ".toml");
                page.dataQuery = t.getConfigStr("settings", "query_" + parts[1], "");
            }

            string_q templateContents = asciiFileToString(templateFile);
            replaceAll(templateContents, "[{DEFAULT_TAGS}]", page.defaultTags);
            replaceAll(templateContents, "[{DATAURL}]", page.dataUrl);
            replaceAll(templateContents, "[{DATAQUERY}]", page.dataQuery);
            replaceAll(templateContents, "[{DEFAULT_TABLE}]", page.defaultTable);
            replaceAll(templateContents, "[{CMDURL}]",
                       page.cmdUrl == "none" ? "" : "\n  const cmdUrl = getApiUrl('" + page.cmdUrl + "');");
            replaceAll(templateContents, "[{DELETE_CMD}]", page.cmdUrl == "none" ? "" : STR_DELETE_CMDS);
            replaceAll(templateContents, "[{DISPATCH}]", page.cmdUrl == "none" ? "" : "dispatch, ");
            replaceAll(templateContents, "[{LONG}]", page.longName);
            replaceAll(templateContents, "[{PROPER}]", page.properName);
            replaceAll(templateContents, "[{SINGULAR}]", page.singular);

            CStringArray importArray;
            explode(importArray, page.imports, '|');

            string_q frag = importArray[0].empty() || importArray[0] == "none" ? "" : (importArray[0] + " ");
            replaceAll(templateContents, "[{FRAGMENT}]", frag);

            string_q tables =
                importArray[1].empty() || importArray[1] == "none" ? "DataTable, PageCaddie" : importArray[1];
            replaceAll(templateContents, "[{TABLE_IMPORT}]", tables);

            string_q utils =
                importArray[2].empty() || importArray[2] == "none"
                    ? "getServerData, sendServerCommand, sortArray, sortStrings, handleClick, navigate, replaceRecord"
                    : importArray[2];
            replaceAll(templateContents, "[{UTILS_IMPORT}]", utils);

            string_q status_store =
                importArray[3].empty() || importArray[3] == "none" ? "useStatus, LOADING, NOT_LOADING" : importArray[3];
            replaceAll(templateContents, "[{STATUS_STORE_IMPORT}]", status_store);

            ostringstream dataStream;
            bool first = true;

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

            doReplace(templateContents, "page-settings", dataStream.str(), "");

            // returns true or false depending on if it WOULD HAVE written the file. If 'test'
            // is true, it doesn't actually write the file
            bool wouldHaveWritten =
                writeTheCode(codewrite_t(codeSource, templateContents, nspace, 2, test, false, force));
            if (wouldHaveWritten) {
                if (test) {
                    cerr << "File '" << codeSource << "' changed but was not written because of testing." << endl;
                } else {
                    counter.nProcessed++;
                }
            }
        }
    }

    cerr << cYellow << "\nGenerating Schemas..." << cOff << endl;

    CStringArray schemaStrs;
    string_q schemaList = toml.getConfigStr("settings", "schemas", "");
    explode(schemaStrs, schemaList, '|');
    for (auto schemasStr : schemaStrs) {
        CStringArray parts;
        explode(parts, schemasStr, '-');
        CPage page = pageMap[parts[0]];
        if (page.longName != "separator") {
            string_q schemaSource = "./pages/" + page.properName + "/" + page.properName +
                                    (parts.size() > 1 ? toProper(parts[1]) : "") + "Schema.jsx";

            if (!fileExists(schemaSource)) {
                string_q emptySchema = "./classDefinitions/templates/emptySchema.jsx";
                string_q source = asciiFileToString(emptySchema);
                stringToAsciiFile(schemaSource, source);
            }

            string_q schemaContents = asciiFileToString(schemaSource);
            string_q origSchema = schemaContents;

            if (contains(schemaContents, "auto-generate")) {
                ostringstream schemaStream;
                cerr << "\tProcessing " << schemaSource << "...\r";
                string_q schemaCSV =
                    "./classDefinitions/schemas/" + page.longName + (parts.size() > 1 ? "-" + parts[1] : "") + ".csv";
                string_q schemaData = asciiFileToString(schemaCSV);
                CSchemaArray schemas;
                CStringArray lines;
                explode(lines, schemaData, '\n');

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
                    string_q line = "Icons,icons,icons,true";
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
                    replaceAll(str, "'getExportValue'", "getExportValue");
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

                doReplace(schemaContents, "schema", schemaStream.str(), "");
                if (origSchema != schemaContents) {
                    LOG_INFO("Writing Schema: ", cTeal, schemaSource, cOff);
                    stringToAsciiFile(schemaSource, schemaContents);
                }
            }
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
    cerr << endl << cYellow << "Checking help files..." << cOff << endl;

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
const char* STR_DELETE_CMDS =
    ""
    "        case 'delete':\n"
    "          {\n"
    "            const cmdQuery = 'editCmd=delete&terms=' + action.record_id + addendum(record, action.record_id);\n"
    "            statusDispatch(LOADING);\n"
    "            dispatch(action);\n"
    "            sendServerCommand(cmdUrl, cmdQuery).then(() => {\n"
    "              // we assume the delete worked, so we don't reload the data\n"
    "              statusDispatch(NOT_LOADING);\n"
    "            });\n"
    "          }\n"
    "          break;\n"
    "        case 'undelete':\n"
    "          {\n"
    "            const cmdQuery = 'editCmd=undelete&terms=' + action.record_id + addendum(record, action.record_id);\n"
    "            statusDispatch(LOADING);\n"
    "            dispatch(action);\n"
    "            sendServerCommand(cmdUrl, cmdQuery).then(() => {\n"
    "              // we assume the delete worked, so we don't reload the data\n"
    "              statusDispatch(NOT_LOADING);\n"
    "            });\n"
    "          }\n"
    "          break;\n"
    "        case 'remove':\n"
    "          {\n"
    "            const cmdQuery = 'editCmd=remove&terms=' + action.record_id + addendum(record, action.record_id);\n"
    "            statusDispatch(LOADING);\n"
    "            sendServerCommand(cmdUrl, cmdQuery).then((theData) => {\n"
    "              // the command worked, but now we need to reload the data\n"
    "              refresh[{PROPER}]Data(dataQuery, dispatch, mocked);\n"
    "              statusDispatch(NOT_LOADING);\n"
    "            });\n"
    "          }\n"
    "          break;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_DEFAULT_TAGS =
    "sortStrings([...new Set([{LONG}].data.map((item) => calcValue(item, { selector: 'tags', onDisplay: getFieldValue "
    "})))], true)";

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_skins(void) {
    return true;
}

/*
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

*/

//---------------------------------------------------------------------------------------------------
bool COptions::handle_initialize_js(CToml& toml, const CClassDefinition& classDef) {
    CPage page;
    page.longName = classDef.base_lower;
    page.properName = classDef.base_proper;
    page.singular = page.properName;
    page.noPage = toml.getConfigBool("settings", "noPage", false);
    page.dataUrl = toml.getConfigStr("settings", "dataUrl", "");
    page.dataQuery = toml.getConfigStr("settings", "dataQuery", "");
    page.cmdUrl = toml.getConfigStr("settings", "cmdUrl", "none");
    page.imports = toml.getConfigStr("settings", "imports", "none|none|none|none|");
    page.dest_path = toml.getConfigStr("settings", "dest_path", "./pages/") + page.properName + "/";
    page.schema = toml.getConfigStr("settings", "schema", "./" + page.longName + ".csv");
    page.defaultTable = toml.getConfigStr("settings", "defaultTable", "DataTable");
    page.defaultSort = toml.getConfigStr("settings", "defaultSort", "");
    page.defaultSearch = toml.getConfigStr("settings", "defaultSearch", page.defaultSort);
    page.defaultTags = toml.getConfigStr("settings", "defaultTags", STR_DEFAULT_TAGS);
    replaceReverse(page.singular, "s", "");

    string_q recordIcons = substitute("," + toml.getConfigStr("settings", "recordIcons", "") + ",", ",,", "");
    replace(recordIcons, "editing,", "header-Add,Delete/Undelete,Edit/Remove,");
    replace(recordIcons, "viewing,", "ExternalLink,");
    replace(recordIcons, "view,", "View/None,");
    replace(recordIcons, "follow,", "AddMonitor/None/View,");
    replace(recordIcons, "exporting,", "footer-CSV,footer-TXT,");
    replace(recordIcons, "importing,", "footer-Import,");
    page.recordIcons = trim(recordIcons, ',');

    CSubpage item;
    string_q subpages = toml.getConfigJson("subpages", "list", "");
    while (item.parseJson3(subpages)) {
        replaceAll(item.options, "_EQ_", "=");
        replaceAll(item.extract, "_0", "[0]");
        page.subpages.push_back(item);
        item = CSubpage();
    }

    pageMap[page.longName] = page;
    if (page.longName.empty() || page.longName == "separator")
        return false;

    establishFolder(page.dest_path);
    string_q cssFile = page.dest_path + page.properName + ".css";
    if (!fileExists(cssFile)) {
        string_q emptyCss = "./classDefinitions/templates/empty.css";
        stringToAsciiFile(cssFile, substitute(asciiFileToString(emptyCss), "[{PROPER}]", page.properName));
    }

    return false;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_js_menus(void) {
    cerr << cYellow << "Generating Menus...                                                          " << cOff << endl;

    CToml toml("./classDefinitions/app.toml");

    CStringArray pagesStrs;
    string_q pageList = toml.getConfigStr("settings", "pages", "");
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
            ostringstream tmpMenu;
            importStream << "import { " << page.properName << " from ./" << page.properName << "/" << page.properName
                         << "';" << endl;

            tmpMenu << "    {" << endl;
            tmpMenu << "      label: '" << page.properName << "'," << endl;
            tmpMenu << "      exact: true," << endl;
            if (page.subpages.size() == 1 && page.subpages[0].route == "/")
                tmpMenu << "      route: '/'," << endl;
            if (page.subpages.size() > 1)
                tmpMenu << "      items: [" << endl;

            for (auto sub : page.subpages) {
                if (!sub.isSeparator && !page.noPage) {
                    pageStream << "  '" << page.longName << "/"
                               << (sub.route.empty() ? toLower(sub.subpage) : sub.route == "/" ? "" : sub.route)
                               << "': { component: "
                               << (sub.component.empty() ? ("<" + page.properName + " />") : sub.component) << " },"
                               << endl;
                }

                if (sub.isSeparator)
                    tmpMenu << "        { label: 'Separator' }," << endl;
                else if (sub.subpage != "") {
                    tmpMenu << "        { label: '" << sub.subpage << "'";
                    if (!sub.route.empty())
                        tmpMenu << ", route: '" << sub.route << "'";
                    tmpMenu << " }," << endl;
                }
            }
            pageStream << "  //" << endl;
            if (page.subpages.size() > 1)
                tmpMenu << "      ]," << endl;
            tmpMenu << "    }," << endl;

            appImportsStream << "import { " << page.longName << "Default, " << page.longName << "Reducer } from 'pages/"
                             << page.properName << "/" << page.properName << "';" << endl;

            appReducersStream << "  const [" << page.longName << "State, " << page.longName << "Dispatch] = useReducer("
                              << page.longName << "Reducer, defaultData['" << page.longName << "']);" << endl;

            appStateStream << "    " << page.longName << ": { " << page.longName << ": " << page.longName
                           << "State, dispatch: " << page.longName << "Dispatch }," << endl;

            appDefaultStream << "  " << page.longName << ": " << page.longName << "Default," << endl;

            allSchemasStream << "import { " << page.longName << "Schema } from 'pages/" << page.properName << "/"
                             << page.properName << "';" << endl;

            useSchemasStream << "    { group: 'pages_', name: '" << page.longName
                             << "Schema', schema: " << page.longName << "Schema }," << endl;

            if (!page.noPage || isSeparator)
                menuStream << tmpMenu.str();
        }
    }
    menuStream << "  ]," << endl;

    {
        string_q indexFile = "./pages/index.jsx";
        string_q indexContents = asciiFileToString(indexFile);
        string_q orig = indexContents;
        doReplace(indexContents, "imports", importStream.str(), "  ");
        doReplace(indexContents, "pages", pageStream.str(), "  ");
        doReplace(indexContents, "menus", menuStream.str(), "  ");
        if (orig != indexContents) {
            LOG_INFO("Writing: ", cTeal, indexFile, cOff);
            stringToAsciiFile(indexFile, indexContents);
        }
    }

    {
        string_q appFile = "./App.jsx";
        string_q appContents = asciiFileToString(appFile);
        string_q orig = appContents;
        doReplace(appContents, "imports", appImportsStream.str(), "");
        doReplace(appContents, "reducers", appReducersStream.str(), "  ");
        doReplace(appContents, "state", appStateStream.str(), "    ");
        doReplace(appContents, "defaults", appDefaultStream.str(), "  ");
        if (orig != appContents) {
            LOG_INFO("Writing: ", cTeal, appFile, cOff);
            stringToAsciiFile(appFile, appContents);
        }
    }

    {
        string_q systemFile = "./pages/Settings/SettingsSchemas.jsx";
        string_q systemContents = asciiFileToString(systemFile);
        string_q orig = systemContents;
        doReplace(systemContents, "all-schemas", allSchemasStream.str(), "    ");
        doReplace(systemContents, "use-schemas", useSchemasStream.str(), "    ");
        if (orig != systemContents) {
            LOG_INFO("Writing: ", cTeal, systemFile, cOff);
            stringToAsciiFile(systemFile, systemContents);
        }
    }

    return true;
}
