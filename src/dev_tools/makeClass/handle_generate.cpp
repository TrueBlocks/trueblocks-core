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
extern const char* STR_CASE_CODE_STRINGARRAY;
extern const char* STR_COMMENT_LINE;
extern const char* STR_OPERATOR_DECL;
extern const char* STR_OPERATOR_IMPL;
extern const char* STR_PARENT_GETBYVALUE;
extern const char* STR_PARENT_REGISTER;
extern const char* STR_PARENT_SET;
extern const char* STR_PARENT_SERIALIZE;
extern const char* STR_GETVALUE1;
extern const char* STR_GETVALUE2;
extern const char* STR_GETOBJ_CODE;
extern const char* STR_GETOBJ_CODE_FIELD;
extern const char* STR_GETOBJ_CODE_FIELD_OBJ;
extern const char* STR_GETSTR_CODE;
extern const char* STR_GETSTR_CODE_FIELD;
extern const char* STR_GETOBJ_HEAD;
extern const char* STR_GETSTR_HEAD;
extern const char* STR_UPGRADE_CODE;
extern const char* STR_SORT_COMMENT_1;
extern const char* STR_SORT_COMMENT_2;
extern const char* STR_EQUAL_COMMENT_1;
extern const char* STR_EQUAL_COMMENT_2;
extern const char* STR_PRTREADFMT;
extern const char* STR_READFMT;
extern const char* STR_PTRWRITEFMT;
extern const char* STR_WRITEFMT;
extern const char* STR_UNKOWNTYPE;
extern const char* STR_CHILD_OBJS;
extern const char* STR_DELETE_CMDS;
extern const char* STR_DEFAULT_TAGS;
//------------------------------------------------------------------------------------------------------------
bool COptions::handle_generate(CToml& toml, const CClassDefinition& classDefIn, bool asJs) {
    CClassDefinition classDef(toml);
    classDef.short_fn = classDefIn.short_fn;
    classDef.input_path = classDefIn.input_path;

    //------------------------------------------------------------------------------------------------
    if (toml.getConfigBool("settings", "disabled", false)) {
        if (verbose)
            cerr << "    disabled class not processed " << classDefIn.short_fn << "\n";
        return true;
    }

    //------------------------------------------------------------------------------------------------
    counter.nVisited++;

    //------------------------------------------------------------------------------------------------
    ostringstream lheaderStream, clearStream, copyStream;
    ostringstream ar_readStream, ar_writeStream;
    ostringstream src_incStream, head_incStream;
    ostringstream child_objStream, add_fieldStream, hide_fieldStream;
    ostringstream defaultsStream, openapi_componentStream;

    //------------------------------------------------------------------------------------------------
    string_q fieldGetObj, fieldGetStr;

    //------------------------------------------------------------------------------------------------
    CStringArray h_incs;
    explode(h_incs, classDef.head_includes, '|');
    for (auto inc : h_incs)
        head_incStream << ("#include \"" + inc + "\"\n");

    //------------------------------------------------------------------------------------------------
    CStringArray c_inc;
    explode(c_inc, classDef.src_includes, '|');
    for (auto inc : c_inc) {
        bool allCaps = inc == toUpper(inc);
        if (allCaps)
            src_incStream << ("#define " + inc + "\n");
        else
            src_incStream << ("#include \"" + inc + "\"\n");
    }

    //------------------------------------------------------------------------------------------------
    for (auto fld : classDef.fieldArray) {
        // keep these in this scope since they may change per field
        string_q declareFmt = "`[{TYPE}]* [{NAME}];";
        string_q regAddFmt = "`ADD_FIELD(CL_NM, \"[{NAME}]\", T_TEXT, ++fieldNum);\n";
        string_q regHideFmt = "`HIDE_FIELD(CL_NM, \"[{NAME}]\");\n";
        string_q ptrClearFmt = "`if ([{NAME}])\n``delete [{NAME}];\n`[{NAME}] = NULL;\n";
        string_q copyFmt = "`[{NAME}] = ++SHORT++.[{NAME}];\n";

        // order matters in the next block
        string_q setFmt, regType;
        // clang-format off
               if (fld.type == "Value")           { setFmt = "`[{NAME}] = [{DEF}]\n";     regType = "T_JSONVAL | TS_OMITEMPTY";
        } else if (fld.type == "wei")             { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_WEI";
        } else if (fld.type == "gas")             { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_GAS";
        } else if (fld.type == "timestamp")       { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_TIMESTAMP";
        } else if (fld.type == "datetime")        { setFmt = "`[{NAME}] = [{DEFT}];\n";   regType = "T_DATE";
        } else if (fld.type == "blknum")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_BLOCKNUM";
        } else if (fld.type == "string")          { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_TEXT | TS_OMITEMPTY";
        } else if (fld.type == "addr")            { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_ADDRESS | TS_OMITEMPTY";
        } else if (fld.type == "address")         { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_ADDRESS | TS_OMITEMPTY";
        } else if (fld.type == "hash")            { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_HASH | TS_OMITEMPTY";
        } else if (fld.type == "ipfshash")        { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_IPFSHASH | TS_OMITEMPTY";
        } else if (fld.type == "int8")            { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int16")           { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int32")           { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int64")           { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int256")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_INT256";
        } else if (fld.type == "uint8")           { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_UNUMBER";
        } else if (fld.type == "uint16")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_UNUMBER";
        } else if (fld.type == "uint32")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_UNUMBER";
        } else if (fld.type == "uint64")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_UNUMBER";
        } else if (fld.type == "uint256")         { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_UINT256";
        } else if (fld.type == "bool")            { setFmt = "`[{NAME}] = [{DEFB}];\n";   regType = "T_BOOL | TS_OMITEMPTY";
        } else if (fld.type == "double")          { setFmt = "`[{NAME}] = [{DEFF}];\n";   regType = "T_DOUBLE";
        } else if (startsWith(fld.type, "bytes")) { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_TEXT | TS_OMITEMPTY";
        } else if (endsWith(fld.type, "_e"))      { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if ((fld.is_flags & IS_POINTER))   { setFmt = "`[{NAME}] = [{DEFP}];\n";   regType = "T_POINTER | TS_OMITEMPTY";
        } else if ((fld.is_flags & IS_OBJECT))    { setFmt = "`[{NAME}] = [{TYPE}]();\n"; regType = "T_OBJECT | TS_OMITEMPTY";
        } else                                    { setFmt = STR_UNKOWNTYPE;              regType = "T_TEXT | TS_OMITEMPTY"; }
        // clang-format on

        if (fld.is_flags & IS_OMITEMPTY && !contains(regType, " | TS_OMITEMPTY"))
            regType += " | TS_OMITEMPTY";

        if ((fld.type == "Value")) {
            setFmt = "\t[{NAME}].clear();\n";
        }

        if ((fld.is_flags & IS_ARRAY)) {
            setFmt = "\t[{NAME}].clear();\n";
            if (contains(fld.type, "Address")) {
                regType = "T_ADDRESS | TS_ARRAY | TS_OMITEMPTY";
            } else if (contains(fld.type, "String")) {
                regType = "T_TEXT | TS_ARRAY | TS_OMITEMPTY";
            } else {
                regType = "T_OBJECT | TS_ARRAY | TS_OMITEMPTY";
            }
        }

        if (fld.is_flags & IS_BUILTIN) {
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

        } else if ((fld.is_flags & IS_OBJECT)) {
            string_q str = STR_GETOBJ_CODE_FIELD;
            if (!(fld.is_flags & IS_ARRAY))
                str = STR_GETOBJ_CODE_FIELD_OBJ;
            replace(str, "[PTR]", ((fld.is_flags & IS_POINTER) ? "" : "&"));
            replaceAll(str, "[{TYPE}]",
                       substitute(substitute(substitute(fld.type, "Array2", ""), "Array", ""), "Ptr", "*"));
            replaceAll(str, "[{FIELD}]", fld.name);
            fieldGetObj += str;
        }

        replace(setFmt, "[{DEFB}]", fld.strDefault.empty() ? "false" : fld.strDefault);
        replace(setFmt, "[{DEFS}]", fld.strDefault.empty() ? "\"\"" : fld.strDefault);
        replace(setFmt, "[{DEF}]", fld.strDefault.empty() ? "0" : fld.strDefault);
        replace(setFmt, "[{DEFF}]", fld.strDefault.empty() ? "0.0" : fld.strDefault);
        replace(setFmt, "[{DEFT}]", fld.strDefault.empty() ? "earliestDate" : fld.strDefault);
        replace(setFmt, "[{DEFP}]", fld.strDefault.empty() ? "NULL" : fld.strDefault);

        if ((fld.is_flags & IS_POINTER) && !(fld.is_flags & IS_ARRAY))
            copyFmt =
                "`if ([++SHORT++.{NAME}]) {\n``[{NAME}] = new [{TYPE}];\n``*[{NAME}] = *[++SHORT++.{NAME}];\n`}\n";

        if (!(fld.is_flags & IS_POINTER) || (fld.is_flags & IS_ARRAY))
            replace(declareFmt, "*", "");

        if (!(fld.is_flags & IS_NOADDFLD)) {
            string_q fieldAddLine = fld.Format(regAddFmt);
            replaceAll(fieldAddLine, "T_TEXT", regType);
            replaceAll(fieldAddLine, "CL_NM", "[{CLASS_NAME}]");
            if ((fld.is_flags & IS_OBJECT) && !(fld.is_flags & IS_ARRAY) && !(fld.is_flags & IS_POINTER)) {
                replaceAll(fieldAddLine, "ADD_FIELD", "ADD_OBJECT");
                replaceAll(fieldAddLine, "++fieldNum);\n", "++fieldNum, GETRUNTIME_CLASS(" + fld.type + "));\n");
            }
            add_fieldStream << fieldAddLine;
            if (fld.is_flags & IS_NOWRITE)
                add_fieldStream << substitute(fld.Format(regHideFmt), "CL_NM", "[{CLASS_NAME}]");
        }

        // minimal means that a field is part of the object's data (such as CReceipt::blockNumber) but should not be
        // part of the 'class' proper -- i.e. it gets its value from its containing class (in this case the block it
        // belongs to).
        if (!(fld.is_flags & IS_MINIMAL)) {
            lheaderStream << convertTypes(fld.Format(declareFmt)) << endl;
            copyStream << substitute(substitute(fld.Format(copyFmt), "++SHORT++", "[{SHORT}]"), "++CLASS++",
                                     "[{CLASS_NAME}]");
            defaultsStream << fld.Format(setFmt);
            clearStream << (((fld.is_flags & IS_POINTER) && !(fld.is_flags & IS_ARRAY)) ? fld.Format(ptrClearFmt) : "");
            if (fld.is_flags & IS_NOWRITE) {
                ar_readStream << substitute(fld.Format(STR_READFMT), "`archive", "`// archive");
                ar_writeStream << substitute(fld.Format(STR_WRITEFMT), "`archive", "`// archive");
            } else {
                ar_readStream << fld.Format((fld.is_flags & IS_POINTER) ? STR_PRTREADFMT : STR_READFMT);
                ar_writeStream << fld.Format((fld.is_flags & IS_POINTER) ? STR_PTRWRITEFMT : STR_WRITEFMT);
            }

            if ((fld.is_flags & IS_OBJECT) && !(fld.is_flags & IS_POINTER) && !contains(fld.type, "Array")) {
                if (child_objStream.str().empty())
                    child_objStream << "\n\n`// test for contained object field specifiers\n`string_q objSpec;\n";
                child_objStream << fld.Format(STR_CHILD_OBJS) << endl;
            }
        }
    }

    //------------------------------------------------------------------------------------------------
    string_q operators_decl = STR_OPERATOR_DECL;
    string_q operators_impl = STR_OPERATOR_IMPL;

    //------------------------------------------------------------------------------------------------
    classDef.sort_str = substitute(classDef.sort_str, "|", "\n```");
    classDef.eq_str = substitute(classDef.eq_str, "|", "\n```");

    //------------------------------------------------------------------------------------------------
    map<uint64_t, string_q> dispMap;
    for (auto fld : classDef.fieldArray)
        if (fld.disp > 0)
            dispMap[fld.disp] = fld.name;
    if (dispMap.size()) {
        string_q add = classDef.display_str;
        classDef.display_str = "";
        for (auto item : dispMap) {
            classDef.display_str += item.second + ",";
        }
        classDef.display_str = trim(classDef.display_str, ',');
        if (!add.empty())
            classDef.display_str += "," + add;
    }

    if (classDef.display_str.empty()) {
        classDef.display_str = " \"\"";
    } else {
        classDef.display_str = "\n`\"[{" + trim(toUpper(classDef.display_str)) + "}]\"";
        replaceAll(classDef.display_str, " ", "");
        replaceAll(classDef.display_str, ",", "}][PTAB]\"\n`\"[{");
    }

    //------------------------------------------------------------------------------------------------
    bool hasObjGetter = !fieldGetObj.empty() || toml.getConfigBool("settings", "force_objget", false);
    if (hasObjGetter)
        fieldGetObj = substitute(string_q(STR_GETOBJ_CODE), "[{FIELDS}]", fieldGetObj);

    //------------------------------------------------------------------------------------------------
    bool hasStrGetter = !fieldGetStr.empty();
    if (hasStrGetter)
        fieldGetStr = substitute(string_q(STR_GETSTR_CODE), "[{FIELDS}]", fieldGetStr);

    //------------------------------------------------------------------------------------------------
    ASSERT(!classDef.base_class.empty());
    bool isBase = (classDef.base_class == "CBaseNode");
    bool isContained = !classDef.contained_by.empty();

    string_q headerFile = classDef.outputPath(".h");
    string_q headSource = asciiFileToString(getPathToTemplates("blank.h"));
    replace(headSource, "// clang-format off\n", "");
    replace(headSource, "// clang-format on\n", "");

    replaceAll(headSource, "[{GET_OBJ}]", (hasObjGetter ? string_q(STR_GETOBJ_HEAD) + (hasStrGetter ? "" : "\n") : ""));
    replaceAll(headSource, "[{GET_STR}]", (hasStrGetter ? string_q(STR_GETSTR_HEAD) + "\n" : ""));
    replaceAll(headSource, "[FIELD_COPY]", copyStream.str());
    replaceAll(headSource, "[FIELD_DEC]", lheaderStream.str());
    replaceAll(headSource, "[FIELD_CLEAR]", clearStream.str());
    replaceAll(headSource, "[H_INCLUDES]", head_incStream.str());
    replaceAll(headSource, "[INIT_DEFAULTS]", defaultsStream.str());
    replaceAll(headSource, "[OPERATORS_DECL]", operators_decl);
    replaceAll(headSource, "[{COMMENT_LINE}]", STR_COMMENT_LINE);
    replaceAll(headSource, "[{BASE_CLASS}]", classDef.base_class);
    replaceAll(headSource, "[{LONG}]", classDef.base_lower);
    replaceAll(headSource, "[{SHORT}]", short2(classDef.base_lower));
    replaceAll(headSource, "[{SHORT3}]", short3(classDef.base_lower));
    replaceAll(headSource, "[{BASE_CLASS}]", classDef.base_class);
    replaceAll(headSource, "[{BASE_BASE}]", classDef.base_base);
    replaceAll(headSource, "[{BASE}]", classDef.base_upper);
    replaceAll(headSource, "[{PROPER}]", classDef.base_proper);
    replaceAll(headSource, "[{CLASS_NAME}]", classDef.class_name);
    replaceAll(headSource, "[{CLASS_BASE}]", classDef.class_base);
    replaceAll(headSource, "[{CLASS_UPPER}]", classDef.base_upper);
    replaceAll(headSource, "[{COMMENT_LINE}]", STR_COMMENT_LINE);
    replaceAll(headSource, "[{SORT_COMMENT}]", (classDef.sort_str.length() ? STR_SORT_COMMENT_1 : STR_SORT_COMMENT_2));
    replaceAll(headSource, "[{EQUAL_COMMENT}]", (classDef.eq_str.length() ? STR_EQUAL_COMMENT_1 : STR_EQUAL_COMMENT_2));
    replaceAll(headSource, "[{SORT_CODE}]", (classDef.sort_str.length() ? classDef.sort_str : "true"));
    replaceAll(headSource, "[{EQUAL_CODE}]", (classDef.eq_str.length() ? classDef.eq_str : "false"));
    replaceAll(headSource, "[{NAMESPACE1}]", "\nnamespace qblocks {\n\n");
    replaceAll(headSource, "[{NAMESPACE2}]", "}  // namespace qblocks\n");
    replaceAll(headSource, "public:\n\n  public:", "public:");
    expandTabbys(headSource);
    counter.nProcessed += writeCodeIn(codewrite_t(headerFile, headSource));

    //------------------------------------------------------------------------------------------------
    string_q srcFile = classDef.outputPath(".cpp");
    string_q srcSource = asciiFileToString(getPathToTemplates("blank.cpp"));
    replace(srcSource, "// clang-format off\n", "");
    replace(srcSource, "// clang-format on\n", "");
    if ((startsWith(classDef.class_name, "CNew") || classDef.class_name == "CPriceQuote") &&
        !contains(getCWD(), "parse"))
        replace(srcSource, "version of the data\n", STR_UPGRADE_CODE);
    replaceAll(srcSource, "[{GET_OBJ}]", fieldGetObj);
    replaceAll(srcSource, "[{GET_STR}]", fieldGetStr);
    replaceAll(srcSource, "[ARCHIVE_READ]", ar_readStream.str());
    replaceAll(srcSource, "[ARCHIVE_WRITE]", ar_writeStream.str());
    replaceAll(srcSource, "[OTHER_INCS]", src_incStream.str());
    replaceAll(srcSource, "[CHILD_OBJ_GETTER]", child_objStream.str());
    replaceAll(srcSource, "[ADD_FIELDS]", add_fieldStream.str());
    replaceAll(srcSource, "[HIDE_FIELDS]", hide_fieldStream.str());
    replaceAll(srcSource, "[SET_CASE_CODE]", getCaseSetCode(classDef.fieldArray));
    replaceAll(srcSource, "[GET_CASE_CODE]", getCaseGetCode(classDef.fieldArray));
    replaceAll(srcSource, "[OPERATORS_IMPL]", operators_impl);
    replaceAll(srcSource, "[{PARENT_SER}]", STR_PARENT_SERIALIZE);
    replaceAll(srcSource, "[{PARENT_REG}]", isBase ? "" : STR_PARENT_REGISTER);
    replaceAll(srcSource, "[{PARENT_SET}]", isBase ? "" : STR_PARENT_SET);
    replaceAll(srcSource, "[PARENT_GETBYVALUE]",
               isContained ? substitute(STR_PARENT_GETBYVALUE, "CONTAINED", classDef.contained_by) : "");
    replaceAll(srcSource, "[{COMMENT_LINE}]", STR_COMMENT_LINE);
    replaceAll(srcSource, "[{BASE_CLASS}]", classDef.base_class);
    replaceAll(srcSource, "[{LONG}]", classDef.base_lower);
    replaceAll(srcSource, "[{SHORT}]", short2(classDef.base_lower));
    replaceAll(srcSource, "[{SHORT3}]", short3(classDef.base_lower));
    replaceAll(srcSource, "[{BASE_CLASS}]", classDef.base_class);
    replaceAll(srcSource, "[{BASE_BASE}]", classDef.base_base);
    replaceAll(srcSource, "[{BASE}]", classDef.base_upper);
    replaceAll(srcSource, "[{PROPER}]", classDef.base_proper);
    replaceAll(srcSource, "[{CLASS_NAME}]", classDef.class_name);
    replaceAll(srcSource, "[{CLASS_BASE}]", classDef.class_base);
    replaceAll(srcSource, "[{CLASS_UPPER}]", classDef.base_upper);
    replaceAll(srcSource, "[{DISPLAY_FIELDS}]", classDef.display_str);
    replaceAll(srcSource, "[{NAMESPACE1}]", "\nnamespace qblocks {\n\n");
    replaceAll(srcSource, "[{NAMESPACE2}]", "}  // namespace qblocks\n");
    replaceAll(srcSource, "[{FN}]", classDef.short_fn);
    expandTabbys(srcSource);
    counter.nProcessed += writeCodeIn(codewrite_t(srcFile, srcSource));

    if (tsx && classDef.tsx)
        handle_tsx_type(classDef);

    return true;
}

