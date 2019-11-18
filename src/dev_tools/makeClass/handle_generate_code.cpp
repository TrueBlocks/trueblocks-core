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

//------------------------------------------------------------------------------------------------------------
bool COptions::handle_generate(CToml& toml, const CClassDefinition& classDef, const string_q& ns) {
    //------------------------------------------------------------------------------------------------
    if (!toml.getConfigBool("settings", "enabled", true))
        return true;

    //------------------------------------------------------------------------------------------------
    string_q class_name = toml.getConfigStr("settings", "class", "");
    string_q base_class = toml.getConfigStr("settings", "base_class", "CBaseNode");
    string_q fields = toml.getConfigStr("settings", "fields", "");
    string_q head_includes = toml.getConfigStr("settings", "includes", "");
    string_q src_includes = toml.getConfigStr("settings", "cpp_includes", "");
    string_q display_str = toml.getConfigStr("settings", "display_str", "");
    string_q sort_str = toml.getConfigStr("settings", "sort", "");
    string_q eq_str = toml.getConfigStr("settings", "equals", "");
    string_q scope_str = toml.getConfigStr("settings", "scope", "static");  // TODO(tjayrush): global data
    bool serializable = toml.getConfigBool("settings", "serializable", false);
    bool use_export = toml.getConfigBool("settings", "use_export", false);

    //------------------------------------------------------------------------------------------------
    string_q class_base = toProper(extract(class_name, 1));
    string_q class_upper = toUpper(class_base);

    //------------------------------------------------------------------------------------------------
    string_q base_name = extract(class_name, 1);
    string_q base_proper = toProper(base_name);
    string_q base_lower = toLower(base_name);
    string_q base_upper = toUpper(base_name);
    string_q base_base = toProper(extract(base_class, 1));

    //------------------------------------------------------------------------------------------------
    ostringstream declare_stream, clear_stream, copy_stream;
    ostringstream ar_read_stream, ar_write_stream;
    ostringstream src_inc_stream, head_inc_stream;
    ostringstream child_obj_stream, add_field_stream, hide_field_stream;
    ostringstream get_case_stream, defaults_stream;

    //------------------------------------------------------------------------------------------------
    string_q fieldGetObj, fieldGetStr;

    //------------------------------------------------------------------------------------------------
    CStringArray h_incs;
    explode(h_incs, head_includes, '|');
    for (auto inc : h_incs)
        head_inc_stream << ("#include \"" + inc + "\"\n");

    //------------------------------------------------------------------------------------------------
    CStringArray c_inc;
    explode(c_inc, src_includes, '|');
    for (auto inc : c_inc)
        src_inc_stream << ("#include \"" + inc + "\"\n");

    //------------------------------------------------------------------------------------------------
    bool isBase = (base_class == "CBaseNode");
    // clang-format off
    string_q parSer2 = !isBase ? "`[{BASE_CLASS}]::SerializeC(archive);\n\n"        : "`[{BASE_CLASS}]::SerializeC(archive);\n";
    string_q parReg  = !isBase ? "[{BASE_CLASS}]::registerClass();\n\n`"            : "";
    string_q parCnk  = !isBase ? "ret = next[{BASE_BASE}]Chunk(fieldName, this);\n" : "ret = next[{BASE_BASE}]Chunk(fieldName, this);\n";
    string_q parSet  = !isBase ? "`if ([{BASE_CLASS}]::setValueByName(fieldName, fieldValue))\n``return true;\n\n" : "";
    // clang-format on

    //------------------------------------------------------------------------------------------------
    CParameterArray fieldArray;
    explode(fieldArray, fields, '|');
    for (auto fld : fieldArray) {
        // keep these in this scope since they may change per field
        string_q declareFmt = "`[{TYPE}]* [{NAME}];";
        string_q regAddFmt = "`ADD_FIELD(CL_NM, \"[{NAME}]\", T_TEXT, ++fieldNum);\n";
        string_q regHideFmt = "`HIDE_FIELD(CL_NM, \"[{NAME}]\");\n";
        string_q ptrClearFmt = "`if ([{NAME}])\n``delete [{NAME}];\n`[{NAME}] = NULL;\n";
        string_q caseFmt = "[{TYPE}]+[{NAME}]-[{ISPOINTER}]~[{ISOBJECT}]|";
        string_q copyFmt = "`[{NAME}] = ++SHORT++.[{NAME}];\n";

        // order matters in the next block
        string_q setFmt, regType;
        // clang-format off
               if (fld.type == "bloom")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_BLOOM";
        } else if (fld.type == "wei")            { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_WEI";
        } else if (fld.type == "gas")            { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_GAS";
        } else if (fld.type == "timestamp")      { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_TIMESTAMP";
        } else if (fld.type == "blknum")         { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "string")         { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_TEXT";
        } else if (fld.type == "addr")           { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_ADDRESS";
        } else if (fld.type == "address")        { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_ADDRESS";
        } else if (fld.type == "hash")           { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_HASH";
        } else if (fld.type == "int8")           { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int16")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int32")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int64")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "int256")         { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint8")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint16")         { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint32")         { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint64")         { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "uint256")        { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.type == "bool")           { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_BOOL";
        } else if (fld.type == "sbool")          { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_BOOL";
        } else if (fld.type == "double")         { setFmt = "`[{NAME}] = [{DEFF}];\n";   regType = "T_DOUBLE";
        } else if (startsWith(fld.type, "bytes")) { setFmt = "`[{NAME}] = [{DEFS}];\n";   regType = "T_TEXT";
        } else if (endsWith(fld.type, "_e"))     { setFmt = "`[{NAME}] = [{DEF}];\n";    regType = "T_NUMBER";
        } else if (fld.isPointer)                { setFmt = "`[{NAME}] = [{DEFP}];\n";   regType = "T_POINTER";
        } else if (fld.isObject)                 { setFmt = "`[{NAME}] = [{TYPE}]();\n"; regType = "T_OBJECT";
        } else                                   { setFmt = STR_UNKOWNTYPE;              regType = "T_TEXT"; }
        // clang-format on

        if (contains(fld.type, "Array")) {
            setFmt = "\t[{NAME}].clear();\n";
            if (contains(fld.type, "Address")) {
                regType = "T_ADDRESS | TS_ARRAY";
            } else if (contains(fld.type, "String")) {
                regType = "T_TEXT | TS_ARRAY";
            } else {
                regType = "T_OBJECT | TS_ARRAY";
            }
        }

        if (contains(fld.type, "CStringArray") || contains(fld.type, "CBlockNumArray") ||
            contains(fld.type, "CAddressArray") || contains(fld.type, "CBigUintArray") ||
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

        } else if (fld.isObject && !fld.isPointer) {
            fieldGetObj += STR_GETOBJ_CODE_FIELD;
            if (!contains(fld.type, "Array")) {
                replace(fieldGetObj, " && index < [{FIELD}].size()", "");
                replace(fieldGetObj, "[index]", "");
            }
            replaceAll(fieldGetObj, "[{FIELD}]", fld.name);
        }

        replace(setFmt, "[{DEFS}]", fld.strDefault.empty() ? "\"\"" : fld.strDefault);
        replace(setFmt, "[{DEF}]", fld.strDefault.empty() ? "0" : fld.strDefault);
        replace(setFmt, "[{DEFF}]", fld.strDefault.empty() ? "0.0" : fld.strDefault);
        replace(setFmt, "[{DEFT}]", fld.strDefault.empty() ? "earliestDate" : fld.strDefault);
        replace(setFmt, "[{DEFP}]", fld.strDefault.empty() ? "NULL" : fld.strDefault);

        if (fld.isPointer)
            copyFmt =
                "`if ([++SHORT++.{NAME}]) {\n``[{NAME}] = new [{TYPE}];\n``*[{NAME}] = *[++SHORT++.{NAME}];\n`}\n";

        if (!fld.isPointer)
            replace(declareFmt, "*", "");

        add_field_stream << substitute(substitute(fld.Format(regAddFmt), "T_TEXT", regType), "CL_NM", "[{CLASS_NAME}]");
        if (fld.noWrite)
            add_field_stream << substitute(fld.Format(regHideFmt), "CL_NM", "[{CLASS_NAME}]");

        // noWrite-min means that a field is part of the object's data (such as CReceipt::blockNumber) but should not be
        // part of the 'class' proper -- i.e. it gets its value from its containing class (in this case the block it
        // belongs to).
        if (!fld.noWrite_min) {
            get_case_stream << fld.Format(caseFmt);
            declare_stream << convertTypes(fld.Format(declareFmt)) << endl;
            copy_stream << substitute(substitute(fld.Format(copyFmt), "++SHORT++", "[{SHORT}]"), "++CLASS++",
                                      "[{CLASS_NAME}]");
            defaults_stream << fld.Format(setFmt);
            clear_stream << (fld.isPointer ? fld.Format(ptrClearFmt) : "");
            ar_read_stream << substitute(fld.Format(fld.isPointer ? STR_PRTREADFMT : STR_READFMT), "`archive",
                                         (fld.noWrite ? "`// archive" : "`archive"));
            ar_write_stream << substitute(fld.Format(fld.isPointer ? STR_PTRWRITEFMT : STR_WRITEFMT), "`archive",
                                          (fld.noWrite ? "`// archive" : "`archive"));

            if (fld.isObject && !fld.isPointer && !contains(fld.type, "Array")) {
                if (child_obj_stream.str().empty())
                    child_obj_stream << "\n`string_q s;\n";
                child_obj_stream << fld.Format(STR_CHILD_OBJS) << endl;
            }
        }
    }

    //------------------------------------------------------------------------------------------------
    string_q operators_decl = string_q(serializable ? STR_OPERATOR_DECL : "\n");
    string_q operators_impl = string_q(serializable ? STR_OPERATOR_IMPL : "\n");

    //------------------------------------------------------------------------------------------------
    sort_str = substitute(sort_str, "|", "\n```");
    eq_str = substitute(eq_str, "|", "\n```");

    //------------------------------------------------------------------------------------------------
    if (display_str.empty()) {
        display_str = " \"\"";
    } else {
        display_str = "\n`\"[{" + trim(toUpper(display_str)) + "}]\"";
        replaceAll(display_str, " ", "");
        replaceAll(display_str, ",", "}][PTAB]\"\n`\"[{");
    }

    //------------------------------------------------------------------------------------------------
    bool hasObjGetter = !fieldGetObj.empty();
    if (hasObjGetter)
        fieldGetObj = substitute(string_q(STR_GETOBJ_CODE), "[{FIELDS}]", fieldGetObj);

    //------------------------------------------------------------------------------------------------
    bool hasStrGetter = !fieldGetStr.empty();
    if (hasStrGetter)
        fieldGetStr = substitute(string_q(STR_GETSTR_CODE), "[{FIELDS}]", fieldGetStr);

    //------------------------------------------------------------------------------------------------
    string_q headerFile = classDef.outputPath(".h");
    string_q headSource = asciiFileToString(configPath("makeClass/blank.h"));
    replace(headSource, "// clang-format off\n", "");
    replace(headSource, "// clang-format on\n", "");

    replaceAll(headSource, "[{GET_OBJ}]", (hasObjGetter ? string_q(STR_GETOBJ_HEAD) + (hasStrGetter ? "" : "\n") : ""));
    replaceAll(headSource, "[{GET_STR}]", (hasStrGetter ? string_q(STR_GETSTR_HEAD) + "\n" : ""));
    replaceAll(headSource, "[FIELD_COPY]", copy_stream.str());
    replaceAll(headSource, "[FIELD_DEC]", declare_stream.str());
    replaceAll(headSource, "[FIELD_CLEAR]", clear_stream.str());
    replaceAll(headSource, "[H_INCLUDES]", head_inc_stream.str());
    replaceAll(headSource, "[INIT_DEFAULTS]", defaults_stream.str());
    replaceAll(headSource, "[OPERATORS_DECL]", operators_decl);
    replaceAll(headSource, "[{COMMENT_LINE}]", STR_COMMENT_LINE);
    replaceAll(headSource, "[{BASE_CLASS}]", base_class);
    replaceAll(headSource, "[{LONG}]", base_lower);
    replaceAll(headSource, "[{SHORT}]", short2(base_lower));
    replaceAll(headSource, "[{SHORT3}]", short3(base_lower));
    replaceAll(headSource, "[{BASE_CLASS}]", base_class);
    replaceAll(headSource, "[{BASE_BASE}]", base_base);
    replaceAll(headSource, "[{BASE}]", base_upper);
    replaceAll(headSource, "[{PROPER}]", base_proper);
    replaceAll(headSource, "[{CLASS_NAME}]", class_name);
    replaceAll(headSource, "[{CLASS_BASE}]", class_base);
    replaceAll(headSource, "[{CLASS_UPPER}]", class_upper);
    replaceAll(headSource, "[{COMMENT_LINE}]", STR_COMMENT_LINE);
    replaceAll(headSource, "[{SORT_COMMENT}]", (sort_str.length() ? STR_SORT_COMMENT_1 : STR_SORT_COMMENT_2));
    replaceAll(headSource, "[{EQUAL_COMMENT}]", (eq_str.length() ? STR_EQUAL_COMMENT_1 : STR_EQUAL_COMMENT_2));
    replaceAll(headSource, "[{SORT_CODE}]", (sort_str.length() ? sort_str : "true"));
    replaceAll(headSource, "[{EQUAL_CODE}]", (eq_str.length() ? eq_str : "false"));
    replaceAll(headSource, "[{NAMESPACE1}]", (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    replaceAll(headSource, "[{NAMESPACE2}]", (ns.empty() ? "" : "}  // namespace qblocks\n"));
    replaceAll(headSource, "public:\n\n  public:", "public:");
    replaceAll(headSource, "`````", string_q(5, '\t'));
    replaceAll(headSource, "````", string_q(4, '\t'));
    replaceAll(headSource, "```", string_q(3, '\t'));
    replaceAll(headSource, "``", string_q(2, '\t'));
    replaceAll(headSource, "`", string_q(1, '\t'));
    // writeTheCode returns true or false depending on if it WOULD HAVE written the file. If 'test' is true, it doesn't
    // actually write the file
    bool wouldHaveWritten = writeTheCode(headerFile, headSource, ns, true, test);
    if (wouldHaveWritten && test)
        cerr << "File '" << headerFile << "' changed but was not written because of testing." << endl;

    //------------------------------------------------------------------------------------------------
    string_q srcFile = classDef.outputPath(".cpp");
    string_q srcSource = asciiFileToString(configPath("makeClass/blank.cpp"));
    replace(srcSource, "// clang-format off\n", "");
    replace(srcSource, "// clang-format on\n", "");
    if (use_export)
        replace(srcSource, "ctx << toJson();", "doExport(ctx);");
    if ((startsWith(class_name, "CNew") || class_name == "CPriceQuote") && !contains(getCWD(), "parse"))
        replace(srcSource, "version of the data\n", STR_UPGRADE_CODE);
    replaceAll(srcSource, "[{GET_OBJ}]", fieldGetObj);
    replaceAll(srcSource, "[{GET_STR}]", fieldGetStr);
    replaceAll(srcSource, "[ARCHIVE_READ]", ar_read_stream.str());
    replaceAll(srcSource, "[ARCHIVE_WRITE]", ar_write_stream.str());
    replaceAll(srcSource, "[OTHER_INCS]", src_inc_stream.str());
    replaceAll(srcSource, "[CHILD_OBJ_GETTER]", child_obj_stream.str());
    replaceAll(srcSource, "[ADD_FIELDS]", add_field_stream.str());
    replaceAll(srcSource, "[HIDE_FIELDS]", hide_field_stream.str());
    replaceAll(srcSource, "[SET_CASE_CODE]", getCaseSetCode(get_case_stream.str()));
    replaceAll(srcSource, "[GET_CASE_CODE]", getCaseGetCode(get_case_stream.str()));
    replaceAll(srcSource, "[SCOPE_CODE]", scope_str);
    replaceAll(srcSource, "[OPERATORS_IMPL]", operators_impl);
    replaceAll(srcSource, "[{PARENT_SER2}]", parSer2);
    replaceAll(srcSource, "[{PARENT_REG}]", parReg);
    replaceAll(srcSource, "[{PARENT_CHNK}]\n", parCnk);
    replaceAll(srcSource, "[{PARENT_SET}]\n", parSet);
    replaceAll(srcSource, "[{COMMENT_LINE}]", STR_COMMENT_LINE);
    replaceAll(srcSource, "[{BASE_CLASS}]", base_class);
    replaceAll(srcSource, "[{LONG}]", base_lower);
    replaceAll(srcSource, "[{SHORT}]", short2(base_lower));
    replaceAll(srcSource, "[{SHORT3}]", short3(base_lower));
    replaceAll(srcSource, "[{BASE_CLASS}]", base_class);
    replaceAll(srcSource, "[{BASE_BASE}]", base_base);
    replaceAll(srcSource, "[{BASE}]", base_upper);
    replaceAll(srcSource, "[{PROPER}]", base_proper);
    replaceAll(srcSource, "[{CLASS_NAME}]", class_name);
    replaceAll(srcSource, "[{CLASS_BASE}]", class_base);
    replaceAll(srcSource, "[{CLASS_UPPER}]", class_upper);
    replaceAll(srcSource, "[{DISPLAY_FIELDS}]", display_str);
    replaceAll(srcSource, "[{NAMESPACE1}]", (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    replaceAll(srcSource, "[{NAMESPACE2}]", (ns.empty() ? "" : "}  // namespace qblocks\n"));
    replaceAll(srcSource, "[{FN}]", classDef.className);
    replaceAll(srcSource, "`````", string_q(5, '\t'));
    replaceAll(srcSource, "````", string_q(4, '\t'));
    replaceAll(srcSource, "```", string_q(3, '\t'));
    replaceAll(srcSource, "``", string_q(2, '\t'));
    replaceAll(srcSource, "`", string_q(1, '\t'));
    // writeTheCode returns true or false depending on if it WOULD HAVE written the file. If 'test' is true, it doesn't
    // actually write the file
    wouldHaveWritten = writeTheCode(srcFile, srcSource, ns, true, test);
    if (wouldHaveWritten && test)
        cerr << "File '" << headerFile << "' changed but was not written because of testing." << endl;

    return true;
}

//------------------------------------------------------------------------------------------------
string_q getCaseGetCode(const string_q& fieldCaseIn) {
    if (fieldCaseIn.empty())
        return "// No fields";

    string_q caseCodeOut;
    for (char ch = '_'; ch < 'z' + 1; ch++) {
        if (contains(toLower(fieldCaseIn), "+" + string_q(1, ch))) {
            caseCodeOut += ("``case '" + string_q(1, ch) + "':\n");
            string_q fields = fieldCaseIn;
            while (!fields.empty()) {
                string_q isObj = nextTokenClear(fields, '|');
                string_q type = nextTokenClear(isObj, '+');
                string_q field12 = nextTokenClear(isObj, '-');
                string_q isPtr = nextTokenClear(isObj, '~');
                bool isPointer = str_2_Bool(isPtr);
                bool isObject = str_2_Bool(isObj);

                if (tolower(field12[0]) == ch) {
                    caseCodeOut += ("```if (fieldName % \"" + field12 + "\"");
                    if (contains(type, "Array"))
                        caseCodeOut += " || fieldName % \"" + field12 + "Cnt\"";
                    caseCodeOut += ")";
                    if (contains(type, "List") || isPointer) {
                        string_q ptrCase = PTR_GET_CASE;
                        replaceAll(ptrCase, "[{NAME}]", field12);
                        replaceAll(ptrCase, "[{TYPE}]", type);
                        caseCodeOut += ptrCase;

                    } else if (type == "bool") {
                        caseCodeOut += "\n````return bool_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "sbool") {
                        caseCodeOut += "\n````return bool_2_Str_t([{PTR}]" + field12 + ");";

                    } else if (type == "bloom") {
                        caseCodeOut += "\n````return bloom_2_Bytes([{PTR}]" + field12 + ");";

                    } else if (type == "wei") {
                        caseCodeOut += "\n````return wei_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "gas") {
                        caseCodeOut += "\n````return gas_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "timestamp") {
                        caseCodeOut += "\n````return ts_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "addr" || type == "address") {
                        caseCodeOut += "\n````return addr_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "hash") {
                        caseCodeOut += "\n````return hash_2_Str([{PTR}]" + field12 + ");";

                    } else if (startsWith(type, "bytes")) {
                        caseCodeOut += "\n````return [{PTR}]" + field12 + ";";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32" || type == "uint64") {
                        caseCodeOut += "\n````return uint_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "blknum") {
                        caseCodeOut += "\n````return uint_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "uint256") {
                        caseCodeOut += "\n````return bnu_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "int8" || type == "int16" || type == "int32" || type == "int64") {
                        caseCodeOut += "\n````return int_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "int256") {
                        caseCodeOut += "\n````return bni_2_Str([{PTR}]" + field12 + ");";

                    } else if (type == "double") {
                        caseCodeOut += "\n````return double_2_Str([{PTR}]" + field12 + ", 5);";

                    } else if (endsWith(type, "_e")) {
                        caseCodeOut += "\n````return uint_2_Str(" + field12 + ");";

                    } else if (contains(type, "CStringArray") || contains(type, "CAddressArray")) {
                        string_q str = STR_CASE_CODE_STRINGARRAY;
                        replaceAll(str, "[{FIELD}]", field12);
                        caseCodeOut += str;

                    } else if (contains(type, "CBigUintArray") || contains(type, "CTopicArray")) {
                        string_q str = STR_CASE_CODE_STRINGARRAY;
                        // hack for size clause
                        replace(str, "[{FIELD}]", field12);
                        // hack for the array access
                        replace(str, "[{FIELD}][i]", "topic_2_Str(" + field12 + "[i])");
                        caseCodeOut += str;

                    } else if (contains(type, "Array")) {
                        string_q str = STR_CASE_CODE_ARRAY;
                        if (contains(type, "CBlockNum"))
                            replaceAll(str, "[{PTR}][{FIELD}][i].Format()", "uint_2_Str([{PTR}][{FIELD}][i])");
                        replaceAll(str, "[{FIELD}]", field12);
                        caseCodeOut += str;

                    } else if (isObject) {
                        caseCodeOut += " {\n````if (" + field12 + " == " + type + "())\n`````return \"\";\n````";
                        caseCodeOut += "expContext().noFrst = true;\n````return [{PTR}]" + field12 + ".Format();\n```}";

                    } else {
                        caseCodeOut += "\n````return [{PTR}]" + field12 + ";";
                    }

                    caseCodeOut += "\n";
                }
            }
            caseCodeOut += ("```break;\n");
        }
    }
    caseCodeOut = "// Return field values\n`switch (tolower(fieldName[0])) {\n" + caseCodeOut + "`}\n";
    replaceAll(caseCodeOut, "[{PTR}]", "");
    return caseCodeOut;
}

//------------------------------------------------------------------------------------------------
string_q getCaseSetCode(const string_q& fieldCaseIn) {
    string_q caseCodeOut;
    for (char ch = '_'; ch < 'z' + 1; ch++) {
        if (contains(toLower(fieldCaseIn), "+" + string_q(1, ch))) {
            caseCodeOut += ("``case '" + string_q(1, ch) + "':\n");
            string_q fields = fieldCaseIn;
            while (!fields.empty()) {
                string_q isObj = nextTokenClear(fields, '|');
                string_q type = nextTokenClear(isObj, '+');
                string_q field12 = nextTokenClear(isObj, '-');
                string_q isPtr = nextTokenClear(isObj, '~');
                bool isPointer = str_2_Bool(isPtr);
                bool isObject = str_2_Bool(isObj);

                if (tolower(field12[0]) == ch) {
                    caseCodeOut += ("```if (fieldName % \"" + field12 + "\")");
                    if (contains(type, "List") || isPointer) {
                        string_q ptrCase = PTR_SET_CASE;
                        replaceAll(ptrCase, "[{NAME}]", field12);
                        replaceAll(ptrCase, "[{TYPE}]", type);
                        caseCodeOut += ptrCase;

                    } else if (type == "bool") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Bool(fieldValue);\n````return true;\n```}";

                    } else if (type == "sbool") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Bool(fieldValue);\n````return true;\n```}";

                    } else if (type == "bloom") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Bloom(fieldValue);\n````return true;\n```}";

                    } else if (type == "wei") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Wei(fieldValue);\n````return true;\n```}";

                    } else if (type == "gas") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Gas(fieldValue);\n````return true;\n```}";

                    } else if (type == "timestamp") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Ts(fieldValue);\n````return true;\n```}";

                    } else if (type == "addr" || type == "address") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Addr(fieldValue);\n````return true;\n```}";

                    } else if (type == "hash") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Hash(fieldValue);\n````return true;\n```}";

                    } else if (startsWith(type, "bytes")) {
                        caseCodeOut += " {\n````" + field12 + " = toLower(fieldValue);\n````return true;\n```}";

                    } else if (type == "int8" || type == "int16" || type == "int32") {
                        caseCodeOut +=
                            " {\n````" + field12 + " = (int32_t)str_2_Uint(fieldValue);\n````return true;\n```}";

                    } else if (type == "int64") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Int(fieldValue);\n````return true;\n```}";

                    } else if (type == "int256") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Wei(fieldValue);\n````return true;\n```}";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32") {
                        caseCodeOut +=
                            " {\n````" + field12 + " = (uint32_t)str_2_Uint(fieldValue);\n````return true;\n```}";

                    } else if (type == "uint64") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Uint(fieldValue);\n````return true;\n```}";

                    } else if (type == "uint256") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Wei(fieldValue);\n````return true;\n```}";

                    } else if (type == "blknum") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Uint(fieldValue);\n````return true;\n```}";

                    } else if (type == "double") {
                        caseCodeOut += " {\n````" + field12 + " = str_2_Double(fieldValue);\n````return true;\n```}";

                    } else if (endsWith(type, "_e")) {
                        caseCodeOut +=
                            " {\n````" + field12 + " = str_2_Enum(" + type + ", fieldValue);\n````return true;\n```}";

                    } else if (contains(type, "CStringArray") || contains(type, "CBlockNumArray")) {
                        string_q str = STR_ARRAY_SET;
                        replaceAll(str, "[{NAME}]", field12);
                        if (contains(type, "CBlockNumArray"))
                            replaceAll(str, "nextTokenClear(str, ',')", "str_2_Uint(nextTokenClear(str, ','))");
                        caseCodeOut += str;

                    } else if (contains(type, "CAddressArray") || contains(type, "CBigUintArray") ||
                               contains(type, "CTopicArray")) {
                        string_q str = STR_ARRAY_SET;
                        replaceAll(str, "[{NAME}]", field12);
                        replaceAll(str, "nextTokenClear(str, ',')", "str_2_[{TYPE}](nextTokenClear(str, ','))");
                        replaceAll(str, "[{TYPE}]",
                                   substitute(substitute(extract(type, 1), "Array", ""), "Address", "Addr"));
                        caseCodeOut += str;

                    } else if (contains(type, "Array")) {
                        string_q str = STR_CASE_SET_CODE_ARRAY;
                        replaceAll(str, "[{NAME}]", field12);
                        replaceAll(str, "[{TYPE}]", substitute(type, "Array", ""));
                        caseCodeOut += str;

                    } else if (isObject) {
                        caseCodeOut += " {\n````return " + field12 + ".parseJson3(fieldValue);\n```}";

                    } else {
                        caseCodeOut += " {\n````" + field12 + " = fieldValue;\n````return true;\n```}";
                    }

                    caseCodeOut += "\n";
                }
            }
            caseCodeOut += ("```break;\n");
        }
    }

    return caseCodeOut + "``default:\n```break;\n";
}

