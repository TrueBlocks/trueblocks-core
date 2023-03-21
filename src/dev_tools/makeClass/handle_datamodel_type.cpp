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
string_q specialCase(const string_q& modelName, const CMember& field, const string_q& type, bool isRaw);
bool skipField(const CMember& field, bool raw);

//------------------------------------------------------------------------------------------------------------
void generate_go_type(COptions* opts, const CClassDefinition& modelIn) {
    CClassDefinition model = modelIn;
    CClassDefinition modelOrig = modelIn;
    for (size_t i = 0; i < model.fieldArray.size(); i++) {
        modelOrig.fieldArray[i] = modelIn.fieldArray[i];
    }

    string_q fn = getPathToSource("apps/chifra/pkg/types/types_" + toLower(model.base_name) + ".go");
    string_q contents = asciiFileToString(getPathToTemplates("blank_type.go.tmpl"));
    replaceAll(contents, "[{CLASS_NAME}]", type_2_ModelName(model.go_model, false));
    replaceAll(contents, "[{RAW_NAME}]", "Raw" + type_2_ModelName(model.go_model, true));

    sort(model.fieldArray.begin(), model.fieldArray.end());

    CMember raw;
    raw.type = "*Raw" + type_2_ModelName(model.go_model, true);
    raw.name = "raw";
    model.fieldArray.push_back(raw);

    size_t maxNameWid = 0, maxModelWid = 0, maxSimpWid = 0, maxRawWid = 0;
    size_t fieldNo = 0;
    for (CMember& field : model.fieldArray) {
        if (skipField(field, false)) {
            fieldNo++;
            continue;
        }
        string_q type = type_2_GoType(field);
        string_q rawType = specialCase(model.base_name, field, type, true);
        string_q simpType = specialCase(model.base_name, field, type, false);
        maxSimpWid = max(maxSimpWid, simpType.length());
        maxRawWid = max(maxRawWid, rawType.length());
        if (field.name != "raw") {
            field.name = firstUpper(field.name);
        }
        field.value = field.name;  // we need it and use it below
        if (field.name == "Type") {
            field.name = model.base_name + "Type";
        }
        if (fieldNo < modelOrig.fieldArray.size()) {
            modelOrig.fieldArray[fieldNo].name = firstUpper(modelOrig.fieldArray[fieldNo].name);
            modelOrig.fieldArray[fieldNo].value = modelOrig.fieldArray[fieldNo].name;  // we need it and use it below
            if (modelOrig.fieldArray[fieldNo].name == "Type") {
                modelOrig.fieldArray[fieldNo].name = modelOrig.base_name + "Type";
            }
        }
        maxNameWid = max(maxNameWid, field.name.length());
        if (!(field.memberFlags & IS_OMITEMPTY)) {
            maxModelWid = max(maxModelWid, field.name.length());
        }
        fieldNo++;
    }

    string_q rawStr;
    for (const CMember& field : model.fieldArray) {
        if (skipField(field, true))
            continue;
        if (!(field.name % "raw")) {
            string_q type = type_2_GoType(field);
            string_q spec = specialCase(model.base_name, field, type, true);
            string_q rawType = field.name % "raw" ? spec : padRight(spec, maxRawWid);
            if (spec.empty())
                continue;
            ostringstream os;
            os << "\t";
            os << padRight(field.name, maxNameWid) << " " << rawType;
            os << " `json:\"" << firstLower(field.value) << "\"`" << endl;
            rawStr += os.str();
        }
    }

    string_q fieldStr;
    for (const CMember& field : model.fieldArray) {
        if (skipField(field, false))
            continue;
        string_q type = type_2_GoType(field);
        string_q spec = specialCase(model.base_name, field, type, false);
        string_q simpType = padRight(spec, maxSimpWid);
        ostringstream os;
        os << "\t" << padRight(field.name, maxNameWid) << " " << simpType;
        if (!(field.name % "raw")) {
            os << " `json:\"" << firstLower(field.value) << (field.memberFlags & IS_OMITEMPTY ? ",omitempty" : "")
               << "\"`";
        } else {
            os << " `json:\"-\"`";
        }
        os << endl;
        fieldStr += os.str();
    }

    replaceAll(contents, "[{RAWFIELDS}]", rawStr);
    replaceAll(contents, "[{FIELDS}]", fieldStr);

    // hackathon!
    replaceAll(contents, "type SimpleBlock[Tx] struct {", "type SimpleBlock[Tx BlockTransaction] struct {");

    codewrite_t cw(fn, contents + "\n");
    cw.nSpaces = 0;
    writeCodeIn(opts, cw);
}

//------------------------------------------------------------------------------------------------------------
string_q type_2_GoType(const CMember& field) {
    string_q type = field.type;
    if (type == "blknum")
        return "base.Blknum";
    if (type == "timestamp")
        return "base.Timestamp";
    if (type == "hash" || type == "bytes32")
        return "base.Hash";
    if (type == "datetime" || type == "bytes")
        return "string";
    if (type == "address")
        return "base.Address";
    if (type == "gas")
        return "base.Gas";
    if (type == "wei")
        return "base.Wei";
    if (type == "int256")
        return "big.Int";
    if (type == "double")
        return "float64";
    return type;
}

//------------------------------------------------------------------------------------------------------------
string_q specialCase(const string_q& modelName, const CMember& field, const string_q& type, bool isRaw) {
    string_q name = field.name;
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

    } else if ((name % "FromAddress" || name % "ToAddress") && type % "CAddressArray") {
        ret = isRaw ? "string" : "[]string";

    } else if (name % "Transactions") {
        ret = isRaw ? "[]any" : "[]Tx";

    } else if (modelName == "Parameter" && name % "Value") {
        ret = isRaw ? "string" : "any";

    } else {
        ret = (type == "CStringArray" ? "[]string" : isRaw ? "string" : type);
    }
    return ret;
}

//------------------------------------------------------------------------------------------------------------
bool skipField(const CMember& field, bool raw) {
    return contains(field.name, "::") || field.name == "InputsDict" || field.name == "OutputsDict" ||
           field.name == "Abi_source" || (!raw && field.name == "LogsBloom") || (raw && field.name == "IsError");
}
