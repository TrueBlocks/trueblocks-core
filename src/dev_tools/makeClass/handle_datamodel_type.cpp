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
string_q type_2_GoType(const string_q& type) {
    if (type == "blknum")
        return "uint64";
    if (type == "timestamp")
        return "int64";
    if (type == "datetime")
        return "string";
    return type;
}

//------------------------------------------------------------------------------------------------------------
void generate_go_code(COptions* opts, const CClassDefinition& model) {
    string_q fn = getPathToSource("apps/chifra/pkg/types/types_" + toLower(model.base_name) + ".go");
    string_q contents = asciiFileToString(getPathToTemplates("blank_type.go.tmpl"));
    replaceAll(contents, "[{CLASS_NAME}]", type_2_ModelName(model.class_name, false));

    ostringstream fieldStream, copyStream, displayStream;
    for (auto field : model.fieldArray) {
        string_q type = type_2_GoType(field.type);
        // if (field.is_flags & IS_ARRAY) {
        //     type = "[]" + type_2_ModelName(type, false);
        // } else if (field.is_flags & IS_OBJECT) {
        //     type = "*" + type_2_ModelName(type, false);
        // }
        bool isOmitEmpty = (field.is_flags & IS_OMITEMPTY);
        fieldStream << "\t" << firstUpper(field.name) << " " << type << " `json:\"" << field.name
                    << (isOmitEmpty ? ",omitempty" : "") << "\"`" << endl;
        if (!isOmitEmpty) {
            copyStream << "\t\t\"" << field.name << "\": s." << firstUpper(field.name) << "," << endl;
            displayStream << "\t\t\"" << field.name << "\""
                          << "," << endl;
        }
    }
    replaceAll(contents, "[{FIELDS}]", fieldStream.str());
    replaceAll(contents, "[{FIELD_COPY}]", copyStream.str());
    replaceAll(contents, "[{FIELD_DISPLAY}]", displayStream.str());

    codewrite_t cw(fn, contents);
    cw.nSpaces = 0;
    writeCodeIn(opts, cw);
    // cerr << fn << endl;
}