//------------------------------------------------------------------------------------------------------------
string_q convertTypes(const string_q& inStr) {
    // Note: Watch out for trailing spaces. They are here to make sure it
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
    replaceAll(outStr, "bool ", "bool ");
    replaceAll(outStr, "sbool ", "bool ");

    replaceAll(outStr, "hash ", "hash_t ");
    replaceAll(outStr, "bloom ", "bloom_t ");
    replaceAll(outStr, "gas ", "gas_t ");
    replaceAll(outStr, "wei ", "wei_t ");

    replaceAll(outStr, "int8 ", "int32_t ");
    replaceAll(outStr, "int16 ", "int32_t ");
    replaceAll(outStr, "int32 ", "int32_t ");
    replaceAll(outStr, "int64 ", "int64_t ");
    replaceAll(outStr, "int256 ", "bigint_t ");
    replaceAll(outStr, "ubigint_t", "biguint_t");

    return outStr;
}

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_SET_CODE_ARRAY =
    " {\n"
    "````[{TYPE}] item;\n"
    "````string_q str = fieldValue;\n"
    "````while (item.parseJson3(str)) {\n"
    "`````[{NAME}].push_back(item);\n"
    "`````item = [{TYPE}]();  /"
    "/ reset\n"
    "````}\n"
    "````return true;\n"
    "```}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_ARRAY =
    " {\n"
    "````size_t cnt = [{PTR}][{FIELD}].size();\n"
    "````if (endsWith(toLower(fieldName), \"cnt\"))\n"
    "`````return uint_2_Str(cnt);\n"
    "````if (!cnt)\n"
    "`````return \"\";\n"
    "````string_q retS;\n"
    "````for (size_t i = 0; i < cnt; i++) {\n"
    "`````retS += [{PTR}][{FIELD}][i].Format();\n"
    "`````retS += ((i < cnt - 1) ? \",\\n\" : \"\\n\");\n"
    "````}\n"
    "````return retS;\n"
    "```}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_STRINGARRAY =
    " {\n"
    "````size_t cnt = [{PTR}][{FIELD}].size();\n"
    "````if (endsWith(toLower(fieldName), \"cnt\"))\n"
    "`````return uint_2_Str(cnt);\n"
    "````if (!cnt)\n"
    "`````return \"\";\n"
    "````string_q retS;\n"
    "````for (size_t i = 0; i < cnt; i++) {\n"
    "`````retS += (\"\\\"\" + [{PTR}][{FIELD}][i] + \"\\\"\");\n"
    "`````retS += ((i < cnt - 1) ? \",\\n\" + indent() : \"\\n\");\n"
    "````}\n"
    "````return retS;\n"
    "```}";

