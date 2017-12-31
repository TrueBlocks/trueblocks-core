/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
extern void     generateCode  (const COptions& options, CToml& toml, const SFString& dataFile, const SFString& ns);
extern SFString getCaseCode   (const SFString& fieldCase, const SFString& ex);
extern SFString getCaseSetCode(const SFString& fieldCase);
extern SFString short3        (const SFString& in);
extern SFString checkType     (const SFString& typeIn);
extern SFString convertTypes  (const SFString& inStr);

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    CParameter::registerClass();

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.isList)
            cout << "Classes found in the classDefinitions folder:\n";

        while (!options.classNames.empty()) {

            SFString className = nextTokenClear(options.classNames, '|');
            SFString fileName = "./classDefinitions/" + toLower(className) + ".txt";
            if (!fileExists(fileName)) {
                return usage("No class definition file found at " + fileName + "\n");

            } else {
                CToml toml(fileName);
                toml.readFile(fileName);

                if (options.isList) {
                    if (verbose) {
                        cout << SFString('-', 80) << "\nFile (dest): " << fileName << "\n";
                        cout << toml << "\n";

                    } else if (!toml.getConfigBool("settings", "disabled", false)) {
                        cout << "\t" << toml.getConfigStr("settings", "class", "") << "\n";
                    }

                } else if (options.isEdit) {
                    editFile(fileName);

                } else if (options.isRemove) {
                    if (isTestMode()) {
                        cout << "Are you sure you want to remove " << className
                                << ".cpp and " << className << ".h? (y=remove files, otherwise ignore): ";
                        cout << "Testing, but would have deleted " << className << ".[ch]*\n";
                    } else {
                        // This does not work on purpose
                        cout << "Are you sure you want to remove " << className
                                << ".cpp and " << className << ".h? (y=remove files, otherwise ignore): ";
                        return usage("Files not removed. Quitting...");
                    }
                } else {
                    if (isTestMode())
                        cout << "Would run class definition file: " << className << " (not run, testing)\n";
                    else if (verbose)
                        cerr << "Running class definition file '" << className << "'\n";

                    if (!isTestMode()) {
                        if (toml.getConfigBool("settings", "disabled", false)) {
                            if (verbose)
                                cerr << "Disabled class not processed " << className << "\n";
                        } else {
                            generateCode(options, toml, fileName, options.namesp);
                        }
                    }
                }
            }
        }
    }

    return 0;
}

//------------------------------------------------------------------------------------------------------------
SFString convertTypes(const SFString& inStr) {

    // Note: Watch out for trailing spaces. They are here to make sure it
    // matches only the types and not the field names.
    SFString outStr = inStr
        .Substitute("address ",   "SFAddress "  )
        .Substitute("bytes32 ",   "SFString "   )
        .Substitute("bytes ",     "SFString "   )
        .Substitute("bloom ",     "SFBloom "    )
        .Substitute("wei ",       "SFWei "      )
        .Substitute("gas ",       "SFGas "      )
        .Substitute("hash ",      "SFHash "     )
        .Substitute("string ",    "SFString "   )
        .Substitute("time ",      "SFTime "     )
        .Substitute("uint256 ",   "SFUintBN "   )
        .Substitute("int256 ",    "SFIntBN "    )
        .Substitute("blknum ",    "blknum_t "   )
        .Substitute("timestamp ", "timestamp_t ")
        .Substitute("bbool ",     "bool "       )
        .Substitute("bool ",      "bool "       )
        .Substitute("uint8 ",     "32uint "     )
        .Substitute("uint16 ",    "32uint "     )
        .Substitute("uint32 ",    "32uint "     )
        .Substitute("uint64 ",    "64uint "     )
        .Substitute("int8 ",      "int32_t "    )
        .Substitute("int16 ",     "int32_t "    )
        .Substitute("int32 ",     "int32_t "    )
        .Substitute("int64 ",     "int64_t "    )
        .Substitute("32uint ",    "uint32_t "   )
        .Substitute("64uint ",    "uint64_t "   );

    if (SFString(getenv("TRACING")) == "true")
        cerr << "\tconvert: " << padRight(inStr, 30) << " ==> " << outStr << "\n";

    return outStr;
}

//------------------------------------------------------------------------------------------------------------
extern const char* STR_COMMENT_LINE;
extern const char* STR_CLASSFILE;
extern const char* STR_CASE_CODE_ARRAY;
extern const char* STR_CASE_SET_CODE_ARRAY;
extern const char* STR_CASE_CODE_STRINGARRAY;
extern const char* STR_OPERATOR_H;
extern const char* STR_OPERATOR_C;
extern const char* STR_SUBCLASS;
extern const char* PTR_SET_CASE;
extern const char* PTR_GET_CASE;
extern const char* STR_GETVALUE1;
extern const char* STR_GETVALUE2;
extern const char* STR_GETOBJ_CODE;
extern const char* STR_GETOBJ_CODE_FIELD;
extern const char* STR_GETSTR_CODE;
extern const char* STR_GETSTR_CODE_FIELD;
extern const char* STR_GETOBJ_HEAD;
extern const char* STR_GETSTR_HEAD;
extern const char* STR_UPGRADE_CODE;

