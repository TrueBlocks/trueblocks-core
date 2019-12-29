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
bool COptions::handle_json_export(void) {
    for (auto classDef : classDefs) {
        CBaseNode* item = createObjectOfType(classDef.short_fn);
        if (item) {
            CRuntimeClass* pClass = item->getRuntimeClass();
            pClass->forEveryField(visitField, &cout);
        } else {
            CToml toml(classDef.input_path);
            toml.readFile(classDef.input_path);
            handle_generate(toml, classDef, nspace, true);
        }
    }
    return false;  // we're done processing
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_generate_frontend(const CJavascriptDef& def) {
    string_q destFolder = "../" + def.longName + "/";
    establishFolder(destFolder);
    CStringArray files;
    explode(files, def.files, '|');
    for (auto file : files)
        handle_one_frontend_file(def, destFolder, (file == "index" ? file : "blank-" + file) + ".js");
    string_q ccsFile = destFolder + def.longName + ".css";
    if (!fileExists(ccsFile))
        copyFile("./blank.css", ccsFile);
    return false;
}

//---------------------------------------------------------------------------------------------------
bool COptions::handle_one_frontend_file(const CJavascriptDef& def, const string_q& folder, const string_q& source) {
    string_q destFile = folder + substitute(source, "blank-", "");
    string_q sourceFile = "./" + source;
    string_q code = asciiFileToString(sourceFile);
    replaceAll(code, "[{CONNECT}]", def.polling ? STR_EXPORT_2 : STR_EXPORT_1);
    replaceAll(code, "[{LONG}]", def.longName);
    replaceAll(code, "[{SEVEN}]", padRight(def.longName.substr(0, 7), 7, '_'));
    replaceAll(code, "[{TWO1}]", toUpper(def.longName.substr(0, 2)));
    replaceAll(code, "[{TWO2}]", toLower(def.longName.substr(0, 2)));
    replaceAll(code, "[{PAGENOTES}]", substitute(def.pageNotes, "|", " \n            "));
    replaceAll(code, "[{PROPER}]", def.properName);
    replaceAll(code, "[{SUBPAGE}]", def.subpage);
    replaceAll(code, "[{QUERY_URL}]", def.query_url);
    replaceAll(code, "[{QUERY_OPTS}]", def.query_opts);
    replaceAll(code, "[{QUERY_EXTRACT}]", def.query_extract);
    replaceAll(code, "[{MENU_TYPE}]", def.menuType);
    replaceAll(code, "[{MENU_FILE}]", def.menuType == "LocalMenu" ? "local-menu" : "summary-table");
    replaceAll(code, "[{POLLING}]", def.polling ? STR_POLLING : "");

    string_q thing = def.subpage;
    replaceAll(code, "[{STATE_FIELDS_2}]", nextTokenClear(thing, ':') + ": value");
    // w riteTheCode returns true or false depending on if it WOULD HAVE written the file. If 'test' is true, it doesn't
    // actually write the file
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