//------------------------------------------------------------------------------------------------------------
const char* STR_COMMENT_LINE = "//---------------------------------------------------------------------------\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_OPERATOR_DECL =
    "[{COMMENT_LINE}]"
    "extern CArchive& operator<<(CArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]);\n"
    "extern CArchive& operator>>(CArchive& archive, [{CLASS_NAME}]& [{SHORT3}]);\n"
    "\n";

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
const char* PTR_GET_CASE =
    " {\n"
    "````if ([{NAME}])\n"
    "`````return [{NAME}]->Format();\n"
    "````return \"\";\n"
    "```}";

//------------------------------------------------------------------------------------------------------------
const char* PTR_SET_CASE =
    " {\n"
    "````clear();\n"
    "````[{NAME}] = new [{TYPE}];\n"
    "````if ([{NAME}]) {\n"
    "`````string_q str = fieldValue;\n"
    "`````return [{NAME}]->parseJson3(str);\n"
    "````}\n"
    "````return false;\n"
    "```}";

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
    "`if (fieldName % \"[{FIELD}]\" && index < [{FIELD}].size())\n"
    "``return &[{FIELD}][index];\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_GETOBJ_CODE =
    "//---------------------------------------------------------------------------\n"
    "const CBaseNode* [{CLASS_NAME}]::getObjectAt(const string_q& fieldName, size_t index) const {\n"
    "[{FIELDS}]`return NULL;\n"
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
    "`s = toUpper(string_q(\"[{NAME}]\")) + \"::\";\n"
    "`if (contains(fieldName, s)) {\n"
    "``string_q f = fieldName;\n"
    "``replaceAll(f, s, \"\");\n"
    "``f = [{NAME}].getValueByName(f);\n"
    "``return f;\n"
    "`}\n";

//------------------------------------------------------------------------------------------------
const char* STR_ARRAY_SET =
    " {\n"
    "````string_q str = fieldValue;\n"
    "````while (!str.empty()) {\n"
    "`````[{NAME}].push_back(nextTokenClear(str, ','));\n"
    "````}\n"
    "````return true;\n"
    "```}";

//------------------------------------------------------------------------------------------------------------
const char* STR_READFMT = "`archive >> [{NAME}];\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_WRITEFMT = "`archive << [{NAME}];\n";
//------------------------------------------------------------------------------------------------------------