SFString tab = SFString("\t");

//------------------------------------------------------------------------------------------------------------
void generateCode(const COptions& options, CToml& toml, const SFString& dataFile, const SFString& ns) {

    //------------------------------------------------------------------------------------------------
    SFString className  = toml.getConfigStr ("settings", "class", "");
    SFString baseClass  = toml.getConfigStr ("settings", "baseClass", "CBaseNode");
    SFString otherIncs  = toml.getConfigStr ("settings", "cIncs", "").Substitute("|", "\n");
    SFString scope      = toml.getConfigStr ("settings", "scope", "static");
    SFString hIncludes  = toml.getConfigStr ("settings", "includes", "");
    bool     serialize  = toml.getConfigBool("settings", "serialize", false);

    //------------------------------------------------------------------------------------------------
    SFString baseBase   = toProper(baseClass.substr(1));
    SFString baseName   = className.substr(1);
    SFString baseProper = toProper(baseName);
    SFString baseLower  = toLower(baseName);
    SFString baseUpper  = toUpper(baseName);

    //------------------------------------------------------------------------------------------------
    SFString fieldDec, fieldSet,  fieldClear,  fieldCopy, fieldGetObj, fieldGetStr, fieldArchiveRead;
    SFString fieldArchiveWrite,   fieldReg,    fieldCase, fieldSubCls, headerIncs;

    //------------------------------------------------------------------------------------------------
    while (!hIncludes.empty()) {
        SFString line = nextTokenClear(hIncludes, '|');
        if (line != "none")
            headerIncs += "#include \"" + line + "\"\n";
    }

    //------------------------------------------------------------------------------------------------
    bool isBase = (baseClass == "CBaseNode");
    SFString parFnc = isBase ?
                        "readBackLevel" :
                        "readBackLevel";
    SFString parSer = isBase ?
                        "\t[{BASE_CLASS}]::Serialize(archive);\n" :
                        "\t[{BASE_CLASS}]::Serialize(archive);\n\n";
    SFString parReg = isBase ?
                        "" :
                        "[{BASE_CLASS}]::registerClass();\n\n\t";
    SFString parCnk = isBase ?
                        "ret = next[{BASE_BASE}]Chunk(fieldName, this);\n" :
                        "ret = next[{BASE_BASE}]Chunk(fieldName, this);\n";
    SFString parSet = isBase ?
                        "" :
                        "\tif ([{BASE_CLASS}]::setValueByName(fieldName, fieldValue))\n\t\treturn true;\n\n";

    //------------------------------------------------------------------------------------------------
    // build the field list from the config file string
    SFString fields = toml.getConfigStr("settings", "fields", "").Substitute("address[]", "SFAddressArray");
    CParameterList fieldList;
    while (!fields.empty()) {
        SFString fieldDef = nextTokenClear(fields, '|');
        CParameter *f = new CParameter(fieldDef);
        fieldList.AddTail(f);
    }

    //------------------------------------------------------------------------------------------------
    LISTPOS lPos = fieldList.GetHeadPosition();
    while (lPos) {
        CParameter *fld = fieldList.GetNext(lPos);

        SFString decFmt  = "\t[{TYPE}] *[{NAME}];";
        if (!fld->isPointer) {
            decFmt.Replace("*", "");
        }
        SFString copyFmt = "\t[{NAME}] = +SHORT+.[{NAME}];\n";
        if (fld->isPointer)
            copyFmt = "\tif ([+SHORT+.{NAME}]) {\n\t\t[{NAME}] = new [{TYPE}];\n"
                        "\t\t*[{NAME}] = *[+SHORT+.{NAME}];\n\t}\n";
        SFString badSet   = "//\t[{NAME}] = ??; /""* unknown type: [{TYPE}] */\n";
        SFString setFmt   = "\t[{NAME}]";
        SFString regFmt   = "\tADD_FIELD(CL_NM, \"[{NAME}]\", T_TEXT, ++fieldNum);\n", regType;
        SFString clearFmt = "\tif ([{NAME}])\n\t\tdelete [{NAME}];\n\t[{NAME}] = NULL;\n";
        SFString subClsFmt = STR_SUBCLASS;

               if (fld->type == "bloom")     { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_BLOOM";
        } else if (fld->type == "wei")       { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_WEI";
        } else if (fld->type == "gas")       { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_GAS";
        } else if (fld->type == "timestamp") { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_TIMESTAMP";
        } else if (fld->type == "blknum")    { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "string")    { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "addr")      { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_ADDRESS";
        } else if (fld->type == "address")   { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_ADDRESS";
        } else if (fld->type == "hash")      { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_HASH";
        } else if (fld->type == "bytes32")   { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "bytes")     { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "int8")      { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "int16")     { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "int32")     { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "int64")     { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "int256")    { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "uint8")     { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "uint16")    { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "uint32")    { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "uint64")    { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "uint256")   { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_NUMBER";
        } else if (fld->type == "bbool")     { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_BOOL";
        } else if (fld->type == "bool")      { setFmt = "\t[{NAME}] = [{DEF}];\n";  regType = "T_BOOL";
        } else if (fld->type == "double")    { setFmt = "\t[{NAME}] = [{DEFF}];\n"; regType = "T_DOUBLE";
        } else if (fld->type == "time")      { setFmt = "\t[{NAME}] = [{DEFT}];\n"; regType = "T_DATE";
        } else if (fld->isPointer)           { setFmt = "\t[{NAME}] = [{DEFP}];\n"; regType = "T_POINTER";
        } else if (fld->isObject)            { setFmt = "\t[{NAME}].Init();\n";     regType = "T_OBJECT";
        } else                               { setFmt = badSet;                     regType = "T_TEXT"; }

        if (fld->type.Contains("Array")) {
            setFmt = "\t[{NAME}].Clear();\n";
            if (fld->type.Contains("Address")) {
                regType = "T_ADDRESS|TS_ARRAY";
            } else if (fld->type.Contains("String")) {
                regType = "T_TEXT|TS_ARRAY";
            } else {
                regType = "T_OBJECT|TS_ARRAY";
            }
        }
#define getDefault(a) (fld->strDefault.empty() ? (a) : fld->strDefault )
        setFmt.Replace("[{DEFS}]", getDefault("\"\""));
        setFmt.Replace("[{DEF}]",  getDefault("0"));
        setFmt.Replace("[{DEFF}]", getDefault("0.0"));
        setFmt.Replace("[{DEFT}]", getDefault("earliestDate"));
        setFmt.Replace("[{DEFP}]", getDefault("NULL"));

        if (fld->type.Contains("Array") || (fld->isObject && !fld->isPointer)) {

            if (fld->type.Contains("SFStringArray")  ||
                fld->type.Contains("SFBlockArray")   ||
                fld->type.Contains("SFAddressArray") ||
                fld->type.Contains("SFBigUintArray") ||
                fld->type.Contains("SFTopicArray")) {

                fieldGetStr += STR_GETSTR_CODE_FIELD;
                fieldGetStr.ReplaceAll("[{FIELD}]", fld->name);
                if (fld->name == "topics") {
                    fieldGetStr.ReplaceAll("THING", "fromTopic");
                } else if (fld->type.Contains("SFBlockArray")) {
                    fieldGetStr.ReplaceAll("THING", "asStringU");
                } else {
                    fieldGetStr.ReplaceAll("THING", "");
                }
            } else {
                fieldGetObj += STR_GETOBJ_CODE_FIELD;
                if (!fld->type.Contains("Array")) {
                    fieldGetObj.Replace(" && index < [{FIELD}].getCount()", "");
                    fieldGetObj.Replace("[index]", "");
                }
                fieldGetObj.ReplaceAll("[{FIELD}]", fld->name);
            }
        }

        fieldReg   += fld->Format(regFmt).Substitute("T_TEXT", regType); fieldReg.ReplaceAll("CL_NM", "[{CLASS_NAME}]");
        fieldCase  += fld->Format("[{TYPE}]+[{NAME}]-[{ISPOINTER}]~[{ISOBJECT}]|");
        fieldDec   += (convertTypes(fld->Format(decFmt)) + "\n");
        fieldCopy  += fld->Format(copyFmt).Substitute("+SHORT+", "[{SHORT}]").Substitute("++CLASS++", "[{CLASS_NAME}]");
        fieldSet   += fld->Format(setFmt);
        fieldClear += (fld->isPointer ? fld->Format(clearFmt) : "");
        if (fld->isObject && !fld->isPointer && !fld->type.Contains("Array")) {
            SFString fmt = subClsFmt;
            fmt.ReplaceAll("[FNAME]", fld->name);
            fmt.ReplaceAll("[SH3]", short3(baseLower));
            SFString fldStr = fld->Format(fmt);
            fldStr.Replace("++", "[{");
            fldStr.Replace("++", "}]");
            if (fieldSubCls.empty())
                fieldSubCls = "\n\tSFString s;\n";
            fieldSubCls += fldStr;
        }

SFString ptrReadFmt =
"    [{NAME}] = NULL;\n"
"    bool has_[{NAME}] = false;\n"
"    archive >> has_[{NAME}];\n"
"    if (has_[{NAME}]) {\n"
"        [{NAME}] = new [{TYPE}];\n"
"        if (![{NAME}])\n"
"            return false;\n"
"        [{NAME}]->Serialize(archive);\n"
"    }\n";

SFString ptrWriteFmt =
"    archive << ([{NAME}] != NULL);\n"
"    if ([{NAME}])\n"
"        [{NAME}]->SerializeC(archive);\n";

        fieldArchiveRead  += fld->Format(fld->isPointer ? ptrReadFmt  : "\tarchive >> [{NAME}];\n");
        fieldArchiveWrite += fld->Format(fld->isPointer ? ptrWriteFmt : "\tarchive << [{NAME}];\n");
    }

    //------------------------------------------------------------------------------------------------
    bool hasObjGetter = !fieldGetObj.empty();
    if (hasObjGetter)
        fieldGetObj = SFString(STR_GETOBJ_CODE).Substitute("[{FIELDS}]", fieldGetObj);
    bool hasStrGetter = !fieldGetStr.empty();
    if (hasStrGetter)
        fieldGetStr = SFString(STR_GETSTR_CODE).Substitute("[{FIELDS}]", fieldGetStr);

    //------------------------------------------------------------------------------------------------
    SFString operatorH = SFString(serialize ? STR_OPERATOR_H : "");
    SFString operatorC = SFString(serialize ? STR_OPERATOR_C : "\n");

    //------------------------------------------------------------------------------------------------
    SFString caseSetCodeStr = getCaseSetCode(fieldCase);
    SFString subClsCodeStr  = fieldSubCls;

    //------------------------------------------------------------------------------------------------
    SFString sorts[4] = { baseLower.substr(0,2)+"_Name", "", baseLower+"ID", "" };
    SFString sortString = toml.getConfigStr("settings", "sort", "");
    uint32_t cnt = 0;
    while (!sortString.empty())
        sorts[cnt++] = nextTokenClear(sortString, '|');

    //------------------------------------------------------------------------------------------------
    SFString headerFile = dataFile.Substitute(".txt", ".h").Substitute("./classDefinitions/", "./");
    SFString headSource = asciiFileToString(configPath("makeClass/blank.h"));
    headSource.ReplaceAll("[{GET_OBJ}]",      (hasObjGetter ? SFString(STR_GETOBJ_HEAD)+(hasStrGetter?"":"\n") : ""));
    headSource.ReplaceAll("[{GET_STR}]",      (hasStrGetter ? SFString(STR_GETSTR_HEAD)+"\n" : ""));
    headSource.ReplaceAll("[FIELD_COPY]",     fieldCopy);
    headSource.ReplaceAll("[FIELD_DEC]",      fieldDec);
    headSource.ReplaceAll("[FIELD_SET]",      fieldSet);
    headSource.ReplaceAll("[FIELD_CLEAR]",    fieldClear);
    headSource.ReplaceAll("[H_INCLUDES]",     headerIncs);
    headSource.ReplaceAll("[{OPERATORS}]",    operatorH);
    headSource.ReplaceAll("[{BASE_CLASS}]",   baseClass);
    headSource.ReplaceAll("[{BASE_BASE}]",    baseBase);
    headSource.ReplaceAll("[{BASE}]",         baseUpper);
    headSource.ReplaceAll("[{CLASS_NAME}]",   className);
    headSource.ReplaceAll("[{COMMENT_LINE}]", STR_COMMENT_LINE);
    headSource.ReplaceAll("[{LONG}]",         baseLower);
    headSource.ReplaceAll("[{PROPER}]",       baseProper);
    headSource.ReplaceAll("[{SHORT}]",        baseLower.substr(0,2));
    headSource.ReplaceAll("[{BASE_CLASS}]",   baseClass);
    headSource.ReplaceAll("[{BASE_BASE}]",    baseBase);
    headSource.ReplaceAll("[{BASE}]",         baseUpper);
    headSource.ReplaceAll("[{CLASS_NAME}]",   className);
    headSource.ReplaceAll("[{COMMENT_LINE}]", STR_COMMENT_LINE);
    headSource.ReplaceAll("[{LONG}]",         baseLower);
    headSource.ReplaceAll("[{PROPER}]",       baseProper);
    headSource.ReplaceAll("[{SHORT3}]",       short3(baseLower));
    headSource.ReplaceAll("[{SHORT}]",        baseLower.substr(0,2));
    headSource.ReplaceAll("[{SCOPE}]",        scope);
    headSource.ReplaceAll("[{NAMESPACE1}]",   (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    headSource.ReplaceAll("[{NAMESPACE2}]",   (ns.empty() ? "" : "}  // namespace qblocks\n"));
    if (options.writeHeader)
        writeTheCode(headerFile, headSource, ns);

    //------------------------------------------------------------------------------------------------
    SFString fieldStr = fieldList.GetCount() ? getCaseCode(fieldCase, "").Substitute("[{PTR}]", "") : "// No fields";

    SFString srcFile    = dataFile.Substitute(".txt", ".cpp").Substitute("./classDefinitions/", "./");
    SFString srcSource  = asciiFileToString(configPath("makeClass/blank.cpp"));
    if ((className.startsWith("CNew") || className == "CPriceQuote") && !getCWD().Contains("parse"))
        srcSource.Replace("version of the data\n", STR_UPGRADE_CODE);
    srcSource.ReplaceAll("[{GET_OBJ}]",         fieldGetObj);
    srcSource.ReplaceAll("[{GET_STR}]",         fieldGetStr);
    srcSource.ReplaceAll("[ARCHIVE_READ]",      fieldArchiveRead);
    srcSource.ReplaceAll("[ARCHIVE_WRITE]",     fieldArchiveWrite);
    srcSource.ReplaceAll("[{OPERATORS}]",       operatorC);
    srcSource.ReplaceAll("[REGISTER_FIELDS]",   fieldReg);
    srcSource.ReplaceAll("[{FIELD_CASE}]",      fieldStr);
    srcSource.ReplaceAll("[OTHER_INCS]",        otherIncs);
    srcSource.ReplaceAll("[FIELD_SETCASE]",     caseSetCodeStr);
    srcSource.ReplaceAll("[{SUBCLASSFLDS}]",    subClsCodeStr);
    srcSource.ReplaceAll("[{PARENT_SER1}]",     parSer);
    srcSource.ReplaceAll("[{PARENT_SER2}]",     parSer.Substitute("Serialize", "SerializeC"));
    srcSource.ReplaceAll("[{PARENT_REG}]",      parReg);
    srcSource.ReplaceAll("[{PARENT_CHNK}]\n",   parCnk);
    srcSource.ReplaceAll("[{PARENT_SET}]\n",    parSet);
    srcSource.ReplaceAll("[{PAR_READ_HEAD}]",   parFnc);
    srcSource.ReplaceAll("[{BASE_CLASS}]",      baseClass);
    srcSource.ReplaceAll("[{BASE_BASE}]",       baseBase);
    srcSource.ReplaceAll("[{BASE}]",            baseUpper);
    srcSource.ReplaceAll("[{CLASS_NAME}]",      className);
    srcSource.ReplaceAll("[{COMMENT_LINE}]",    STR_COMMENT_LINE);
    srcSource.ReplaceAll("[{LONG}]",            baseLower);
    srcSource.ReplaceAll("[{PROPER}]",          baseProper);
    srcSource.ReplaceAll("[{SHORT}]",           baseLower.substr(0,2));
    srcSource.ReplaceAll("[{NAME_SORT1}]",      sorts[0]);
    srcSource.ReplaceAll("[{NAME_SORT2}]",      sorts[1]);
    srcSource.ReplaceAll("[{ID_SORT1}]",        sorts[2]);
    srcSource.ReplaceAll("[{ID_SORT2}]",        sorts[3]);
    srcSource.ReplaceAll("[{BASE_CLASS}]",      baseClass);
    srcSource.ReplaceAll("[{BASE_BASE}]",       baseBase);
    srcSource.ReplaceAll("[{BASE}]",            baseUpper);
    srcSource.ReplaceAll("[{CLASS_NAME}]",      className);
    srcSource.ReplaceAll("[{COMMENT_LINE}]",    STR_COMMENT_LINE);
    srcSource.ReplaceAll("[{LONG}]",            baseLower);
    srcSource.ReplaceAll("[{PROPER}]",          baseProper);
    srcSource.ReplaceAll("[{SHORT3}]",          short3(baseLower));
    srcSource.ReplaceAll("[{SHORT}]",           baseLower.substr(0,2));
    srcSource.ReplaceAll("[{SCOPE}]",           scope);
    srcSource.ReplaceAll("[{NAMESPACE1}]",      (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    srcSource.ReplaceAll("[{NAMESPACE2}]",      (ns.empty() ? "" : "}  // namespace qblocks\n"));
    if (options.writeSource)
        writeTheCode(srcFile, srcSource, ns);
}

//------------------------------------------------------------------------------------------------
SFString getCaseCode(const SFString& fieldCase, const SFString& ex) {
    SFString baseTab = (tab+tab+ex);
    SFString caseCode;
    for (char ch = '_' ; ch < 'z' + 1 ; ch++) {
        if (fieldCase.ContainsI("+"+SFString(ch))) {
            caseCode += baseTab + "case '" + ch + "':\n";
            SFString fields = fieldCase;
            while (!fields.empty()) {
                SFString isObj = nextTokenClear(fields, '|');
                SFString type  = nextTokenClear(isObj, '+');
                SFString field = nextTokenClear(isObj, '-');
                SFString isPtr = nextTokenClear(isObj, '~');
                bool     isPointer = atoi((const char*)isPtr);
                bool     isObject = atoi((const char*)isObj);

                if (tolower(field[0]) == ch) {
                    caseCode += baseTab + tab + "if ( fieldName % \"" + field + "\"";
                    if (type.Contains("Array"))
                        caseCode += " || fieldName % \"" + field + "Cnt\"";
                    caseCode += " )";
                    if (type.Contains("List") || isPointer) {
                        SFString ptrCase = PTR_GET_CASE;
                        ptrCase.ReplaceAll("[{NAME}]", field);
                        ptrCase.ReplaceAll("[{TYPE}]", type);
                        caseCode += ptrCase;

                    } else if (type == "time") {
                        caseCode += " return [{PTR}]" + field + ".Format(FMT_JSON);";

                    } else if (type == "bbool" || type == "bool") {
                        caseCode += " return asString([{PTR}]" + field + ");";

                    } else if (type == "bloom") {
                        caseCode += " return fromBloom([{PTR}]" + field + ");";

                    } else if (type == "wei") {
                        caseCode += " return fromWei([{PTR}]" + field + ");";

                    } else if (type == "gas") {
                        caseCode += " return fromGas([{PTR}]" + field + ");";

                    } else if (type == "timestamp") {
                        caseCode += " return fromTimestamp([{PTR}]" + field + ");";

                    } else if (type == "addr" || type == "address") {
                        caseCode += " return fromAddress([{PTR}]" + field + ");";

                    } else if (type == "hash") {
                        caseCode += " return fromHash([{PTR}]" + field + ");";

                    } else if (type == "bytes" || type == "bytes32") {
                        caseCode += " return [{PTR}]" + field + ";";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32" || type == "uint64") {
                        caseCode += " return asStringU([{PTR}]" + field + ");";

                    } else if (type == "blknum") {
                        caseCode += " return asStringU([{PTR}]" + field + ");";

                    } else if (type == "uint256") {
                        caseCode += " return asStringBN([{PTR}]" + field + ");";

                    } else if (type == "int8" || type == "int16" || type == "int32" || type == "int64") {
                        caseCode += " return asString([{PTR}]" + field + ");";

                    } else if (type == "int256") {
                        caseCode += " return asStringBN([{PTR}]" + field + ");";

                    } else if (type == "double") {
                        caseCode += " return fmtFloat([{PTR}]" + field + ");";

                    } else if (type.Contains("SFStringArray") || type.Contains("SFAddressArray")) {
                        SFString str = STR_CASE_CODE_STRINGARRAY;
                        str.ReplaceAll("[{FIELD}]", field);
                        caseCode += str;

                    } else if (type.Contains("SFBigUintArray") || type.Contains("SFTopicArray")) {
                        SFString str = STR_CASE_CODE_STRINGARRAY;
                        // hack for getCount clause
                        str.Replace("[{FIELD}]", field);
                        // hack for the array access
                        str.Replace("[{FIELD}][i]", "fromTopic("+field+"[i])");
                        caseCode += str;

                    } else if (type.Contains("Array")) {
                        SFString str = STR_CASE_CODE_ARRAY;
                        if (type.Contains("SFUint") || type.Contains("SFBlock"))
                            str.ReplaceAll("[{PTR}][{FIELD}][i].Format()", "asStringU([{PTR}][{FIELD}][i])");
                        str.ReplaceAll("[{FIELD}]", field);
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
    caseCode.ReplaceAll("[BTAB]", baseTab);
    caseCode = "// Return field values\n\tswitch (tolower(fieldName[0])) {\n" + caseCode + "\t}\n";
    return caseCode;
}

SFString strArraySet =
" {\n"
"\t\t\t\tSFString str = fieldValue;\n"
"\t\t\t\twhile (!str.empty()) {\n"
"\t\t\t\t\t[{NAME}][[{NAME}].getCount()] = nextTokenClear(str,',');\n"
"\t\t\t\t}\n"
"\t\t\t\treturn true;\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------
SFString getCaseSetCode(const SFString& fieldCase) {
    SFString baseTab = (tab+tab);
    SFString caseCode;
    for (char ch = '_' ; ch < 'z' + 1 ; ch++) {
        if (fieldCase.ContainsI("+"+SFString(ch))) {
            caseCode += baseTab + "case '" + ch + "':\n";
            SFString fields = fieldCase;
            while (!fields.empty()) {
                SFString isObj = nextTokenClear(fields, '|');
                SFString type  = nextTokenClear(isObj, '+');
                SFString field = nextTokenClear(isObj, '-');
                SFString isPtr = nextTokenClear(isObj, '~');
                bool     isPointer = atoi((const char*)isPtr);
                bool     isObject = atoi((const char*)isObj);

                if (tolower(field[0]) == ch) {
                    caseCode += baseTab + tab + "if ( fieldName % \"" + field + "\" )";
                    if (type.Contains("List") || isPointer) {
                        SFString ptrCase = PTR_SET_CASE;
                        ptrCase.ReplaceAll("[{NAME}]", field);
                        ptrCase.ReplaceAll("[{TYPE}]", type);
                        caseCode += ptrCase;

                    } else if (type == "time") {
                        caseCode += " { " + field + " = parseDate(fieldValue); return true; }";

                    } else if (type == "bbool" || type == "bool") {
                        caseCode +=  " { " + field + " = toBool(fieldValue); return true; }";

                    } else if (type == "bloom") {
                        caseCode +=  " { " + field + " = toBloom(fieldValue); return true; }";

                    } else if (type == "wei") {
                        caseCode +=  " { " + field + " = toWei(fieldValue); return true; }";

                    } else if (type == "gas") {
                        caseCode +=  " { " + field + " = toGas(fieldValue); return true; }";

                    } else if (type == "timestamp") {
                        caseCode +=  " { " + field + " = toTimestamp(fieldValue); return true; }";

                    } else if (type == "addr" || type == "address") {
                        caseCode += " { " + field + " = toAddress(fieldValue); return true; }";

                    } else if (type == "hash") {
                        caseCode += " { " + field + " = toHash(fieldValue); return true; }";

                    } else if (type.Contains("bytes")) {
                        caseCode += " { " + field + " = toLower(fieldValue); return true; }";

                    } else if (type == "int8" || type == "int16" || type == "int32") {
                        caseCode +=  " { " + field + " = toLong32(fieldValue); return true; }";

                    } else if (type == "int64") {
                        caseCode +=  " { " + field + " = toLong(fieldValue); return true; }";

                    } else if (type == "int256") {
                        caseCode +=  " { " + field + " = toLong(fieldValue); return true; }";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32") {
                        caseCode +=  " { " + field + " = toLong32u(fieldValue); return true; }";

                    } else if (type == "uint64") {
                        caseCode +=  " { " + field + " = toUnsigned(fieldValue); return true; }";

                    } else if (type == "uint256") {
                        caseCode +=  " { " + field + " = toWei(fieldValue); return true; }";

                    } else if (type == "blknum") {
                        caseCode +=  " { " + field + " = toUnsigned(fieldValue); return true; }";

                    } else if (type == "double") {
                        caseCode +=  " { " + field + " = toDouble(fieldValue); return true; }";

                    } else if (type.Contains("SFStringArray") || type.Contains("SFBlockArray")) {
                        SFString str = strArraySet;
                        str.ReplaceAll("[{NAME}]", field);
                        if (type.Contains("SFBlockArray"))
                            str.ReplaceAll("nextTokenClear(str,',')", "toUnsigned(nextTokenClear(str,','))");
                        caseCode += str;

                    } else if (type.Contains("SFAddressArray") || type.Contains("SFBigUintArray") || type.Contains("SFTopicArray")) {
                        SFString str = strArraySet;
                        str.ReplaceAll("[{NAME}]", field);
                        str.ReplaceAll("nextTokenClear(str,',')", "to[{TYPE}](nextTokenClear(str,','))");
                        str.ReplaceAll("[{TYPE}]", type.substr(2).Substitute("Array", ""));
                        caseCode += str;

                    } else if (type.Contains("Array")) {
                        SFString str = STR_CASE_SET_CODE_ARRAY;
                        str.ReplaceAll("[{NAME}]", field);
                        str.ReplaceAll("[{TYPE}]", type.Substitute("Array", ""));
                        caseCode += str;

                    } else if (isObject) {
                        caseCode +=  " { /* " + field + " = fieldValue; */ return false; }";

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
const char* STR_CLASSFILE =
"class:\t\t[CLASS_NAME]\n"
"fields:\t\tbool dataField1|int dataField2|string dataField3|Array dataField4\n"
"includes:\tnone\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_SET_CODE_ARRAY =
" {\n"
"\t\t\t\tchar *p = (char *)fieldValue.c_str();\n"
"\t\t\t\twhile (p && *p) {\n"
"\t\t\t\t\t[{TYPE}] item;\n"
"\t\t\t\t\tuint32_t nFields = 0;\n"
"\t\t\t\t\tp = item.parseJson(p, nFields);\n"
"\t\t\t\t\tif (nFields)\n"
"\t\t\t\t\t\t[{NAME}][[{NAME}].getCount()] = item;\n"
"\t\t\t\t}\n"
"\t\t\t\treturn true;\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_ARRAY =
" {\n"
"[BTAB]\t\tuint32_t cnt = [{PTR}][{FIELD}].getCount();\n"
"[BTAB]\t\tif (fieldName.endsWith(\"Cnt\"))\n"
"[BTAB]\t\t\treturn asStringU(cnt);\n"
"[BTAB]\t\tif (!cnt) return \"\";\n"
"[BTAB]\t\tSFString retS;\n"
"[BTAB]\t\tfor (uint32_t i = 0 ; i < cnt ; i++) {\n"
"[BTAB]\t\t\tretS += [{PTR}][{FIELD}][i].Format();\n"
"[BTAB]\t\t\tretS += ((i < cnt - 1) ? \",\\n\" : \"\\n\");\n"
"[BTAB]\t\t}\n"
"[BTAB]\t\treturn retS;\n"
"[BTAB]\t}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_STRINGARRAY =
" {\n"
"[BTAB]\t\tuint32_t cnt = [{PTR}][{FIELD}].getCount();\n"
"[BTAB]\t\tif (fieldName.endsWith(\"Cnt\"))\n"
"[BTAB]\t\t\treturn asStringU(cnt);\n"
"[BTAB]\t\tif (!cnt) return \"\";\n"
"[BTAB]\t\tSFString retS;\n"
"[BTAB]\t\tfor (uint32_t i = 0 ; i < cnt ; i++) {\n"
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
"extern SFArchive& operator<<(SFArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]);\n"
"extern SFArchive& operator>>(SFArchive& archive, [{CLASS_NAME}]& [{SHORT3}]);\n"
"\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_OPERATOR_C =
"[{COMMENT_LINE}]"
"SFArchive& operator<<(SFArchive& archive, const [{CLASS_NAME}]& [{SHORT3}]) {\n"
"\t[{SHORT3}].SerializeC(archive);\n"
"\treturn archive;\n"
"}\n"
"\n"
"[{COMMENT_LINE}]"
"SFArchive& operator>>(SFArchive& archive, [{CLASS_NAME}]& [{SHORT3}]) {\n"
"\t[{SHORT3}].Serialize(archive);\n"
"\treturn archive;\n"
"}\n"
"\n";

//------------------------------------------------------------------------------------------------------------
const char* STR_SUBCLASS =
"\ts = toUpper(SFString(\"[FNAME]\")) + \"::\";\n"
"\tif (fieldName.Contains(s)) {\n"
"\t\tSFString f = fieldName;\n"
"\t\tf.ReplaceAll(s,\"\");\n"
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
"\t\t\t\tClear();\n"
"\t\t\t\t[{NAME}] = new [{TYPE}];\n"
"\t\t\t\tif ([{NAME}]) {\n"
"\t\t\t\t\tchar *p = cleanUpJson((char *)fieldValue.c_str());\n"
"\t\t\t\t\tuint32_t nFields = 0;\n"
"\t\t\t\t\t[{NAME}]->parseJson(p, nFields);\n"
"\t\t\t\t\treturn true;\n"
"\t\t\t\t}\n"
"\t\t\t\treturn false;\n"
"\t\t\t}";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETOBJ_HEAD =
"\tconst CBaseNode *getObjectAt(const SFString& fieldName, uint32_t index) const override;\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETOBJ_CODE_FIELD =
"\tif ( fieldName % \"[{FIELD}]\" && index < [{FIELD}].getCount() )\n"
"\t\treturn &[{FIELD}][index];\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETOBJ_CODE =
"//---------------------------------------------------------------------------\n"
"const CBaseNode *[{CLASS_NAME}]::getObjectAt(const SFString& fieldName, uint32_t index) const {\n"
"[{FIELDS}]\treturn NULL;\n"
"}\n\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETSTR_HEAD =
"\tconst SFString getStringAt(const SFString& name, uint32_t i) const override;\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETSTR_CODE_FIELD =
"\tif ( name % \"[{FIELD}]\" && i < [{FIELD}].getCount() )\n"
"\t\treturn THING([{FIELD}][i]);\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_GETSTR_CODE =
"//---------------------------------------------------------------------------\n"
"const SFString [{CLASS_NAME}]::getStringAt(const SFString& name, uint32_t i) const {\n"
"[{FIELDS}]\treturn \"\";\n"
"}\n\n";

//------------------------------------------------------------------------------------------------------------
const char *STR_UPGRADE_CODE =
"version of the data\n\t(([{CLASS_NAME}]*)this)->m_schema = getVersionNum();\n";

//------------------------------------------------------------------------------------------------------------
SFString short3(const SFString& str) {
    SFString ret = str.substr(0,3);
    if (ret == "new")
        ret = "newp";
    if (ret == "ret")
        ret = "retp";
    return ret;
}

//---------------------------------------------------------------------------
SFString checkType(const SFString& typeIn) {
    if (typeIn.startsWith("C")) return typeIn;
    if (typeIn.endsWith("Array")) return typeIn;

    SFString keywords[] = {
        "address", "bloom",  "bool",  "bytes",     "bytes32",
        "double",  "gas",    "hash",  "int256",    "int32",
        "int64",   "string", "time",  "timestamp", "uint256",
        "uint32",  "uint64", "uint8", "wei",       "blknum",
    };
    uint32_t cnt = sizeof(keywords) / sizeof(SFString);
    for (uint32_t i=0;i<cnt;i++) {
        if (keywords[i] == typeIn)
            return typeIn;
    }
    cerr << "Invalid type: " << typeIn << ". Quitting...(hit enter)\n";
    cerr.flush();
    getchar();
    exit(0);
}