//------------------------------------------------------------------------------------------------
string_q getCaseGetCode(const CParameterArray& fieldsIn) {
    if (fieldsIn.empty())
        return "// No fields";

    map<char, CParameterArray> ch_map;
    for (auto f : fieldsIn) {
        if (!(f.is_flags & IS_MINIMAL)) {
            ch_map[f.name[0]].push_back(f);
        }
    }

    ostringstream outStream;
    for (auto ar : ch_map) {
        outStream << ("``case '" + string_q(1, ar.first) + "':\n");
        for (auto p : ar.second) {
            if (p.name[0] == ar.first) {
                outStream << ("```if (fieldName % \"" + p.name + "\"");
                if (contains(p.type, "Array"))
                    outStream << (" || fieldName % \"" + p.name + "Cnt\"");
                outStream << (") {\n````");

                if (p.type == "Value") {
                    const char* STR_JSON_VALUE_STR =
                        "for (size_t i = 0; i < [{FIELD_ND}].size(); i++)\n"
                        "    ((CFunction*)this)->[{FIELD}][[{FIELD_ND}][i].name] = [{FIELD_ND}][i].value;\n"
                        "ostringstream os;\n"
                        "JsonWriter writer;\n"
                        "writer.writeJson(os, [{FIELD}]);\n"
                        "string_q str = os.str();\n"
                        "while (startsWith(str, '\\n') || startsWith(str, ' '))\n"
                        "    str = trim(trim(str, '\\n'), ' ');\n"
                        "return str;";
                    string_q str = substitute(STR_JSON_VALUE_STR, "\n", "\n````");
                    replaceAll(str, "[{FIELD}]", p.name);
                    replaceAll(str, "[{FIELD_ND}]", substitute(p.name, "_dict", ""));
                    outStream << str;

                } else if (p.type == "bool") {
                    if (p.is_flags & IS_OMITEMPTY)
                        outStream << ("return bool_2_Str_t([{PTR}]" + p.name + ");");
                    else
                        outStream << ("return bool_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "wei") {
                    outStream << ("return wei_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "gas") {
                    if (p.is_flags & IS_OMITEMPTY)
                        outStream << ("return " + p.name + " == 0 ? \"\" : gas_2_Str([{PTR}]" + p.name + ");");
                    else
                        outStream << ("return gas_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "timestamp") {
                    outStream << ("return ts_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "datetime") {
                    outStream << ("return [{PTR}]" + p.name + ".Format(FMT_JSON);");

                } else if (p.type == "addr" || p.type == "address") {
                    if (p.maxWidth != NOPOS) {
                        outStream << "return addr_2_Str([{PTR}]" << p.name << ".substr(0, " << p.maxWidth << "));";
                    } else {
                        outStream << "return addr_2_Str([{PTR}]" << p.name << ");";
                    }
                } else if (p.type == "hash") {
                    if (p.maxWidth != NOPOS) {
                        outStream << "return hash_2_Str([{PTR}]" << p.name << ".substr(0, " << p.maxWidth << "));";
                    } else {
                        outStream << "return hash_2_Str([{PTR}]" << p.name << ");";
                    }
                } else if (startsWith(p.type, "bytes")) {
                    if (p.maxWidth != NOPOS) {
                        outStream << "return [{PTR}]" << p.name << ".substr(0, " << p.maxWidth << ");";
                    } else {
                        outStream << "return [{PTR}]" << p.name << ";";
                    }

                } else if (p.type == "uint8") {
                    outStream << ("return uint_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "uint16") {
                    outStream << ("return uint_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "uint32" || p.type == "uint64") {
                    if (p.is_flags & IS_OMITEMPTY)
                        outStream << ("return " + p.name + " == 0 ? \"\" : uint_2_Str([{PTR}]" + p.name + ");");
                    else
                        outStream << ("return uint_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "blknum") {
                    outStream << ("return uint_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "uint256") {
                    outStream << ("return bnu_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "int8") {
                    outStream << ("return int_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "int16") {
                    outStream << ("return int_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "int32" || p.type == "int64") {
                    outStream << ("return int_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "int256") {
                    outStream << ("return bni_2_Str([{PTR}]" + p.name + ");");

                } else if (p.type == "double") {
                    outStream << "return double_2_Str([{PTR}]" << p.name << ", " << p.precision << ");";

                } else if (endsWith(p.type, "_e")) {
                    outStream << ("return uint_2_Str(" + p.name + ");");

                } else if (contains(p.type, "CStringArray") || contains(p.type, "CAddressArray")) {
                    string_q str = substitute(STR_CASE_CODE_STRINGARRAY, "\n", "\n````");
                    replaceAll(str, "[{FIELD}]", p.name);
                    outStream << (str);

                } else if (contains(p.type, "CBigUintArray") || contains(p.type, "CTopicArray")) {
                    string_q str = substitute(STR_CASE_CODE_STRINGARRAY, "\n", "\n````");
                    // hack for size clause
                    replace(str, "[{FIELD}]", p.name);
                    // hack for the array access
                    replace(str, "[{FIELD}][i]", "topic_2_Str(" + p.name + "[i])");
                    outStream << (str);

                } else if (contains(p.type, "Array")) {
                    const char* STR_CASE_CODE_ARRAY =
                        "size_t cnt = [{PTR}][{FIELD}].size();\n"
                        "if (endsWith(toLower(fieldName), \"cnt\"))\n"
                        "`return uint_2_Str(cnt);\n"
                        "if (!cnt)\n"
                        "`return \"\";\n"
                        "string_q retS;\n"
                        "for (size_t i = 0; i < cnt; i++) {\n"
                        "`retS += [{PTR}][{FIELD}][i].Format();\n"
                        "`retS += ((i < cnt - 1) ? \",\\n\" : \"\\n\");\n"
                        "}\n"
                        "return retS;";
                    string_q str = substitute(STR_CASE_CODE_ARRAY, "\n", "\n````");
                    if (contains(p.type, "CBlockNum"))
                        replaceAll(str, "[{PTR}][{FIELD}][i].Format()", "uint_2_Str([{PTR}][{FIELD}][i])");
                    replaceAll(str, "[{FIELD}]", p.name);
                    if ((p.is_flags & IS_POINTER))
                        replace(str, "[i].Format", "[i]->Format");
                    outStream << (str);

                } else if (contains(p.type, "List") || (p.is_flags & IS_POINTER)) {
                    const char* PTR_GET_CASE =
                        "if ([{NAME}])\n"
                        "`return [{NAME}]->Format();\n"
                        "return \"\";";
                    string_q ptrCase = substitute(PTR_GET_CASE, "\n", "\n````");
                    replaceAll(ptrCase, "[{NAME}]", p.name);
                    replaceAll(ptrCase, "[{TYPE}]", p.type);
                    outStream << (ptrCase);

                } else if ((p.is_flags & IS_OBJECT)) {
                    const char* STR_OBJECT_CASE =
                        "if ([{NAME}] == [{TYPE}]())\n"
                        "`return \"{}\";\n"
                        "return [{PTR}][{NAME}].Format();";
                    string_q objCase = substitute(STR_OBJECT_CASE, "\n", "\n````");
                    replaceAll(objCase, "[{NAME}]", p.name);
                    replaceAll(objCase, "[{TYPE}]", p.type);
                    outStream << (objCase);

                } else {
                    if (p.maxWidth != NOPOS) {
                        outStream << "return [{PTR}]" << p.name << ".substr(0, " << p.maxWidth << ");";
                    } else {
                        outStream << "return [{PTR}]" << p.name << ";";
                    }
                }
                outStream << endl << "```}" << endl;
            }
        }
        outStream << ("```break;\n");
    }
    outStream << "``default:\n```break;\n";

    string out = "// Return field values\n`switch (tolower(fieldName[0])) {\n" + outStream.str() + "`}\n";
    replaceAll(out, "[{PTR}]", "");
    return out;
}

//------------------------------------------------------------------------------------------------
string_q getCaseSetCode(const CParameterArray& fieldsIn) {
    map<char, CParameterArray> ch_map;
    for (auto f : fieldsIn) {
        if (!(f.is_flags & IS_MINIMAL)) {
            ch_map[f.name[0]].push_back(f);
        }
    }

    ostringstream outStream;
    for (auto ar : ch_map) {
        outStream << ("``case '" + string_q(1, ar.first) + "':\n");
        for (auto p : ar.second) {
            if (p.name[0] == ar.first) {
                outStream << ("```if (fieldName % \"" + p.name + "\") {\n````");
                if (p.type == "Value") {
                    outStream << (p.name + " = fieldValue;\n````return true;");

                } else if (p.type == "bool") {
                    outStream << (p.name + " = str_2_Bool(fieldValue);\n````return true;");

                } else if (p.type == "wei") {
                    outStream << (p.name + " = str_2_Wei(fieldValue);\n````return true;");

                } else if (p.type == "gas") {
                    outStream << (p.name + " = str_2_Gas(fieldValue);\n````return true;");

                } else if (p.type == "timestamp") {
                    outStream << (p.name + " = str_2_Ts(fieldValue);\n````return true;");

                } else if (p.type == "datetime") {
                    outStream << (p.name + " = str_2_Date(fieldValue);\n````return true;");

                } else if (p.type == "address") {
                    if (p.maxWidth != NOPOS) {
                        outStream << p.name << " = str_2_Addr("
                                  << "fieldValue.substr(0, " << p.maxWidth << ")"
                                  << ");\n````return true;";
                    } else {
                        outStream << (p.name + " = str_2_Addr(fieldValue);\n````return true;");
                    }
                } else if (p.type == "hash") {
                    if (p.maxWidth != NOPOS) {
                        outStream << p.name << " = str_2_Hash("
                                  << "fieldValue.substr(0, " << p.maxWidth << ")"
                                  << ");\n````return true;";
                    } else {
                        outStream << (p.name + " = str_2_Hash(fieldValue);\n````return true;");
                    }
                } else if (p.type == "blknum") {
                    outStream << (p.name + " = str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "double") {
                    outStream << (p.name + " = str_2_Double(fieldValue);\n````return true;");

                } else if (endsWith(p.type, "_e")) {
                    outStream << (p.name + " = str_2_Enum(" + p.type + ", fieldValue);\n````return true;");

                } else if (p.type == "int64") {
                    outStream << (p.name + " = str_2_Int(fieldValue);\n````return true;");

                } else if (p.type == "int256") {
                    outStream << (p.name + " = str_2_BigInt(fieldValue);\n````return true;");

                } else if (p.type == "uint64") {
                    outStream << (p.name + " = str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "uint256") {
                    outStream << (p.name + " = str_2_Wei(fieldValue);\n````return true;");

                } else if (p.type == "int8") {
                    outStream << (p.name + " = (int8_t)str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "int16") {
                    outStream << (p.name + " = (int16_t)str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "int32") {
                    outStream << (p.name + " = (int32_t)str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "uint8") {
                    outStream << (p.name + " = (uint8_t)str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "uint16") {
                    outStream << (p.name + " = (uint16_t)str_2_Uint(fieldValue);\n````return true;");

                } else if (p.type == "uint32") {
                    outStream << (p.name + " = (uint32_t)str_2_Uint(fieldValue);\n````return true;");

                } else if (startsWith(p.type, "bytes")) {
                    outStream << (p.name + " = toLower(fieldValue);\n````return true;");

                } else if (contains(p.type, "CStringArray") || contains(p.type, "CBlockNumArray")) {
                    const char* STR_ARRAY_SET =
                        "string_q str = fieldValue;\n"
                        "while (!str.empty()) {\n"
                        "`[{NAME}].push_back(nextTokenClear(str, ','));\n"
                        "}\n"
                        "return true;";
                    string_q str = substitute(STR_ARRAY_SET, "\n", "\n````");
                    replaceAll(str, "[{NAME}]", p.name);
                    if (contains(p.type, "CBlockNumArray"))
                        replaceAll(str, "nextTokenClear(str, ',')", "str_2_Uint(nextTokenClear(str, ','))");
                    outStream << (str);

                } else if (contains(p.type, "CAddressArray") || contains(p.type, "CBigUintArray") ||
                           contains(p.type, "CTopicArray")) {
                    const char* STR_ARRAY_SET =
                        "string_q str = fieldValue;\n"
                        "while (!str.empty()) {\n"
                        "`[{NAME}].push_back(nextTokenClear(str, ','));\n"
                        "}\n"
                        "return true;";
                    string_q str = substitute(STR_ARRAY_SET, "\n", "\n````");
                    replaceAll(str, "[{NAME}]", p.name);
                    replaceAll(str, "nextTokenClear(str, ',')", "str_2_[{TYPE}](nextTokenClear(str, ','))");
                    replaceAll(str, "[{TYPE}]",
                               substitute(substitute(extract(p.type, 1), "Array", ""), "Address", "Addr"));
                    outStream << (str);

                } else if ((p.is_flags & IS_POINTER)) {
                    const char* PTR_SET_CASE =
                        "// This drops memory, so we comment it out for now\n"
                        "clear();\n"
                        "[{NAME}] = new [{TYPE}];\n"
                        "if ([{NAME}]) {\n"
                        "`string_q str = fieldValue;\n"
                        "`return [{NAME}]->parseJson3(str);\n"
                        "}\n"
                        "return false;";
                    string_q ptrCase = substitute(PTR_SET_CASE, "\n", "\n````// ");
                    replace(ptrCase, "// return false;", "return false;");
                    replaceAll(ptrCase, "[{NAME}]", p.name);
                    replaceAll(ptrCase, "[{TYPE}]", p.type);
                    outStream << ptrCase;

                } else if (contains(p.type, "Array")) {
                    const char* STR_CASE_SET_CODE_ARRAY =
                        "[{TYPE}] obj;\n"
                        "string_q str = fieldValue;\n"
                        "while (obj.parseJson3(str)) {\n"
                        "`[{NAME}].push_back(obj);\n"
                        "`obj = [{TYPE}]();  // reset\n"
                        "}\n"
                        "return true;";
                    string_q str = substitute(STR_CASE_SET_CODE_ARRAY, "\n", "\n````");
                    replaceAll(str, "[{NAME}]", p.name);
                    replaceAll(str, "[{TYPE}]", substitute(p.type, "Array", ""));
                    outStream << (str);

                } else if ((p.is_flags & IS_OBJECT)) {
                    outStream << ("return " + p.name + ".parseJson3(fieldValue);");

                } else {
                    if (p.maxWidth != NOPOS) {
                        outStream << p.name << " = "
                                  << "fieldValue.substr(0, " << p.maxWidth << ")"
                                  << ";\n````return true;";
                    } else {
                        outStream << (p.name + " = fieldValue;\n````return true;");
                    }
                }
                outStream << endl << "```}" << endl;
            }
        }
        outStream << ("```break;\n");
    }
    outStream << "``default:\n```break;\n";

    return outStream.str();
}

//------------------------------------------------------------------------------------------------------------
string_q convertTypes(const string_q& inStr) {
    // Note: Look out for trailing spaces. They are here to make sure it
    // matches only the types and not the field names.
    string_q outStr = inStr;
    replaceAll(outStr, "address ", "address_t ");

    replaceAll(outStr, "bytes ", "string_q ");
    replaceAll(outStr, "bytes4 ", "string_q ");
    replaceAll(outStr, "bytes8 ", "string_q ");
    replaceAll(outStr, "bytes16 ", "string_q ");
    replaceAll(outStr, "bytes32 ", "string_q ");
    replaceAll(outStr, "string ", "string_q ");
    replaceAll(outStr, "blknum ", "blknum_t ");
    replaceAll(outStr, "timestamp ", "timestamp_t ");
    replaceAll(outStr, "datetime ", "time_q ");
    replaceAll(outStr, "bool ", "bool ");

    replaceAll(outStr, "hash ", "hash_t ");
    replaceAll(outStr, "gas ", "gas_t ");
    replaceAll(outStr, "wei ", "wei_t ");

    replaceAll(outStr, "int8 ", "int8_t ");
    replaceAll(outStr, "int16 ", "int16_t ");
    replaceAll(outStr, "int32 ", "int32_t ");
    replaceAll(outStr, "int64 ", "int64_t ");
    replaceAll(outStr, "int256 ", "bigint_t ");
    replaceAll(outStr, "ubigint_t", "biguint_t");

    return outStr;
}

//------------------------------------------------------------------------------------------------------------
const char* STR_COMMENT_LINE = "//---------------------------------------------------------------------------\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_OPERATOR_DECL =
    "[{COMMENT_LINE}]"
    "extern CArchive& operator<<(CArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]);\n"
    "extern CArchive& operator>>(CArchive& archive, [{CLASS_NAME}]& [{SHORT3}]);\n"
    "\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_PARENT_GETBYVALUE =
    "`extern string_q nextCONTAINEDChunk(const string_q& fieldIn, const void* data);\n"
    "`ret = nextCONTAINEDChunk(fieldName, pCONTAINED);\n"
    "`if (contains(ret, \"Field not found\"))\n"
    "`    ret = \"\";\n"
    "`if (!ret.empty())\n"
    "`    return ret;\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_PARENT_REGISTER = "[{BASE_CLASS}]::registerClass();\n\n`";

//------------------------------------------------------------------------------------------------------------
const char* STR_PARENT_SET = "`if ([{BASE_CLASS}]::setValueByName(fieldName, fieldValue))\n``return true;\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_PARENT_SERIALIZE = "`[{BASE_CLASS}]::SerializeC(archive);\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_OPERATOR_IMPL =
    "[{COMMENT_LINE}]"
    "CArchive& operator<<(CArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]) {\n"
    "`[{SHORT3}].SerializeC(archive);\n"
    "`return archive;\n"
    "}\n"
    "\n"
    "[{COMMENT_LINE}]"
    "CArchive& operator>>(CArchive& archive, [{CLASS_NAME}]& [{SHORT3}]) {\n"
    "`[{SHORT3}].Serialize(archive);\n"
    "`return archive;\n"
    "}\n"
    "\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_PRTREADFMT =
    "`[{NAME}] = NULL;\n"
    "`bool has_[{NAME}] = false;\n"
    "`archive >> has_[{NAME}];\n"
    "`if (has_[{NAME}]) {\n"
    "``string_q className;\n"
    "``archive >> className;\n"
    "``[{NAME}] = reinterpret_cast<[{TYPE}]*>(createObjectOfType(className));\n"
    "``if (![{NAME}])\n"
    "```return false;\n"
    "``[{NAME}]->Serialize(archive);\n"
    "`}\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_PTRWRITEFMT =
    "`archive << ([{NAME}] != NULL);\n"
    "`if ([{NAME}]) {\n"
    "``archive << [{NAME}]->getRuntimeClass()->getClassNamePtr();\n"
    "``[{NAME}]->SerializeC(archive);\n"
    "`}\n";

const char* STR_GETOBJ_HEAD =
    "`const CBaseNode* getObjectAt(const string_q& fieldName, size_t index) const override;\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETOBJ_CODE_FIELD =
    "`if (fieldName % \"[{FIELD}]\") {\n"
    "``if (index == NOPOS) {\n"
    "```[{TYPE}] empty;\n"
    "```(([{CLASS_NAME}]*)this)->[{FIELD}].push_back(empty);  // NOLINT\n"
    "```index = [{FIELD}].size() - 1;\n"
    "``}\n"
    "``if (index < [{FIELD}].size())\n"
    "```return [PTR][{FIELD}][index];\n"
    "`}\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETOBJ_CODE_FIELD_OBJ =
    "`if (fieldName % \"[{FIELD}]\")\n"
    "``return [PTR][{FIELD}];\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETOBJ_CODE =
    "//---------------------------------------------------------------------------\n"
    "const CBaseNode* [{CLASS_NAME}]::getObjectAt(const string_q& fieldName, size_t index) const {\n"
    "`// EXISTING_CODE\n"
    "`// EXISTING_CODE\n"
    "[{FIELDS}]`// EXISTING_CODE\n`// EXISTING_CODE\n\n`return NULL;\n"
    "}\n\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETSTR_HEAD = "`const string_q getStringAt(const string_q& fieldName, size_t i) const override;\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETSTR_CODE_FIELD =
    "`if (fieldName % \"[{FIELD}]\" && i < [{FIELD}].size())\n"
    "``return THING([{FIELD}][i]);\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETSTR_CODE =
    "//---------------------------------------------------------------------------\n"
    "const string_q [{CLASS_NAME}]::getStringAt(const string_q& fieldName, size_t i) const {\n"
    "[{FIELDS}]`return \"\";\n"
    "}\n\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_UPGRADE_CODE =
    "version of the data\n`(([{CLASS_NAME}]*)this)->m_schema = getVersionNum();  /"
    "/ NOLINT\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_SORT_COMMENT_1 = "Default sort as defined in class definition";

//------------------------------------------------------------------------------------------------------------
const char* STR_SORT_COMMENT_2 =
    "No default sort defined in class definition, assume already sorted, preserve ordering";

//------------------------------------------------------------------------------------------------------------
const char* STR_EQUAL_COMMENT_1 = "Equality operator as defined in class definition";

//------------------------------------------------------------------------------------------------------------
const char* STR_EQUAL_COMMENT_2 =
    "No default equal operator in class definition, assume none are equal (so find fails)";

//------------------------------------------------------------------------------------------------------------
const char* STR_UNKOWNTYPE =
    "//`[{NAME}] = ??; /"
    "* unknown type: [{TYPE}] */\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_CHILD_OBJS =
    "`objSpec = toUpper(\"[{NAME}]\") + \"::\";\n"
    "`if (contains(fieldName, objSpec))\n"
    "``return [{NAME}].getValueByName(substitute(fieldName, objSpec, \"\"));\n"
    "\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_READFMT = "`archive >> [{NAME}];\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_WRITEFMT = "`archive << [{NAME}];\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_STRINGARRAY =
    "size_t cnt = [{PTR}][{FIELD}].size();\n"
    "if (endsWith(toLower(fieldName), \"cnt\"))\n"
    "`return uint_2_Str(cnt);\n"
    "if (!cnt)\n"
    "`return \"\";\n"
    "string_q retS;\n"
    "for (size_t i = 0; i < cnt; i++) {\n"
    "`retS += (\"\\\"\" + [{PTR}][{FIELD}][i] + \"\\\"\");\n"
    "`retS += ((i < cnt - 1) ? \",\\n\" + indentStr() : \"\\n\");\n"
    "}\n"
    "return retS;";
