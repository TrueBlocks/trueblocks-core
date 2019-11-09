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
#include "acctlib.h"
#include "options.h"

extern string_q getCaseCode    (const string_q& fieldCase, const string_q& ex);
extern string_q getCaseSetCode (const string_q& fieldCase);
extern string_q short3         (const string_q& in);
extern string_q short2         (const string_q& in);
extern string_q checkType      (const string_q& typeIn);
extern string_q convertTypes   (const string_q& inStr);
static string_q tab = string_q("\t");
//------------------------------------------------------------------------------------------------------------
void handle_generate(const COptions& options, CToml& toml, const string_q& dataFile, const string_q& ns) {

    //------------------------------------------------------------------------------------------------
    string_q className   = toml.getConfigStr ("settings", "class", "");
    string_q classBase   = toProper(extract(className, 1));
    string_q classUpper  = toUpper(classBase);
    string_q baseClass   = toml.getConfigStr ("settings", "baseClass", "CBaseNode");
    string_q scope       = toml.getConfigStr ("settings", "scope", "static");     //TODO(tjayrush): global data
    string_q hIncludes   = toml.getConfigStr ("settings", "includes", "");
    string_q sIncludes   = toml.getConfigStr ("settings", "cIncs", "");
    bool     serialize   = toml.getConfigBool("settings", "serialize", false);
    bool     useExport   = toml.getConfigBool("settings", "useExport", false);
    string_q display_str = toml.getConfigStr ("settings", "display_str", "");

    //------------------------------------------------------------------------------------------------
    string_q baseBase   = toProper(extract(baseClass, 1));
    string_q baseName   = extract(className, 1);
    string_q baseProper = toProper(baseName);
    string_q baseLower  = toLower(baseName);
    string_q baseUpper  = toUpper(baseName);

    //------------------------------------------------------------------------------------------------
    string_q fieldDec, fieldSet,    fieldCopy,   fieldClear, fieldCase;
    string_q fieldReg, fieldSubCls, fieldGetObj, fieldGetStr;
    string_q fieldArchiveWrite, fieldArchiveRead;
    string_q headerIncs, sourceIncs;

    //------------------------------------------------------------------------------------------------
    if (!hIncludes.empty()) {
        CStringArray items;
        explode(items, hIncludes, '|');
        for (auto item : items) {
            if (!item.empty() && item != "none")
                headerIncs += "#include \"" + item + "\"\n";
        }
    }

    //------------------------------------------------------------------------------------------------
    if (!sIncludes.empty()) {
        CStringArray items;
        explode(items, sIncludes, '|');
        for (auto item : items) {
            if (!item.empty() && item != "none")
                sourceIncs += "#include \"" + item + "\"\n";
        }
    }

    //------------------------------------------------------------------------------------------------
    bool isBase = (baseClass == "CBaseNode");
    string_q parSer2 = isBase ?
                        "\t[{BASE_CLASS}]::SerializeC(archive);\n" :
                        "\t[{BASE_CLASS}]::SerializeC(archive);\n\n";
    string_q parReg = isBase ?
                        "" :
                        "[{BASE_CLASS}]::registerClass();\n\n\t";
    string_q parCnk = isBase ?
                        "ret = next[{BASE_BASE}]Chunk(fieldName, this);\n" :
                        "ret = next[{BASE_BASE}]Chunk(fieldName, this);\n";
    string_q parSet = isBase ?
                        "" :
                        "\tif ([{BASE_CLASS}]::setValueByName(fieldName, fieldValue))\n\t\treturn true;\n\n";

    //------------------------------------------------------------------------------------------------
    // build the field list from the config file string
    string_q fields = substitute(toml.getConfigStr("settings", "fields", ""), "address[]", "CAddressArray");
    CParameterArray fieldList;
    while (!fields.empty()) {
        string_q fieldDef = nextTokenClear(fields, '|');
        CParameter param(fieldDef);
        fieldList.push_back(param);
    }

    //------------------------------------------------------------------------------------------------
    for (auto fld : fieldList) {

        string_q declareFmt = "\t[{TYPE}] *[{NAME}];";
        string_q copyFmt    = "\t[{NAME}] = +SHORT+.[{NAME}];\n";
        string_q regFmt     = "\tADD_FIELD(CL_NM, \"[{NAME}]\", T_TEXT, ++fieldNum);\n";
        string_q regHide    = "\tHIDE_FIELD(CL_NM, \"[{NAME}]\");\n";

        if (fld.isPointer) {
            copyFmt = "\tif ([+SHORT+.{NAME}]) {\n\t\t[{NAME}] = new [{TYPE}];\n"
            "\t\t*[{NAME}] = *[+SHORT+.{NAME}];\n\t}\n";
        } else {
            replace(declareFmt, "*", "");
        }

        string_q setFmt = "\t[{NAME}]";
        string_q regType;
               if (fld.type == "bloom")           { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_BLOOM";
        } else if (fld.type == "wei")             { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_WEI";
        } else if (fld.type == "gas")             { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_GAS";
        } else if (fld.type == "timestamp")       { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_TIMESTAMP";
        } else if (fld.type == "blknum")          { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "string")          { setFmt = "\t[{NAME}] = [{DEFS}];\n";   regType = "T_TEXT";
        } else if (fld.type == "addr")            { setFmt = "\t[{NAME}] = [{DEFS}];\n";   regType = "T_ADDRESS";
        } else if (fld.type == "address")         { setFmt = "\t[{NAME}] = [{DEFS}];\n";   regType = "T_ADDRESS";
        } else if (fld.type == "hash")            { setFmt = "\t[{NAME}] = [{DEFS}];\n";   regType = "T_HASH";
        } else if (fld.type == "int8")            { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int16")           { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int32")           { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int64")           { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int256")          { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint8")           { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint16")          { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint32")          { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint64")          { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint256")         { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "bool")            { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_BOOL";
        } else if (fld.type == "double")          { setFmt = "\t[{NAME}] = [{DEFF}];\n";   regType = "T_DOUBLE";
        } else if (startsWith(fld.type, "bytes")) { setFmt = "\t[{NAME}] = [{DEFS}];\n";   regType = "T_TEXT";
        } else if (endsWith(fld.type, "_e"))      { setFmt = "\t[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.isPointer)                 { setFmt = "\t[{NAME}] = [{DEFP}];\n";   regType = "T_POINTER";
        } else if (fld.isObject)                  { setFmt = "\t[{NAME}] = [{TYPE}]();\n"; regType = "T_OBJECT";
        } else                                    { setFmt = STR_UNKOWNTYPE;               regType = "T_TEXT"; }

        if (contains(fld.type, "Array")) {
            setFmt = "\t[{NAME}].clear();\n";
            if (contains(fld.type, "Address")) {
                regType = "T_ADDRESS|TS_ARRAY";
            } else if (contains(fld.type, "String")) {
                regType = "T_TEXT|TS_ARRAY";
            } else {
                regType = "T_OBJECT|TS_ARRAY";
            }
        }
#define getDefault(a) (fld.strDefault.empty() ? (a) : fld.strDefault )
        replace(setFmt, "[{DEFS}]", getDefault("\"\""));
        replace(setFmt, "[{DEF}]",  getDefault("0"));
        replace(setFmt, "[{DEFF}]", getDefault("0.0"));
        replace(setFmt, "[{DEFT}]", getDefault("earliestDate"));
        replace(setFmt, "[{DEFP}]", getDefault("NULL"));

        if (contains(fld.type, "Array") || (fld.isObject && !fld.isPointer)) {

            if (contains(fld.type, "CStringArray")  ||
                contains(fld.type, "CBlockNumArray")   ||
                contains(fld.type, "CAddressArray") ||
                contains(fld.type, "CBigUintArray") ||
                contains(fld.type, "CTopicArray")) {

                fieldGetStr += STR_GETSTR_CODE_FIELD;
                replaceAll(fieldGetStr, "[{FIELD}]", fld.name);
                if (fld.name == "topics") {
                    replaceAll(fieldGetStr, "THING", "topic_2_Str");
                } else if (contains(fld.type, "CBlockNumArray")) {
                    replaceAll(fieldGetStr, "THING", "uint_2_Str");
                } else if (contains(fld.type, "CBigUintArray")) {
                    replaceAll(fieldGetStr, "THING", "bnu_2_Str");
                } else {
                    replaceAll(fieldGetStr, "THING", "");
                }
            } else {
                fieldGetObj += STR_GETOBJ_CODE_FIELD;
                if (!contains(fld.type, "Array")) {
                    replace(fieldGetObj, " && index < [{FIELD}].size()", "");
                    replace(fieldGetObj, "[index]", "");
                }
                replaceAll(fieldGetObj, "[{FIELD}]", fld.name);
            }
        }

        fieldReg  += substitute(fld.Format(regFmt), "T_TEXT", regType);
        replaceAll(fieldReg, "CL_NM", "[{CLASS_NAME}]");
        if (fld.noWrite) {
            fieldReg += fld.Format(regHide);
            replaceAll(fieldReg, "CL_NM", "[{CLASS_NAME}]");
        }

        if (!fld.noWrite_min) {
            fieldCase += fld.Format("[{TYPE}]+[{NAME}]-[{ISPOINTER}]~[{ISOBJECT}]|");
            fieldDec  += (convertTypes(fld.Format(declareFmt)) + "\n");
            fieldCopy += substitute(substitute(fld.Format(copyFmt), "+SHORT+", "[{SHORT}]"), "++CLASS++", "[{CLASS_NAME}]");
            fieldSet  += fld.Format(setFmt);

            string_q clearFmt = "\tif ([{NAME}])\n\t\tdelete [{NAME}];\n\t[{NAME}] = NULL;\n";
            fieldClear += (fld.isPointer ? fld.Format(clearFmt) : "");

            if (fld.isObject && !fld.isPointer && !contains(fld.type, "Array")) {
                string_q fmt = STR_SUBCLASS;
                replaceAll(fmt, "[FNAME]", fld.name);
                replaceAll(fmt, "[SH3]", short3(baseLower));
                string_q fldStr = fld.Format(fmt);
                replace(fldStr, "++", "[{");
                replace(fldStr, "++", "}]");
                if (fieldSubCls.empty())
                    fieldSubCls = "\n\tstring_q s;\n";
                fieldSubCls += fldStr;
            }

            fieldArchiveRead  += ((fld.noWrite ? "//" : "") + fld.Format(fld.isPointer ? STR_PRTREADFMT  : STR_READFMT));
            fieldArchiveWrite += ((fld.noWrite ? "//" : "") + fld.Format(fld.isPointer ? STR_PTRWRITEFMT : STR_WRITEFMT));
        }
    }

    if (!display_str.empty())
        display_str = "\n\"[{" + substitute(substitute(trim(toUpper(display_str)), " ", ""), ",", "}]\\t\"\n\"[{") + "}]\"";
    else
        display_str = "\"\"";

    //------------------------------------------------------------------------------------------------
    bool hasObjGetter = !fieldGetObj.empty();
    if (hasObjGetter)
        fieldGetObj = substitute(string_q(STR_GETOBJ_CODE), "[{FIELDS}]", fieldGetObj);
    bool hasStrGetter = !fieldGetStr.empty();
    if (hasStrGetter)
        fieldGetStr = substitute(string_q(STR_GETSTR_CODE), "[{FIELDS}]", fieldGetStr);

    //------------------------------------------------------------------------------------------------
    string_q operatorH = string_q(serialize ? STR_OPERATOR_H : "");
    string_q operatorC = string_q(serialize ? STR_OPERATOR_C : "\n");

    //------------------------------------------------------------------------------------------------
    string_q caseSetCodeStr = getCaseSetCode(fieldCase);
    string_q subClsCodeStr  = fieldSubCls;

    //------------------------------------------------------------------------------------------------
    string_q sortStr = substitute(toml.getConfigStr("settings", "sort", ""), "&&", "&&\n\t\t\t");
    string_q eqStr   = substitute(toml.getConfigStr("settings", "equals", ""), "&&", "&&\n\t\t\t");

    //------------------------------------------------------------------------------------------------
    string_q fnBase = substitute(substitute(dataFile, ".txt", ""), "./classDefinitions/", "");
    string_q headerFile = "./" + fnBase + ".h";
    string_q headSource;
    asciiFileToString(configPath("makeClass/blank.h"), headSource);
    replaceAll(headSource, "[{GET_OBJ}]",        (hasObjGetter ? string_q(STR_GETOBJ_HEAD)+(hasStrGetter?"":"\n") : ""));
    replaceAll(headSource, "[{GET_STR}]",        (hasStrGetter ? string_q(STR_GETSTR_HEAD)+"\n" : ""));
    replaceAll(headSource, "[FIELD_COPY]",       fieldCopy);
    replaceAll(headSource, "[FIELD_DEC]",        fieldDec);
    replaceAll(headSource, "[FIELD_SET]",        fieldSet);
    replaceAll(headSource, "[FIELD_CLEAR]",      fieldClear);
    replaceAll(headSource, "[H_INCLUDES]",       headerIncs);
    replaceAll(headSource, "[{OPERATORS}]",      operatorH);
    replaceAll(headSource, "[{BASE_CLASS}]",     baseClass);
    replaceAll(headSource, "[{BASE_BASE}]",      baseBase);
    replaceAll(headSource, "[{BASE}]",           baseUpper);
    replaceAll(headSource, "[{CLASS_NAME}]",     className);
    replaceAll(headSource, "[{COMMENT_LINE}]",   STR_COMMENT_LINE);
    replaceAll(headSource, "[{LONG}]",           baseLower);
    replaceAll(headSource, "[{PROPER}]",         baseProper);
    replaceAll(headSource, "[{SHORT}]",          short2(baseLower));
    replaceAll(headSource, "[{BASE_CLASS}]",     baseClass);
    replaceAll(headSource, "[{BASE_BASE}]",      baseBase);
    replaceAll(headSource, "[{BASE}]",           baseUpper);
    replaceAll(headSource, "[{CLASS_NAME}]",     className);
    replaceAll(headSource, "[{CLASS_BASE}]",     classBase);
    replaceAll(headSource, "[{CLASS_UPPER}]",    classUpper);
    replaceAll(headSource, "[{COMMENT_LINE}]",   STR_COMMENT_LINE);
    replaceAll(headSource, "[{LONG}]",           baseLower);
    replaceAll(headSource, "[{PROPER}]",         baseProper);
    replaceAll(headSource, "[{SHORT3}]",         short3(baseLower));
    replaceAll(headSource, "[{SHORT}]",          short2(baseLower));
    replaceAll(headSource, "[{SCOPE}]",          scope);
    replaceAll(headSource, "[{SORT_COMMENT}]",   (sortStr.length() ? STR_SORT_COMMENT_1 : STR_SORT_COMMENT_2));
    replaceAll(headSource, "[{SORT_CODE}]",      (sortStr.length() ? sortStr : "true"));
    replaceAll(headSource, "[{EQUAL_COMMENT}]",  (eqStr.length() ? STR_EQUAL_COMMENT_1 : STR_EQUAL_COMMENT_2));
    replaceAll(headSource, "[{EQUAL_CODE}]",     (eqStr.length() ? eqStr : "false"));
    replaceAll(headSource, "[{NAMESPACE1}]",     (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    replaceAll(headSource, "[{NAMESPACE2}]",     (ns.empty() ? "" : "}  /""/ namespace qblocks\n"));
    replaceAll(headSource, "public:\n\npublic:", "public:");

    writeTheCode(headerFile, headSource, ns);

    //------------------------------------------------------------------------------------------------
    string_q fieldStr = fieldList.size() ? substitute(getCaseCode(fieldCase, ""), "[{PTR}]", "") : "// No fields";

    string_q srcFile    = "./" + fnBase + ".cpp";
    string_q srcSource;
    asciiFileToString(configPath("makeClass/blank.cpp"), srcSource);
    if (useExport)
        replace(srcSource, "ctx << toJson();", "doExport(ctx);");
    if ((startsWith(className, "CNew") || className == "CPriceQuote") && !contains(getCWD(), "parse"))
        replace(srcSource, "version of the data\n", STR_UPGRADE_CODE);
    replaceAll(srcSource, "[{GET_OBJ}]",         fieldGetObj);
    replaceAll(srcSource, "[{GET_STR}]",         fieldGetStr);
    replaceAll(srcSource, "[ARCHIVE_READ]",      fieldArchiveRead);
    replaceAll(srcSource, "[ARCHIVE_WRITE]",     fieldArchiveWrite);
    replaceAll(srcSource, "[{OPERATORS}]",       operatorC);
    replaceAll(srcSource, "[REGISTER_FIELDS]",   fieldReg);
    replaceAll(srcSource, "[{FIELD_CASE}]",      fieldStr);
    replaceAll(srcSource, "[OTHER_INCS]",        sourceIncs);
    replaceAll(srcSource, "[FIELD_SETCASE]",     caseSetCodeStr);
    replaceAll(srcSource, "[{SUBCLASS_FLDS}]",   subClsCodeStr);
    replaceAll(srcSource, "[{PARENT_SER2}]",     parSer2);
    replaceAll(srcSource, "[{PARENT_REG}]",      parReg);
    replaceAll(srcSource, "[{PARENT_CHNK}]\n",   parCnk);
    replaceAll(srcSource, "[{PARENT_SET}]\n",    parSet);
    replaceAll(srcSource, "[{BASE_CLASS}]",      baseClass);
    replaceAll(srcSource, "[{BASE_BASE}]",       baseBase);
    replaceAll(srcSource, "[{BASE}]",            baseUpper);
    replaceAll(srcSource, "[{CLASS_NAME}]",      className);
    replaceAll(srcSource, "[{COMMENT_LINE}]",    STR_COMMENT_LINE);
    replaceAll(srcSource, "[{LONG}]",            baseLower);
    replaceAll(srcSource, "[{PROPER}]",          baseProper);
    replaceAll(srcSource, "[{SHORT}]",           short2(baseLower));
    replaceAll(srcSource, "[{BASE_CLASS}]",      baseClass);
    replaceAll(srcSource, "[{BASE_BASE}]",       baseBase);
    replaceAll(srcSource, "[{BASE}]",            baseUpper);
    replaceAll(srcSource, "[{CLASS_NAME}]",      className);
    replaceAll(srcSource, "[{CLASS_BASE}]",      classBase);
    replaceAll(srcSource, "[{CLASS_UPPER}]",     classUpper);
    replaceAll(srcSource, "[{COMMENT_LINE}]",    STR_COMMENT_LINE);
    replaceAll(srcSource, "[{DISPLAY_FIELDS}]",  display_str);
    replaceAll(srcSource, "[{LONG}]",            baseLower);
    replaceAll(srcSource, "[{PROPER}]",          baseProper);
    replaceAll(srcSource, "[{SHORT3}]",          short3(baseLower));
    replaceAll(srcSource, "[{SHORT}]",           short2(baseLower));
    replaceAll(srcSource, "[{SCOPE}]",           scope);
    replaceAll(srcSource, "[{NAMESPACE1}]",      (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    replaceAll(srcSource, "[{NAMESPACE2}]",      (ns.empty() ? "" : "}  // namespace qblocks\n"));
    replaceAll(srcSource, "[{FN}]",              fnBase);
    writeTheCode(srcFile, srcSource, ns);
}

//------------------------------------------------------------------------------------------------
string_q getCaseCode(const string_q& fieldCase, const string_q& ex) {
    string_q baseTab = (tab+tab+ex);
    string_q caseCode;
    for (char ch = '_' ; ch < 'z' + 1 ; ch++) {
        string_q charStr;
        charStr = ch;
        if (contains(toLower(fieldCase), "+" + charStr)) {
            caseCode += baseTab + "case '" + ch + "':\n";
            string_q fields = fieldCase;
            while (!fields.empty()) {
                string_q isObj = nextTokenClear(fields, '|');
                string_q type  = nextTokenClear(isObj, '+');
                string_q field = nextTokenClear(isObj, '-');
                string_q isPtr = nextTokenClear(isObj, '~');
                bool     isPointer = str_2_Bool(isPtr);
                bool     isObject  = str_2_Bool(isObj);

                if (tolower(field[0]) == ch) {
                    caseCode += baseTab + tab + "if ( fieldName % \"" + field + "\"";
                    if (contains(type, "Array"))
                        caseCode += " || fieldName % \"" + field + "Cnt\"";
                    caseCode += " )";
                    if (contains(type, "List") || isPointer) {
                        string_q ptrCase = PTR_GET_CASE;
                        replaceAll(ptrCase, "[{NAME}]", field);
                        replaceAll(ptrCase, "[{TYPE}]", type);
                        caseCode += ptrCase;

                    } else if (type == "bool") {
                        caseCode += " return bool_2_Str_t([{PTR}]" + field + ");";

                    } else if (type == "bloom") {
                        caseCode += " return bloom_2_Bytes([{PTR}]" + field + ");";

                    } else if (type == "wei") {
                        caseCode += " return wei_2_Str([{PTR}]" + field + ");";

                    } else if (type == "gas") {
                        caseCode += " return gas_2_Str([{PTR}]" + field + ");";

                    } else if (type == "timestamp") {
                        caseCode += " return ts_2_Str([{PTR}]" + field + ");";

                    } else if (type == "addr" || type == "address") {
                        caseCode += " return addr_2_Str([{PTR}]" + field + ");";

                    } else if (type == "hash") {
                        caseCode += " return hash_2_Str([{PTR}]" + field + ");";

                    } else if (startsWith(type, "bytes")) {
                        caseCode += " return [{PTR}]" + field + ";";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32" || type == "uint64") {
                        caseCode += " return uint_2_Str([{PTR}]" + field + ");";

                    } else if (type == "blknum") {
                        caseCode += " return uint_2_Str([{PTR}]" + field + ");";

                    } else if (type == "uint256") {
                        caseCode += " return bnu_2_Str([{PTR}]" + field + ");";

                    } else if (type == "int8" || type == "int16" || type == "int32" || type == "int64") {
                        caseCode += " return int_2_Str([{PTR}]" + field + ");";

                    } else if (type == "int256") {
                        caseCode += " return bni_2_Str([{PTR}]" + field + ");";

                    } else if (type == "double") {
                        caseCode += " return double_2_Str([{PTR}]" + field + ");";

                    } else if (endsWith(type,"_e")) {
                        caseCode +=  " return uint_2_Str(" + field + ");";

                    } else if (contains(type, "CStringArray") || contains(type, "CAddressArray")) {
                        string_q str = STR_CASE_CODE_STRINGARRAY;
                        replaceAll(str, "[{FIELD}]", field);
                        caseCode += str;

                    } else if (contains(type, "CBigUintArray") || contains(type, "CTopicArray")) {
                        string_q str = STR_CASE_CODE_STRINGARRAY;
                        // hack for size clause
                        replace(str, "[{FIELD}]", field);
                        // hack for the array access
                        replace(str, "[{FIELD}][i]", "topic_2_Str("+field+"[i])");
                        caseCode += str;

                    } else if (contains(type, "Array")) {
                        string_q str = STR_CASE_CODE_ARRAY;
                        if (contains(type, "CBlockNum"))
                            replaceAll(str, "[{PTR}][{FIELD}][i].Format()", "uint_2_Str([{PTR}][{FIELD}][i])");
                        replaceAll(str, "[{FIELD}]", field);
                        caseCode += str;

                    } else if (isObject) {
                        caseCode += " { expContext().noFrst=true; return [{PTR}]" + field + ".Format(); }";

                    } else {
                        caseCode += " return [{PTR}]" + field + ";";
                    }

                    caseCode += "\n";
                }
            }
            caseCode += baseTab + tab + "break;\n";
        }
    }
    replaceAll(caseCode, "[BTAB]", baseTab);
    caseCode = "// Return field values\n\tswitch (tolower(fieldName[0])) {\n" + caseCode + "\t}\n";
    return caseCode;
}

string_q strArraySet =
" {\n"
"\t\t\t\tstring_q str = fieldValue;\n"
"\t\t\t\twhile (!str.empty()) {\n"
"\t\t\t\t\t[{NAME}].push_back(nextTokenClear(str, ','));\n"
"\t\t\t\t}\n"
"\t\t\t\treturn true;\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------
string_q getCaseSetCode(const string_q& fieldCase) {
    string_q baseTab = (tab+tab);
    string_q caseCode;
    for (char ch = '_' ; ch < 'z' + 1 ; ch++) {
        string_q charStr;
        charStr = ch;
        if (contains(toLower(fieldCase), "+" + charStr)) {
            caseCode += baseTab + "case '" + ch + "':\n";
            string_q fields = fieldCase;
            while (!fields.empty()) {
                string_q isObj = nextTokenClear(fields, '|');
                string_q type  = nextTokenClear(isObj, '+');
                string_q field = nextTokenClear(isObj, '-');
                string_q isPtr = nextTokenClear(isObj, '~');
                bool     isPointer = str_2_Bool(isPtr);
                bool     isObject  = str_2_Bool(isObj);

                if (tolower(field[0]) == ch) {
                    caseCode += baseTab + tab + "if ( fieldName % \"" + field + "\" )";
                    if (contains(type, "List") || isPointer) {
                        string_q ptrCase = PTR_SET_CASE;
                        replaceAll(ptrCase, "[{NAME}]", field);
                        replaceAll(ptrCase, "[{TYPE}]", type);
                        caseCode += ptrCase;

                    } else if (type == "bool") {
                        caseCode +=  " { " + field + " = str_2_Bool(fieldValue); return true; }";

                    } else if (type == "bloom") {
                        caseCode +=  " { " + field + " = str_2_Bloom(fieldValue); return true; }";

                    } else if (type == "wei") {
                        caseCode +=  " { " + field + " = str_2_Wei(fieldValue); return true; }";

                    } else if (type == "gas") {
                        caseCode +=  " { " + field + " = str_2_Gas(fieldValue); return true; }";

                    } else if (type == "timestamp") {
                        caseCode +=  " { " + field + " = str_2_Ts(fieldValue); return true; }";

                    } else if (type == "addr" || type == "address") {
                        caseCode += " { " + field + " = str_2_Addr(fieldValue); return true; }";

                    } else if (type == "hash") {
                        caseCode += " { " + field + " = str_2_Hash(fieldValue); return true; }";

                    } else if (startsWith(type, "bytes")) {
                        caseCode += " { " + field + " = toLower(fieldValue); return true; }";

                    } else if (type == "int8" || type == "int16" || type == "int32") {
                        caseCode +=  " { " + field + " = (int32_t)str_2_Uint(fieldValue); return true; }";

                    } else if (type == "int64") {
                        caseCode +=  " { " + field + " = str_2_Int(fieldValue); return true; }";

                    } else if (type == "int256") {
                        caseCode +=  " { " + field + " = str_2_Wei(fieldValue); return true; }";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32") {
                        caseCode +=  " { " + field + " = (uint32_t)str_2_Uint(fieldValue); return true; }";

                    } else if (type == "uint64") {
                        caseCode +=  " { " + field + " = str_2_Uint(fieldValue); return true; }";

                    } else if (type == "uint256") {
                        caseCode +=  " { " + field + " = str_2_Wei(fieldValue); return true; }";

                    } else if (type == "blknum") {
                        caseCode +=  " { " + field + " = str_2_Uint(fieldValue); return true; }";

                    } else if (type == "double") {
                        caseCode +=  " { " + field + " = str_2_Double(fieldValue); return true; }";

                    } else if (endsWith(type,"_e")) {
                        caseCode +=  " { " + field + " = str_2_Enum(" + type + ",fieldValue); return true; }";

                    } else if (contains(type, "CStringArray") || contains(type, "CBlockNumArray")) {
                        string_q str = strArraySet;
                        replaceAll(str, "[{NAME}]", field);
                        if (contains(type, "CBlockNumArray"))
                            replaceAll(str, "nextTokenClear(str, ',')", "str_2_Uint(nextTokenClear(str, ','))");
                        caseCode += str;

                    } else if (contains(type, "CAddressArray") ||
                               contains(type, "CBigUintArray") ||
                               contains(type, "CTopicArray")) {
                        string_q str = strArraySet;
                        replaceAll(str, "[{NAME}]", field);
                        replaceAll(str, "nextTokenClear(str, ',')", "str_2_[{TYPE}](nextTokenClear(str, ','))");
                        replaceAll(str, "[{TYPE}]", substitute(substitute(extract(type, 1), "Array", ""), "Address", "Addr"));
                        caseCode += str;

                    } else if (contains(type, "Array")) {
                        string_q str = STR_CASE_SET_CODE_ARRAY;
                        replaceAll(str, "[{NAME}]", field);
                        replaceAll(str, "[{TYPE}]", substitute(type, "Array", ""));
                        caseCode += str;

                    } else if (isObject) {
                        caseCode +=  " { return " + field + ".parseJson3(fieldValue); }";

                    } else {
                        caseCode +=  " { " + field + " = fieldValue; return true; }";
                    }

                    caseCode += "\n";
                }
            }
            caseCode += baseTab + tab + "break;\n";
        }
    }

    return caseCode + "\t\tdefault:\n\t\t\tbreak;\n";
}

//------------------------------------------------------------------------------------------------------------
const char* STR_CLASS_FILE =
"class:\t\t[CLASS_NAME]\n"
"fields:\t\tbool dataField1|int dataField2|string dataField3|Array dataField4\n"
"includes:\tnone\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_SET_CODE_ARRAY =
" {\n"
"\t\t\t\t[{TYPE}] item;\n"
"\t\t\t\tstring_q str = fieldValue;\n"
"\t\t\t\twhile (item.parseJson3(str)) {\n"
"\t\t\t\t\t[{NAME}].push_back(item);\n"
"\t\t\t\t\titem = [{TYPE}]();  /""/ reset\n"
"\t\t\t\t}\n"
"\t\t\t\treturn true;\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_ARRAY =
" {\n"
"[BTAB]\t\tsize_t cnt = [{PTR}][{FIELD}].size();\n"
"[BTAB]\t\tif (endsWith(toLower(fieldName), \"cnt\"))\n"
"[BTAB]\t\t\treturn uint_2_Str(cnt);\n"
"[BTAB]\t\tif (!cnt) return \"\";\n"
"[BTAB]\t\tstring_q retS;\n"
"[BTAB]\t\tfor (size_t i = 0 ; i < cnt ; i++) {\n"
"[BTAB]\t\t\tretS += [{PTR}][{FIELD}][i].Format();\n"
"[BTAB]\t\t\tretS += ((i < cnt - 1) ? \",\\n\" : \"\\n\");\n"
"[BTAB]\t\t}\n"
"[BTAB]\t\treturn retS;\n"
"[BTAB]\t}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_STRINGARRAY =
" {\n"
"[BTAB]\t\tsize_t cnt = [{PTR}][{FIELD}].size();\n"
"[BTAB]\t\tif (endsWith(toLower(fieldName), \"cnt\"))\n"
"[BTAB]\t\t\treturn uint_2_Str(cnt);\n"
"[BTAB]\t\tif (!cnt) return \"\";\n"
"[BTAB]\t\tstring_q retS;\n"
"[BTAB]\t\tfor (size_t i = 0 ; i < cnt ; i++) {\n"
"[BTAB]\t\t\tretS += (\"\\\"\" + [{PTR}][{FIELD}][i] + \"\\\"\");\n"
"[BTAB]\t\t\tretS += ((i < cnt - 1) ? \",\\n\" + indent() : \"\\n\");\n"
"[BTAB]\t\t}\n"
"[BTAB]\t\treturn retS;\n"
"[BTAB]\t}";

//------------------------------------------------------------------------------------------------------------
const char* STR_COMMENT_LINE =
"//---------------------------------------------------------------------------\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_OPERATOR_H =
"[{COMMENT_LINE}]"
"extern CArchive& operator<<(CArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]);\n"
"extern CArchive& operator>>(CArchive& archive, [{CLASS_NAME}]& [{SHORT3}]);\n"
"\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_OPERATOR_C =
"[{COMMENT_LINE}]"
"CArchive& operator<<(CArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]) {\n"
"\t[{SHORT3}].SerializeC(archive);\n"
"\treturn archive;\n"
"}\n"
"\n"
"[{COMMENT_LINE}]"
"CArchive& operator>>(CArchive& archive, [{CLASS_NAME}]& [{SHORT3}]) {\n"
"\t[{SHORT3}].Serialize(archive);\n"
"\treturn archive;\n"
"}\n"
"\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_SUBCLASS =
"\ts = toUpper(string_q(\"[FNAME]\")) + \"::\";\n"
"\tif (contains(fieldName, s)) {\n"
"\t\tstring_q f = fieldName;\n"
"\t\treplaceAll(f, s, \"\");\n"
"\t\tf = [FNAME].getValueByName(f);\n"
"\t\treturn f;\n"
"\t}\n\n";

//------------------------------------------------------------------------------------------------------------
const char *PTR_GET_CASE =
" {\n"
"\t\t\t\tif ([{NAME}])\n"
"\t\t\t\t\treturn [{NAME}]->Format();\n"
"\t\t\t\treturn \"\";\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------------------
const char* PTR_SET_CASE =
" {\n"
"\t\t\t\tclear();\n"
"\t\t\t\t[{NAME}] = new [{TYPE}];\n"
"\t\t\t\tif ([{NAME}]) {\n"
"\t\t\t\t\tstring_q str = fieldValue;\n"
"\t\t\t\t\treturn [{NAME}]->parseJson3(str);\n"
"\t\t\t\t}\n"
"\t\t\t\treturn false;\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETOBJ_HEAD =
"\tconst CBaseNode *getObjectAt(const string_q& fieldName, size_t index) const override;\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETOBJ_CODE_FIELD =
"\tif ( fieldName % \"[{FIELD}]\" && index < [{FIELD}].size() )\n"
"\t\treturn &[{FIELD}][index];\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETOBJ_CODE =
"//---------------------------------------------------------------------------\n"
"const CBaseNode *[{CLASS_NAME}]::getObjectAt(const string_q& fieldName, size_t index) const {\n"
"[{FIELDS}]\treturn NULL;\n"
"}\n\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETSTR_HEAD =
"\tconst string_q getStringAt(const string_q& fieldName, size_t i) const override;\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETSTR_CODE_FIELD =
"\tif ( fieldName % \"[{FIELD}]\" && i < [{FIELD}].size() )\n"
"\t\treturn THING([{FIELD}][i]);\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETSTR_CODE =
"//---------------------------------------------------------------------------\n"
"const string_q [{CLASS_NAME}]::getStringAt(const string_q& fieldName, size_t i) const {\n"
"[{FIELDS}]\treturn \"\";\n"
"}\n\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_UPGRADE_CODE =
"version of the data\n\t(([{CLASS_NAME}]*)this)->m_schema = getVersionNum();  /""/ NOLINT\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_SORT_COMMENT_1 =
"Default sort as defined in class definition";

//------------------------------------------------------------------------------------------------------------
const char* STR_SORT_COMMENT_2 =
"No default sort defined in class definition, assume already sorted, preserve ordering";

//------------------------------------------------------------------------------------------------------------
const char* STR_EQUAL_COMMENT_1 =
"Equality operator as defined in class definition";

//------------------------------------------------------------------------------------------------------------
const char* STR_EQUAL_COMMENT_2 =
"No default equal operator in class definition, assume none are equal (so find fails)";

//------------------------------------------------------------------------------------------------------------
const char* STR_PRTREADFMT =
"    [{NAME}] = NULL;\n"
"    bool has_[{NAME}] = false;\n"
"    archive >> has_[{NAME}];\n"
"    if (has_[{NAME}]) {\n"
"        string_q className;\n"
"        archive >> className;\n"
"        [{NAME}] = reinterpret_cast<[{TYPE}] *>(createObjectOfType(className));\n"
"        if (![{NAME}])\n"
"            return false;\n"
"        [{NAME}]->Serialize(archive);\n"
"    }\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_READFMT = "\tarchive >> [{NAME}];\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_PTRWRITEFMT =
"    archive << ([{NAME}] != NULL);\n"
"    if ([{NAME}]) {\n"
"        archive << [{NAME}]->getRuntimeClass()->getClassNamePtr();\n"
"        [{NAME}]->SerializeC(archive);\n"
"    }\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_WRITEFMT = "\tarchive << [{NAME}];\n";

const char* STR_UNKOWNTYPE =
"//\t[{NAME}] = ??; /""* unknown type: [{TYPE}] */\n";

//------------------------------------------------------------------------------------------------------------
string_q short2(const string_q& str) {
    string_q ret = extract(str, 0, 2);
    if (ret == "or")
        ret = "ord";
    return ret;
}

//------------------------------------------------------------------------------------------------------------
string_q short3(const string_q& str) {
    string_q ret = extract(str, 0, 3);
    if (ret == "new")
        ret = "newp";
    if (ret == "ret")
        ret = "retp";
    return ret;
}

//---------------------------------------------------------------------------
string_q checkType(const string_q& typeIn) {
    if (startsWith(typeIn, "C")) return typeIn;
    if (endsWith(typeIn, "Array")) return typeIn;

    string_q keywords[] = {
        "address", "bloom",  "bool",
        "bytes",   "bytes4", "bytes8",    "bytes16",   "bytes32",
        "double",  "gas",    "hash",      "int256",    "int32",
        "int64",   "string", "timestamp", "uint256",
        "uint32",  "uint64", "uint8",     "wei",       "blknum",
    };
    size_t cnt = sizeof(keywords) / sizeof(string_q);
    for (size_t i = 0 ; i < cnt ; i++) {
        if (keywords[i] == typeIn)
            return typeIn;
    }
    cerr << "Invalid type: " << typeIn << ". Quitting...(hit enter)" << endl;
    return "";
}

//------------------------------------------------------------------------------------------------------------
string_q convertTypes(const string_q& inStr) {

    // Note: Watch out for trailing spaces. They are here to make sure it
    // matches only the types and not the field names.
    string_q outStr = inStr;
    replaceAll(outStr, "address ",   "address_t "  );

    replaceAll(outStr, "bytes ",     "string_q "   );
    replaceAll(outStr, "bytes4 ",    "string_q "   );
    replaceAll(outStr, "bytes8 ",    "string_q "   );
    replaceAll(outStr, "bytes16 ",   "string_q "   );
    replaceAll(outStr, "bytes32 ",   "string_q "   );
    replaceAll(outStr, "string ",    "string_q "   );
    replaceAll(outStr, "blknum ",    "blknum_t "   );
    replaceAll(outStr, "timestamp ", "timestamp_t ");
    replaceAll(outStr, "bool ",      "bool "       );

    replaceAll(outStr, "hash ",      "hash_t "     );
    replaceAll(outStr, "bloom ",     "bloom_t "    );
    replaceAll(outStr, "gas ",       "gas_t "      );
    replaceAll(outStr, "wei ",       "wei_t "      );

    replaceAll(outStr, "int8 ",      "int32_t "    );
    replaceAll(outStr, "int16 ",     "int32_t "    );
    replaceAll(outStr, "int32 ",     "int32_t "    );
    replaceAll(outStr, "int64 ",     "int64_t "    );
    replaceAll(outStr, "int256 ",    "bigint_t "   );
    replaceAll(outStr, "ubigint_t",  "biguint_t"   );

    return outStr;
}
