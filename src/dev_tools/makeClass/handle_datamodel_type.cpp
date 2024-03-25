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

extern string_q type_2_GoType(const CMember& field);
extern string_q specialCase(const CClassDefinition& model, const CMember& field, const string_q& type, bool isRaw);
extern string_q jsonName(const CClassDefinition& model, const CMember& field, bool raw);
extern string_q get_cache_code(const CClassDefinition& modelIn);
extern string_q get_id_code(const string_q& cacheBy);
extern string_q get_marshal_fields(const CClassDefinition& modelIn);
extern string_q get_unmarshal_fields(const CClassDefinition& modelIn);
extern string_q type_2_TypeName(const string_q& type, bool raw);
extern bool skipField(const CClassDefinition& model, const CMember& field, bool raw);

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

    bool hasTimestamp = false;
    size_t maxRawNameWid = 0, maxSimpNameWid = 0, maxRawTypeWid = 0, maxSimpTypeWid = 0, maxModelWid = 0;
    for (size_t i = 0; i < model.fieldArray.size(); i++) {
        CMember* field = &model.fieldArray[i];
        if (field->name == "timestamp") {
            hasTimestamp = true;
        }

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
        if (skipField(model, field, true)) {
            continue;
        }

        string_q spec = specialCase(model, field, type_2_GoType(field), true);
        if (spec.empty()) {
            continue;
        }

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
        if (skipField(model, field, false)) {
            continue;
        }

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
    replaceAll(contents, "[{CACHE_CODE}]", get_cache_code(modelIn));
    if (hasTimestamp) {
        const char* STR_DATE_CODE =
            "\n"
            "func (s *Simple[{CLASS_NAME}]) Date() string {\n"
            "\treturn utils.FormattedDate(s.Timestamp)\n"
            "}\n"
            "\n";
        string_q dateCode = STR_DATE_CODE;
        replaceAll(dateCode, "[{CLASS_NAME}]", type_2_ModelName(model.go_model, false));
        replaceAll(dateCode, "Simple", isInternal ? "simple" : "Simple");
        replaceAll(contents, "[{DATE_CODE}]", dateCode);
    } else {
        replaceAll(contents, "[{DATE_CODE}]", "");
    }
    // hackathon!
    replaceAll(contents, "type SimpleBlock[Tx] struct {", "type SimpleBlock[Tx string | SimpleTransaction] struct {");

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
        return "base.MyWei";
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

    } else if (name % "Withdrawals") {
        ret = isRaw ? "[]RawWithdrawal" : "[]SimpleWithdrawal";

    } else if (name % "Action") {
        ret = isRaw ? "RawTraceAction" : "*SimpleTraceAction";

    } else if ((modelName % "State" || modelName % "Result") && name % "Values") {
        ret = isRaw ? "[]string" : "map[string]string";

    } else if (name % "Components" || name % "Inputs" || name % "Outputs") {
        ret = isRaw ? "string" : "[]SimpleParameter";

    } else if (startsWith(modelName, "Trace") && name % "Result") {
        ret = isRaw ? "*RawTraceResult" : "*SimpleTraceResult";

    } else if (startsWith(name, "Articulated") || name % "State") {
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
    if (field.isCalculated()) {
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
           field.name == "Topic1" || field.name == "Topic2" || field.name == "Topic3" || field.name == "Unused" ||
           (model.base_name == "Block" && field.name == "Finalized");
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

//------------------------------------------------------------------------------------------------------------
string_q get_cache_code(const CClassDefinition& modelIn) {
    if (modelIn.cache_type.empty()) {
        return "";
    }

    bool isCacheable = modelIn.cache_type == "cacheable";
    bool isGroupable = modelIn.cache_as == "group";
    bool isMarshalable = modelIn.cache_type == "cacheable" || modelIn.cache_type == "marshal_only";
    bool hasAddress = contains(modelIn.cache_by, "addr");

    ostringstream os;
    os << endl;
    os << "// --> " << modelIn.cache_type << (isCacheable ? " by " + modelIn.cache_by : "")
       << (isGroupable ? " as " + modelIn.cache_as : "");
    os << endl;

    string_q modelName = type_2_ModelName(modelIn.go_model, false);
    string_q groupName = type_2_ModelName(modelIn.go_model, false) + "Group";

    if (isGroupable) {
        const char* STR_GROUP_STRUCT =
            "type Simple[{GROUP_NAME}] struct {\n"
            "\tBlockNumber      base.Blknum\n"
            "\tTransactionIndex base.Txnum\n"
            "[{ADDRESS}]"
            "\t[{Proper}][]Simple[{CLASS_NAME}]\n"
            "}\n";

        string_q structCode = STR_GROUP_STRUCT;
        replaceAll(structCode, "[{GROUP_NAME}]", groupName);
        replaceAll(structCode, "[{CLASS_NAME}]", modelName);
        replaceAll(structCode, "[{ADDRESS}]", hasAddress ? "\tAddress          base.Address\n" : "");
        replaceAll(structCode, "[{Proper}]", padRight(modelIn.base_proper + "s", 17));
        os << structCode << endl;
    }

    if (isCacheable) {
        const char* STR_CACHE_NAME =
            "func (s *Simple[{CLASS_NAME}]) CacheName() string {\n"
            "\treturn \"[{Proper}]\"\n"
            "}\n";

        string_q nameCode = STR_CACHE_NAME;
        if (isGroupable) {
            replaceAll(nameCode, "[{CLASS_NAME}]", groupName);
        } else {
            replaceAll(nameCode, "[{CLASS_NAME}]", modelName);
        }
        string_q x = modelName;
        replaceAll(nameCode, "[{Proper}]", nextTokenClear(x, '['));
        os << nameCode << endl;

        const char* STR_CACHE_ID =
            "func (s *Simple[{CLASS_NAME}]) CacheId() string {\n"
            "\treturn [{CACHE_ID}]\n"
            "}\n";

        string_q idCode = STR_CACHE_ID;
        if (isGroupable) {
            replaceAll(idCode, "[{CLASS_NAME}]", groupName);
        } else {
            replaceAll(idCode, "[{CLASS_NAME}]", modelName);
        }
        replaceAll(idCode, "[{Proper}]", modelIn.base_proper);
        replaceAll(idCode, "[{CACHE_ID}]", get_id_code(modelIn.cache_by));
        os << idCode << endl;

        const char* STR_CACHE_LOCATION =
            "func (s *Simple[{CLASS_NAME}]) CacheLocation() (directory string, extension string) {\n"
            "\tpaddedId := s.CacheId()\n"
            "\tparts := make([]string, 3)\n"
            "\tparts[0] = paddedId[:2]\n"
            "\tparts[1] = paddedId[2:4]\n"
            "\tparts[2] = paddedId[4:6]\n"
            "\n"
            "\tsubFolder := strings.ToLower(s.CacheName()) + \"s\"\n"
            "\tdirectory = filepath.Join(subFolder, filepath.Join(parts...))\n"
            "\textension = \"bin\"\n"
            "\n"
            "\treturn\n"
            "}\n";
        string_q locCode = STR_CACHE_LOCATION;
        if (isGroupable) {
            replaceAll(locCode, "[{CLASS_NAME}]", groupName);
        } else {
            replaceAll(locCode, "[{CLASS_NAME}]", modelName);
        }
        os << locCode << endl;
    }

    if (isMarshalable) {
        if (isGroupable) {
            const char* STR_MARSHAL_GROUP =
                "func (s *Simple[{CLASS_NAME}]) MarshalCache(writer io.Writer) (err error) {\n"
                "\treturn cache.WriteValue(writer, s.[{Proper}]s)\n"
                "}\n";

            string_q marshalGroupCode = STR_MARSHAL_GROUP;
            replaceAll(marshalGroupCode, "[{CLASS_NAME}]", groupName);
            replaceAll(marshalGroupCode, "[{Proper}]", modelIn.base_proper);
            os << marshalGroupCode << endl;

            const char* STR_UNMARSHAL_GROUP =
                "func (s *Simple[{CLASS_NAME}]) UnmarshalCache(version uint64, reader io.Reader) (err error) {\n"
                "\treturn cache.ReadValue(reader, &s.[{Proper}]s, version)\n"
                "}\n";

            string_q unmarshalGroupCode = STR_UNMARSHAL_GROUP;
            replaceAll(unmarshalGroupCode, "[{CLASS_NAME}]", groupName);
            replaceAll(unmarshalGroupCode, "[{Proper}]", modelIn.base_proper);
            os << unmarshalGroupCode << endl;
        }

        const char* STR_CACHE_MARSHAL =
            "func (s *Simple[{MODEL_NAME}]) MarshalCache(writer io.Writer) (err error) {\n"
            "[{FIELDS}]"
            "\treturn nil\n"
            "}\n";

        string_q marshalCode = STR_CACHE_MARSHAL;

        if (isGroupable) {
            replaceAll(marshalCode, "[{CLASS_NAME}]", groupName);
        } else {
            replaceAll(marshalCode, "[{CLASS_NAME}]", modelName);
        }
        replaceAll(marshalCode, "[{MODEL_NAME}]", modelName);
        replaceAll(marshalCode, "[{FIELDS}]", get_marshal_fields(modelIn));
        os << marshalCode << endl;

        const char* STR_CACHE_UNMARSHAL =
            "func (s *Simple[{MODEL_NAME}]) UnmarshalCache(version uint64, reader io.Reader) (err error) {\n"
            "[{FIELDS}]"
            "\ts.FinishUnmarshal()\n"
            "\n"
            "\treturn nil\n"
            "}\n"
            "\n"
            "func (s *Simple[{MODEL_NAME}]) FinishUnmarshal() {\n"
            "\t// EXISTING_CODE\n"
            "\t// EXISTING_CODE\n"
            "}\n"
            "\n";

        string_q unmarshalCode = STR_CACHE_UNMARSHAL;
        if (isGroupable) {
            replaceAll(unmarshalCode, "[{CLASS_NAME}]", groupName);
        } else {
            replaceAll(unmarshalCode, "[{CLASS_NAME}]", modelName);
        }
        replaceAll(unmarshalCode, "[{MODEL_NAME}]", modelName);
        replaceAll(unmarshalCode, "[{FIELDS}]", get_unmarshal_fields(modelIn));
        replaceAll(unmarshalCode, "SimpleTopic", "base.Hash");
        replaceAll(unmarshalCode, "SimpleString", "uint64");
        replaceAll(unmarshalCode, "]SimpleTransaction", "]string");
        replaceAll(unmarshalCode, "SimpleHash", "base.Hash");
        replaceAll(unmarshalCode, "func (s *SimpleBlock[Tx]) UnmarshalCache",
                   "func (s *SimpleBlock[string]) UnmarshalCache");
        os << unmarshalCode << endl;
    }

    return os.str();
}

string_q get_id_code(const string_q& cacheBy) {
    if (cacheBy == "block") {
        return "fmt.Sprintf(\"%09d\", s.BlockNumber)";
    } else if (cacheBy == "tx") {
        return "fmt.Sprintf(\"%09d-%05d\", s.BlockNumber, s.TransactionIndex)";
    } else if (cacheBy == "address,tx") {
        return "fmt.Sprintf(\"%s-%09d-%05d\", s.Address.Hex()[2:], s.BlockNumber, s.TransactionIndex)";
    } else if (cacheBy == "address,block") {
        return "fmt.Sprintf(\"%s-%09d\", s.Address.Hex()[2:], s.BlockNumber)";
    } else if (cacheBy == "address,block,fourbyte") {
        const char* STR_THE_CODE =
            "fmt.Sprintf(\"%s-%s-%09d\", s.Address.Hex()[2:], s.Encoding[2:], s.BlockNumber)\n"
            "\t// TODO: The above creates a very large number of files for a large contract.";
        return STR_THE_CODE;
    } else {
        cerr << bRed << "Unknown cache_by value: " << cacheBy << ". Quiting..." << cOff << endl;
        exit(0);
    }
}

bool isArray(const CMember& field) {
    return (field.memberFlags & IS_ARRAY);
}

bool isObject(const CMember& field) {
    if (containsI(field.type, "topic") || containsI(field.name, "traceaddress") || containsI(field.name, "uncles")) {
        return false;
    }
    return (field.memberFlags & IS_OBJECT);
}

bool isObjectArray(const CMember& field) {
    return isArray(field) && isObject(field);
}

string_q specialCacheCase2(const string_q& modelName, const CMember& field) {
    if (field.name == "value" && modelName == "Parameter") {
        return "\t// Value\n"
               "\tvar value string\n"
               "\tif err = cache.ReadValue(reader, &value, version); err != nil {\n"
               "\t\treturn err\n"
               "\t}\n"
               "\tif err = json.Unmarshal([]byte(value), &s.Value); err != nil {\n"
               "\t\treturn fmt.Errorf(\"cannot unmarshal Value: %w\", err)\n"
               "\t}\n";
    }
    return "";
}

string_q specialCacheCase1(const string_q& modelName, const CMember& field) {
    if (field.name == "value" && modelName == "Parameter") {
        return "\t// Value\n"
               "\tvalue, err := json.Marshal(s.Value)\n"
               "\tif err != nil {\n"
               "\t\treturn fmt.Errorf(\"cannot marshal Value: %w\", err)\n"
               "\t}\n"
               "\tif err = cache.WriteValue(writer, value); err != nil {\n"
               "\t\treturn err\n"
               "\t}\n";
    }
    return "";
}

string_q get_marshal_fields(const CClassDefinition& modelIn) {
    CClassDefinition model = modelIn;
    // CClassDefinition modelOrig = modelIn;
    sort(model.fieldArray.begin(), model.fieldArray.end());

    ostringstream os;
    for (auto field : model.fieldArray) {
        string_q modelName = type_2_ModelName(modelIn.go_model, false);

        if (field.name == "raw" || skipField(model, field, false))
            continue;

        if (modelName == "Transaction" && (field.name == "traces" || field.name == "compressedTx")) {
            continue;
        }

        string_q spec = specialCacheCase1(modelName, field);
        if (spec != "") {
            os << spec << endl;
            continue;
        }

        string_q type = type_2_TypeName(field.type, false);
        string_q name = "s." + firstUpper(field.name);
        if (name == "s.LogsBloom") {
            continue;
        }
        string_q goType = type_2_GoType(field);
        if (goType == "base.MyWei" || goType == "base.Wei" || goType == "base.Hash") {
            if (name != "s.CumulativeGasUsed") {
                name = "&" + name;
            }
        }
        if (name == "s.Type") {
            name = "s." + modelIn.base_name + "Type";
        }

        os << "\t"
           << "// " << substitute(substitute(name, "&", ""), "s.", "") << endl;
        if (field.name % "transactions") {
            const char* STR_HACK =
                "\tvar txHashes []string\n"
                "\tswitch v := any(s.Transactions).(type) {\n"
                "\tcase []string:\n"
                "\t\ttxHashes = v\n"
                "\tcase []SimpleTransaction:\n"
                "\t\ttxHashes = make([]string, 0, len(s.Transactions))\n"
                "\t\tfor _, tx := range v {\n"
                "\t\t\ttxHashes = append(txHashes, tx.Hash.Hex())\n"
                "\t\t}\n"
                "\t}\n"
                "\tif err = cache.WriteValue(writer, txHashes); err != nil {\n"
                "\t\treturn err\n"
                "\t}\n";
            os << STR_HACK << endl;

        } else if (isObjectArray(field)) {
            const char* STR_ARRAY_CODE =
                "\t[{LOWER}] := make([]cache.Marshaler, 0, len([{FIELD_NAME}]))\n"
                "\tfor _, [{SINGULAR}] := range [{FIELD_NAME}] {\n"
                "\t\t[{LOWER}] = append([{LOWER}], &[{SINGULAR}])\n"
                "\t}\n"
                "\tif err = cache.WriteValue(writer, [{LOWER}]); err != nil {\n"
                "\t\treturn err\n"
                "\t}\n";
            string_q arrayCode = STR_ARRAY_CODE;
            replaceAll(arrayCode, "[{FIELD_NAME}]", name);
            replaceAll(arrayCode, "[{LOWER}]", substitute(substitute(toLower(name), "&", ""), "s.", ""));
            replaceAll(arrayCode, "[{SINGULAR}]",
                       substitute(substitute(toLower(name).substr(0, name.length() - 1), "&", ""), "s.", ""));
            os << arrayCode << endl;

        } else if (isObject(field)) {
            const char* STR_OPT_CODE =
                "\topt[{FIELD_NAME}] := &cache.Optional[Simple[{FIELD_TYPE}]]{\n"
                "\t\tValue: s.[{FIELD_NAME}],\n"
                "\t}\n"
                "\tif err = cache.WriteValue(writer, opt[{FIELD_NAME}]); err != nil {\n"
                "\t\treturn err\n"
                "\t}\n";
            string_q optCode = STR_OPT_CODE;
            replaceAll(optCode, "[{FIELD_NAME}]", substitute(name, "s.", ""));
            replaceAll(optCode, "[{FIELD_TYPE}]", type);
            os << optCode << endl;

        } else {
            os << "\t"
               << "if err = cache.WriteValue(writer, " << name << "); err != nil {" << endl;
            os << "\t"
               << "\treturn err" << endl;
            os << "\t"
               << "}" << endl;
        }
        os << endl;
    }
    return os.str();
}

string_q get_unmarshal_fields(const CClassDefinition& modelIn) {
    CClassDefinition model = modelIn;
    sort(model.fieldArray.begin(), model.fieldArray.end());

    ostringstream os;
    for (auto field : model.fieldArray) {
        if (field.name == "raw" || skipField(model, field, false))
            continue;

        string_q modelName = type_2_ModelName(modelIn.go_model, false);

        // cerr << modelName << " " << field.name << endl;
        if (modelName == "Transaction" && (field.name == "traces" || field.name == "compressedTx")) {
            continue;
        }

        string_q spec = specialCacheCase2(modelName, field);
        if (spec != "") {
            os << spec << endl;
            continue;
        }

        string_q type = type_2_TypeName(field.type, false);
        string_q name = "s." + firstUpper(field.name);
        if (name == "s.LogsBloom") {
            continue;
        }
        if (name == "s.Type") {
            name = "s." + modelIn.base_name + "Type";
        }

        os << "\t"
           << "// " << substitute(substitute(name, "&", ""), "s.", "") << endl;

        if (isArray(field) && !contains(field.type, "uint64")) {
            os << "\t" << name << " = make([]Simple" << type << ", 0)" << endl;
            os << "\tif err = cache.ReadValue(reader, &" << name << ", version); err != nil {" << endl;
            os << "\t"
               << "\treturn err" << endl;
            os << "\t"
               << "}" << endl;
            os << endl;
        } else if (isArray(field) && contains(field.type, "uint64")) {
            os << "\t" << name << " = make([]uint64, 0)" << endl;
            os << "\tif err = cache.ReadValue(reader, &" << name << ", version); err != nil {" << endl;
            os << "\t"
               << "\treturn err" << endl;
            os << "\t"
               << "}" << endl;
            os << endl;
        } else if (isObject(field)) {
            const char* STR_OPT_CODE =
                "\topt[{FIELD_NAME}] := &cache.Optional[Simple[{FIELD_TYPE}]]{\n"
                "\t\tValue: s.[{FIELD_NAME}],\n"
                "\t}\n"
                "\tif err = cache.ReadValue(reader, opt[{FIELD_NAME}], version); err != nil {\n"
                "\t\treturn err\n"
                "\t}\n"
                "\ts.[{FIELD_NAME}] = opt[{FIELD_NAME}].Get()\n";
            string_q optCode = STR_OPT_CODE;
            replaceAll(optCode, "[{FIELD_NAME}]", substitute(name, "s.", ""));
            replaceAll(optCode, "[{FIELD_TYPE}]", type);
            os << optCode << endl;
        } else {
            os << "\t"
               << "if err = cache.ReadValue(reader, &" << name << ", version); err != nil {" << endl;
            os << "\t"
               << "\treturn err" << endl;
            os << "\t"
               << "}" << endl;
            os << endl;
        }
    }
    return os.str();
}
