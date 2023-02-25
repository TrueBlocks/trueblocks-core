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

string_q type_2_GoType(const CMember& field);
string_q specialCase(const CMember& field, const string_q& name, const string_q& type, bool isRaw);
string_q debug(const CMember& field);
bool skipField(const CMember& field);

//------------------------------------------------------------------------------------------------------------
void generate_go_type(COptions* opts, const CClassDefinition& modelIn) {
    CClassDefinition model = modelIn;

    string_q fn = getPathToSource("apps/chifra/pkg/types/types_" + toLower(model.base_name) + ".go");
    string_q contents = asciiFileToString(getPathToTemplates("blank_type.go.tmpl"));
    replaceAll(contents, "[{CLASS_NAME}]", type_2_ModelName(model.go_model, false));
    replaceAll(contents, "[{RAW_NAME}]", "Raw" + type_2_ModelName(model.go_model, true));

    CMember raw;
    raw.type = "*Raw" + type_2_ModelName(model.go_model, true);
    raw.name = "raw";
    model.fieldArray.push_back(raw);

    size_t maxNameWid = 0, maxSimpWid = 0, maxRawWid = 0;
    for (auto& field : model.fieldArray) {
        if (skipField(field))
            continue;
        string_q type = type_2_GoType(field);
        string_q rawType = specialCase(field, field.name, type, true);
        string_q simpType = specialCase(field, field.name, type, false);
        maxNameWid = max(maxNameWid, field.name.length());
        if (field.name != "raw") {
            maxSimpWid = max(maxSimpWid, simpType.length());
            maxRawWid = max(maxRawWid, rawType.length());
            field.name = firstUpper(field.name);
            field.value = field.name;  // we need it and use it below
            if (field.name == "Type") {
                field.name = model.base_name + "Type";
            }
        }
    }

    string_q rawStr;
    for (auto field : model.fieldArray) {
        if (skipField(field))
            continue;
        if (!(field.name % "raw")) {
            string_q type = type_2_GoType(field);
            string_q spec = specialCase(field, field.name, type, true);
            string_q rawType = field.name % "raw" ? spec : padRight(spec, maxRawWid);
            if (spec.empty())
                continue;
            ostringstream os;
            os << "\t";
            os << padRight(field.name, maxNameWid) << " " << rawType;
            os << " `json:\"" << firstLower(field.value) << "\"`" << debug(field) << endl;
            rawStr += os.str();
        }
    }

    string_q fieldStr;
    for (auto field : model.fieldArray) {
        if (skipField(field))
            continue;
        string_q type = type_2_GoType(field);
        string_q spec = specialCase(field, field.name, type, false);
        string_q simpType = (field.name % "raw") ? spec : padRight(spec, maxSimpWid);
        ostringstream os;
        os << "\t" << padRight(field.name, maxNameWid) << " " << simpType;
        if (!(field.name % "raw")) {
            os << " `json:\"" << firstLower(field.value) << (field.is_flags & IS_OMITEMPTY ? ",omitempty" : "") << "\"`"
               << debug(field);
        }
        os << endl;
        fieldStr += os.str();
    }

    string_q modelStr;
    for (auto field : model.fieldArray) {
        if (skipField(field))
            continue;
        ostringstream os;
        if (!(field.name % "raw") && !(field.is_flags & (IS_OMITEMPTY | IS_ARRAY))) {
            os << "\t\t" << padRight("\"" + firstLower(field.value) + "\":", maxNameWid + 3) << " s."
               << firstUpper(field.name) << "," << debug(field) << endl;
        }
        modelStr += os.str();
    }

    string_q orderStr;
    for (auto field : model.fieldArray) {
        if (skipField(field))
            continue;
        ostringstream os;
        if (!(field.name % "raw") && !(field.is_flags & (IS_OMITEMPTY | IS_ARRAY))) {
            os << "\t\t\"" << firstLower(field.value) << "\"," << debug(field) << endl;
        }
        orderStr += os.str();
    }

    replaceAll(contents, "[{RAWFIELDS}]", rawStr);
    replaceAll(contents, "[{FIELDS}]", fieldStr);
    replaceAll(contents, "[{MODEL_FIELDS}]", modelStr);
    replaceAll(contents, "[{ORDER_FIELDS}]", orderStr);

    codewrite_t cw(fn, contents + "\n");
    cw.nSpaces = 0;
    writeCodeIn(opts, cw);
}

//------------------------------------------------------------------------------------------------------------
string_q type_2_GoType(const CMember& field) {
    string_q type = field.type;
    if (type == "blknum")
        return "uint64";
    if (type == "timestamp")
        return "int64";
    if (type == "hash" || type == "bytes32")
        return "common.Hash";
    if (type == "datetime" || type == "bytes")
        return "string";
    if (type == "address")
        return "Address";
    if (type == "gas")
        return "Gas";
    if (type == "wei")
        return "Wei";
    if (type == "int256")
        return "big.Int";
    if (type == "double")
        return "float64";
    return type;
}

//------------------------------------------------------------------------------------------------------------
string_q specialCase(const CMember& field, const string_q& name, const string_q& type, bool isRaw) {
    string ret;
    if (name % "CumulativeGasUsed" && !isRaw) {
        ret = "string";
    } else if (name % "Logs") {
        ret = isRaw ? "[]RawLog" : "[]SimpleLog";
    } else if (name % "Action") {
        ret = isRaw ? "RawTraceAction" : "*SimpleTraceAction";
    } else if (name % "Components" || name % "Inputs" || name % "Outputs") {
        ret = isRaw ? "string" : "[]SimpleParameter";
    } else if (name % "Result") {
        ret = isRaw ? "RawTraceResult" : "*SimpleTraceResult";
    } else if (name % "ArticulatedTrace") {
        ret = isRaw ? "" : "*SimpleFunction";
    } else {
        ret = (type == "CStringArray" ? "[]string" : isRaw ? "string" : type);
    }
    return ret;
}

//------------------------------------------------------------------------------------------------------------
string_q debug(const CMember& field) {
    ostringstream os;
    // os << " //";
    // os << " doc: " << field.doc;
    // os << " disp: " << field.disp;
    // os << " omit: " << (field.is_flags & IS_OMITEMPTY);
    return os.str();
}

//------------------------------------------------------------------------------------------------------------
bool skipField(const CMember& field) {
    return contains(field.name, "::") || field.name == "InputsDict" || field.name == "OutputsDict" ||
           field.name == "Abi_source";
}
