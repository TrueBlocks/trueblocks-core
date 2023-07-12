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

string_q type_2_GoType(const CMember& field);
string_q specialCase(const CClassDefinition& model, const CMember& field, const string_q& type, bool isRaw);
string_q jsonName(const CClassDefinition& model, const CMember& field, bool raw);
bool skipField(const CClassDefinition& model, const CMember& field, bool raw);

//------------------------------------------------------------------------------------------------------------
void generate_go_type(COptions* opts, const CClassDefinition& modelIn) {
    CClassDefinition model = modelIn;
    CClassDefinition modelOrig = modelIn;
    for (size_t i = 0; i < model.fieldArray.size(); i++) {
        modelOrig.fieldArray[i] = modelIn.fieldArray[i];
    }

    bool isInternal = contains(modelIn.go_output, "/internal/");
    string_q contents = asciiFileToString(getPathToTemplates("blank_type.go.tmpl"));
    if (isInternal) {
        contents = asciiFileToString(getPathToTemplates("blank_type_int.go.tmpl"));
    }
    replaceAll(contents, "[{CLASS_NAME}]", type_2_ModelName(model.go_model, false));
    replaceAll(contents, "[{RAW_NAME}]", "Raw" + type_2_ModelName(model.go_model, true));
    replaceAll(contents, "[{ROUTE}]",
               substitute(substitute(modelIn.go_output, "./apps/chifra/internal/", ""), "/", ""));

    sort(model.fieldArray.begin(), model.fieldArray.end());

    if (!isInternal) {
        CMember raw;
        raw.type = "*Raw" + type_2_ModelName(model.go_model, true);
        raw.name = "raw";
        model.fieldArray.push_back(raw);
    }

    size_t maxRawNameWid = 0, maxSimpNameWid = 0, maxRawTypeWid = 0, maxSimpTypeWid = 0, maxModelWid = 0;

    for (size_t i = 0; i < model.fieldArray.size(); i++) {
        CMember* field = &model.fieldArray[i];

        if (field->name != "raw") {
            field->name = firstUpper(field->name);
        }
        field->value = field->name;  // we need it and use it below
        if (field->name == "Type") {
            field->name = modelOrig.base_name + "Type";
        }

        string_q type = type_2_GoType(*field);
        string_q rawType = specialCase(model, *field, type, true);
        string_q simpType = specialCase(model, *field, type, false);

        if (!skipField(model, *field, true)) {
            maxRawTypeWid = max(maxRawTypeWid, rawType.length());
            maxRawNameWid = max(maxRawNameWid, field->name.length());
            if (!(field->memberFlags & IS_OMITEMPTY)) {
                maxModelWid = max(maxModelWid, field->name.length());
            }
        }

        if (!skipField(model, *field, false)) {
            maxSimpTypeWid = max(maxSimpTypeWid, simpType.length());
            maxSimpNameWid = max(maxSimpNameWid, field->name.length());
            if (!(field->memberFlags & IS_OMITEMPTY)) {
                maxModelWid = max(maxModelWid, field->name.length());
            }
        }
    }

    string_q rawStr;
    const char* STR_JSON_TAG = " `json:\"[{NAME}][{OE}]\"`";
    for (const CMember& field : model.fieldArray) {
        if (skipField(model, field, true))
            continue;

        string_q spec = specialCase(model, field, type_2_GoType(field), true);
        if (spec.empty())
            continue;

        string_q rawType = padRight(spec, maxRawTypeWid);
        string_q jName = jsonName(model, field, true);
        string_q oe = "";

        ostringstream os;
        os << "\t";
        os << padRight(field.name, maxRawNameWid) << " " << rawType;
        os << substitute(substitute(STR_JSON_TAG, "[{NAME}]", jName), "[{OE}]", oe) << endl;
        rawStr += os.str();
    }

    string_q fieldStr;
    for (const CMember& field : model.fieldArray) {
        if (skipField(model, field, false))
            continue;

        string_q spec = specialCase(model, field, type_2_GoType(field), false);
        string_q simpType = padRight(spec, maxSimpTypeWid);
        string_q jName = jsonName(model, field, false);
        string_q oe = (field.memberFlags & IS_OMITEMPTY ? ",omitempty" : "");

        ostringstream os;
        os << "\t" << padRight(field.name, maxSimpNameWid) << " " << simpType;
        os << substitute(substitute(STR_JSON_TAG, "[{NAME}]", jName), "[{OE}]", oe);
        os << endl;
        fieldStr += os.str();
    }

    replaceAll(contents, "[{RAWFIELDS}]", rawStr);
    replaceAll(contents, "[{FIELDS}]", fieldStr);

    // hackathon!
    replaceAll(contents, "type SimpleBlock[Tx] struct {", "type SimpleBlock[Tx BlockTransaction] struct {");

    string_q fn = substitute(
        substitute(getPathToSource(modelIn.go_output + "types_" + toLower(model.base_name) + ".go"), "//", "/"), "/./",
        "/");

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
    if (type == "ipfshash")
        return "base.IpfsHash";
    if (type == "blkrange")
        return "string";
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
string_q specialCase(const CClassDefinition& model, const CMember& field, const string_q& type, bool isRaw) {
    string_q modelName = model.base_name;
    string_q name = field.name;
    string ret;
    if (name % "CumulativeGasUsed" && !isRaw) {
        ret = "string";

    } else if (name % "Emitters") {
        ret = isRaw ? "[]string" : "[]base.Address";

    } else if (name % "Logs") {
        ret = isRaw ? "[]RawLog" : "[]SimpleLog";

    } else if (name % "Action") {
        ret = isRaw ? "RawTraceAction" : "*SimpleTraceAction";

    } else if (modelName % "CallResult" && name % "Outputs") {
        ret = isRaw ? "[]string" : "map[string]string";

    } else if (name % "Components" || name % "Inputs" || name % "Outputs") {
        ret = isRaw ? "string" : "[]SimpleParameter";

    } else if (startsWith(modelName, "Trace") && name % "Result") {
        ret = isRaw ? "*RawTraceResult" : "*SimpleTraceResult";

    } else if (startsWith(name, "Articulated") || name % "CallResult") {
        ret = isRaw ? "" : "*SimpleFunction";

    } else if (name % "Uncles") {
        ret = isRaw ? "[]string" : "[]base.Hash";

    } else if ((name % "FromAddress" || name % "ToAddress") && type % "CAddressArray") {
        ret = isRaw ? "string" : "[]base.Address";

    } else if (name % "Transactions") {
        ret = isRaw ? "[]any" : "[]Tx";

    } else if (modelName % "Manifest" && name % "Chunks") {
        ret = isRaw ? "string" : "[]SimpleChunkRecord";

    } else if (modelName % "Bounds" && (name % "FirstApp" || name % "LatestApp")) {
        ret = isRaw ? "string" : "types.RawAppearance";

    } else if (name % "TraceAddress") {
        ret = "[]uint64";

    } else if (modelName % "Trace" && (name % "Subtraces" || name % "TransactionIndex")) {
        ret = "uint64";

    } else if (modelName % "Trace" && (name % "BlockNumber")) {
        ret = "base.Blknum";

    } else if (modelName % "CacheItem" && name % "Items" && !isRaw) {
        ret = "[]any";

    } else if (name % "Topics") {
        ret = isRaw ? "[]string" : "[]base.Hash";

    } else if (name % "Emitter") {
        ret = isRaw ? "[]string" : "[]base.Address";

    } else if (modelName == "Parameter" && name % "Value") {
        ret = isRaw ? "string" : "any";

    } else if (name % "AccessList") {
        ret = isRaw ? "[]StorageSlot" : "";

    } else if (modelName % "Transaction" && name % "Receipt") {
        ret = isRaw ? "" : "*SimpleReceipt";

    } else if (modelName % "Transaction" && name % "Traces") {
        ret = isRaw ? "" : "[]SimpleTrace";

    } else if (modelName % "Transaction" && (name % "IsError" || name % "HasToken")) {
        ret = isRaw ? "" : "bool";

    } else {
        ret = (type == "CStringArray" ? "[]string" : isRaw ? "string" : type);
    }

    return ret;
}

//------------------------------------------------------------------------------------------------------------
bool skipField(const CClassDefinition& model, const CMember& field, bool raw) {
    bool noGo =
        (field.memberFlags & IS_MINIMAL) && (field.memberFlags & IS_NOWRITE) && (field.memberFlags & IS_NOADDFLD);
    if (noGo) {
        return true;
    }

    if (!raw && field.memberFlags & IS_RAWONLY) {
        return true;
    }

    if (model.base_name % "Transaction") {
        if (field.name % "Cachebits" || field.name % "ExtraData" || field.name % "ExtraValue1" ||
            field.name % "ExtraValue2" || field.name % "Finalized" || field.name % "FromName" ||
            field.name % "ToName" || field.name % "Reserved2" || field.name % "Statements" || field.name % "Unused") {
            return true;
        } else if (raw) {
            return field.name % "Date" || field.name % "Encoding" || field.name % "Ether" ||
                   field.name % "EtherGasPrice" || field.name % "CompressedTx" || field.name % "GasUsed" ||
                   field.name % "HasToken" || field.name % "IsError" || field.name % "Receipt" ||
                   field.name % "Statements" || field.name % "Timestamp" || field.name % "Traces" || field.name % "raw";
        }
        return false;
    }

    return contains(field.name, "::") || field.name == "InputsDict" || field.name == "OutputsDict" ||
           field.name == "Abi_source" || (!raw && field.name == "LogsBloom") || (raw && field.name == "IsError") ||
           (raw && startsWith(field.name, "Compressed")) || (field.name == "raw" && raw) ||
           (model.base_name == "Trace" && raw && field.name == "Timestamp") ||
           (model.base_name == "Log" && raw && field.name == "Date") ||
           (model.base_name == "Log" && raw && field.name == "Timestamp") || field.name == "Topic0" ||
           field.name == "Topic1" || field.name == "Topic2" || field.name == "Topic3" || field.name == "LogType" ||
           field.name == "Unused" || (model.base_name == "Block" && field.name == "Finalized");
}

//------------------------------------------------------------------------------------------------------------
string_q jsonName(const CClassDefinition& model, const CMember& field, bool raw) {
    if (field.name == "raw") {
        return "-";
    }

    string_q name = firstLower(field.value);
    if (!raw) {
        return name;
    }

    if (model.base_name == "Trace" && field.name == "TransactionIndex") {
        name = "transactionPosition";
    } else if (model.base_name == "Block" && field.name == "BlockNumber") {
        name = "number";
    }

    return name;
}
