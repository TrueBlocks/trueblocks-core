/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "abilib.h"
#include "options.h"

//------------------------------------------------------------------------------------------------------------
extern void     establishFiles(const SFString& className);
extern void     generateCode(CToml& classFile, const SFString& dataFile, const SFString& ns);
extern SFString convertTypes(const SFString& inStr);
extern SFString getCaseCode(const SFString& fieldCase);
extern SFString getCaseSetCode(const SFString& fieldCase);
extern SFString short3(const SFString& in);

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
                CToml classFile("");
                classFile.readFile(fileName);

                if (options.isList) {
                    if (verbose) {
                        cout << SFString('-', 80) << "\nFile (dest): " << fileName << "\n";
                        cout << classFile << "\n";

                    } else if (!classFile.getConfigBool("settings", "disabled", false)) {
                        cout << "\t" << classFile.getConfigStr("settings","class","") << "\n";
                    }

                } else if (options.isEdit) {
                    SFString cmd = "open \"" + fileName + "\"";
                    if (SFString(getenv("TEST_MODE")) == "true") {
                        cout << cmd << "\n";
                    } else {
                        if (system(cmd.c_str())) {}  // do not remove. Silences compiler warnings
                    }

                } else if (options.isRemove) {
                    if (SFString(getenv("TEST_MODE")) == "true") {
                        cout << "Are you sure you want to remove " << className
                                << ".cpp and " << className << ".h? (y=remove files, otherwise ignore): ";
                        cout << "Testing, but would have deleted " << className << ".[ch]*\n";
                    } else {
                        // TODO(tjayrush): Doesn't work on purpose
                        cout << "Are you sure you want to remove " << className
                                << ".cpp and " << className << ".h? (y=remove files, otherwise ignore): ";
                        return usage("Files not removed. Quitting...");
                    }
                } else {
                    if (SFString(getenv("TEST_MODE")) == "true")
                        cout << "Would run class definition file: " << className << " (not run, testing)\n";
                    else if (verbose)
                        cerr << "Running class definition file '" << className << "'\n";

                    if (SFString(getenv("TEST_MODE")) != "true") {
                        if (classFile.getConfigBool("settings", "disabled", false)) {
                            if (verbose)
                                cerr << "Disabled class not processed " << className << "\n";
                        } else {
                            generateCode(classFile, fileName, options.namesp);
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
    return inStr.Substitute("address ", "SFAddress ")
    .Substitute("bytes32 ", "SFString ")
    .Substitute("bytes ",   "SFString ")
    .Substitute("bloom ",   "SFBloom ")
    .Substitute("wei ",     "SFWei ")
    .Substitute("uint256 ", "SFUintBN ")
    .Substitute("int256 ",  "SFIntBN ")
    .Substitute("uint8 ",   "SFUxnt32 ")
    .Substitute("uint16 ",  "SFUxnt32 ")
    .Substitute("uint32 ",  "SFUxnt32 ")
    .Substitute("uint64 ",  "SFUxnt32 ")
    .Substitute("hash ",    "SFHash ")
    .Substitute("string ",  "SFString ")
    .Substitute("bbool ",   "xool ")
    .Substitute("bool ",    "bool ")
    .Substitute("time ",    "SFTime ")
    .Substitute("int8 ",    "int32_t ")
    .Substitute("int16 ",   "int32_t ")
    .Substitute("int32 ",   "int32_t ")
    .Substitute("int64 ",   "int64_t ")

    .Substitute("xool ",    "bool ")
    .Substitute("xnt32 ",   "int32 ");
}

//------------------------------------------------------------------------------------------------------------
extern const char* STR_COMMENT_LINE;
extern const char* STR_CLASSFILE;
extern const char* STR_CASE_CODE_ARRAY;
extern const char* STR_CASE_CODE_STRINGARRAY;
extern const char* STR_OPERATOR_H;
extern const char* STR_OPERATOR_C;
extern const char* STR_SUBCLASS;

SFString tab = SFString("\t");

//------------------------------------------------------------------------------------------------------------
void generateCode(CToml& classFile, const SFString& dataFile, const SFString& ns) {
    //------------------------------------------------------------------------------------------------
    SFString className  = classFile.getConfigStr("settings","class","");
    SFString baseClass  = classFile.getConfigStr("settings","baseClass",""); if (baseClass.empty()) { baseClass = "CBaseNode"; }
    SFString baseBase   = toProper(baseClass.substr(1));
    SFString baseName   = className.substr(1);
    SFString baseProper = toProper(baseName), baseLower = toLower(baseName), baseUpper = toUpper(baseName);
    SFString otherIncs  = classFile.getConfigStr("settings", "cIncs", "");
    SFString scope      = classFile.getConfigStr("settings", "scope", "static");
    bool     serialize  = classFile.getConfigBool("settings", "serialize", false);

    //------------------------------------------------------------------------------------------------
    SFString fieldDec, fieldSet, fieldCopy, fieldArchiveRead, fieldArchiveWrite;
    SFString fieldReg, fieldCase, fieldSubCls;

    //------------------------------------------------------------------------------------------------
    SFString hIncs;
    SFString hIncludes = classFile.getConfigStr("settings","includes","");
    while (!hIncludes.empty()) {
        SFString line = nextTokenClear(hIncludes, '|');
        if (line != "none")
            hIncs += "#include \"" + line + "\"\n";
    }

    //------------------------------------------------------------------------------------------------
    bool isBase = (baseClass == "CBaseNode");
    SFString parSer = isBase ?
                        "\tif (!preSerialize(archive))\n\t\treturn false;\n\n" :
                        "\t[{BASE_CLASS}]::Serialize(archive);\n\n";
    SFString parReg = isBase ?
                        "" :
                        "[{BASE_CLASS}]::registerClass();\n\n\t";
    SFString parCnk = isBase ?
                        "ret = next[{BASE_BASE}]Chunk(fieldIn, force, [{SHORT3}]);\n" :
                        "ret = next[{BASE_BASE}]Chunk(fieldIn, force, [{SHORT3}]);\n";
    SFString parSet = isBase?"":"\tif ([{BASE_CLASS}]::setValueByName(fieldName, fieldValue))\n\t\treturn true;\n\n";

    //------------------------------------------------------------------------------------------------
    CParameterList theList;
    SFString allFields = classFile.getConfigStr("settings","fields","");
    while (!allFields.empty()) {
        SFString fieldDef = nextTokenClear(allFields, '|');
        CParameter *f = new CParameter;
        if (fieldDef.Contains("=")) {
            f->strDefault = fieldDef;
            fieldDef = nextTokenClear(f->strDefault, '=');
        }
        f->type       = nextTokenClear(fieldDef, ' ');
        f->isPointer  = fieldDef.Contains("*"); fieldDef.Replace("*", "");
        f->isArray    = fieldDef.Contains("Array");
        f->isObject   = !f->isArray && f->type.startsWith('C');
        f->name       = fieldDef.Substitute("*", "");
        theList.AddTail(f);
    }

    //------------------------------------------------------------------------------------------------
    LISTPOS lPos = theList.GetHeadPosition();
    while (lPos) {
        CParameter *fld = theList.GetNext(lPos);
        SFString nameUpper = toUpper(fld->name);

        SFString caseFmt = "[{TYPE}]+[{NAME}]-[{ISPOINTER}]~[{ISOBJECT}]|";
        SFString decFmt  = "\t[{TYPE}] *[{NAME}];\n";
        if (!fld->isPointer) {
            decFmt.Replace("*", "");
        }
        SFString archFmt = "\tarchive >> [{NAME}];\n";
        if (fld->isPointer) {
            archFmt = "//" + archFmt;
        }
        SFString copyFmt = "\t[{NAME}] = +SHORT+.[{NAME}];\n";
        if (fld->isPointer) {
            copyFmt = "\tif ([+SHORT+.{NAME}])\n\t\t*[{NAME}] = *+SHORT+.[{NAME}];\n";
        }
        SFString badSet = "/""/\t[{NAME}] = ??; /""* unknown type: [{TYPE}] */\n";
        SFString setFmt = "\t[{NAME}]";
        SFString regFmt = "\tADD_FIELD(CL_NM, \"[{NAME}]\", T_TEXT, ++fieldNum);\n", regType;
        SFString subClsFmt = STR_SUBCLASS;

               if (fld->type == "bloom")    { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_BLOOM";
        } else if (fld->type == "wei")      { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_WEI";
        } else if (fld->type == "string")   { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "addr")     { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "address")  { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "hash")     { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "bytes32")  { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "bytes")    { setFmt = "\t[{NAME}] = [{DEFS}];\n"; regType = "T_TEXT";
        } else if (fld->type == "int8")     { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "int16")    { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "int32")    { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "int64")    { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "int256")   { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "uint8")    { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "uint16")   { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "uint32")   { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "uint64")   { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "uint256")  { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_NUMBER";
        } else if (fld->type == "bbool")    { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_BOOL";
        } else if (fld->type == "bool")     { setFmt = "\t[{NAME}] = [{DEF}];\n"; regType = "T_BOOL";
        } else if (fld->type == "float")    { setFmt = "\t[{NAME}] = [{DEFF}];\n"; regType = "T_FLOAT";
        } else if (fld->type == "double")   { setFmt = "\t[{NAME}] = [{DEFF}];\n"; regType = "T_DOUBLE";
        } else if (fld->type == "time")     { setFmt = "\t[{NAME}] = [{DEFT}];\n"; regType = "T_DATE";
        } else if (fld->isPointer)          { setFmt = "\t[{NAME}] = [{DEFP}];\n"; regType = "T_POINTER";
        } else if (fld->type.Contains("Array")) { setFmt = badSet; regType = "T_TEXT|TS_ARRAY";
        } else if (fld->isObject)           { setFmt = badSet; regType = "T_TEXT|TS_OBJECT";
        } else                              { setFmt = badSet; regType = "T_TEXT"; }

#define getDefault(a) (fld->strDefault.empty() ? (a) : fld->strDefault )
        setFmt.Replace("[{DEFS}]", getDefault("\"\""));
        setFmt.Replace("[{DEF}]",  getDefault("0"));
        setFmt.Replace("[{DEFF}]", getDefault("0.0"));
        setFmt.Replace("[{DEFT}]", getDefault("earliestDate"));
        setFmt.Replace("[{DEFP}]", getDefault("NULL"));

        // string types are already empty, but they need to be initialized in Init
        //setFmt.Replace("\t[{NAME}] = \"\";\n", "/""/\t[{NAME}] = \"\";\n");
        //if (fld->type.Contains("Array")) regType += "|TS_ARRAY";

        fieldReg += fld->Format(regFmt).Substitute("T_TEXT", regType);
        fieldReg.ReplaceAll("CL_NM", "[{CLASS_NAME}]");
        fieldCase += fld->Format(caseFmt);
        fieldDec += convertTypes(fld->Format(decFmt));
        fieldCopy += fld->Format(copyFmt).Substitute("+SHORT+", "[{SHORT}]");
        fieldSet += fld->Format(setFmt);
        if (fld->isObject && !fld->isPointer && !fld->type.Contains("Array")) {
            SFString fmt = subClsFmt;
            fmt.ReplaceAll("[FNAME]",fld->name);
            fmt.Replace("[SH3]",short3(baseLower));
            SFString fldStr = fld->Format(fmt);
            fldStr.Replace("++", "[{");
            fldStr.Replace("++", "}]");
            if (fieldSubCls.empty())
                fieldSubCls = "\n\tSFString s;\n";
            fieldSubCls += fldStr;
        }
        fieldArchiveRead += fld->Format(archFmt);
        fieldArchiveWrite += fld->Format(archFmt.Substitute(">>", "<<"));
    }

    //------------------------------------------------------------------------------------------------
    SFString operatorH = SFString(serialize ? STR_OPERATOR_H : "");
    SFString operatorC = SFString(serialize ? STR_OPERATOR_C : "\n");

    //------------------------------------------------------------------------------------------------
    SFString caseCodeStr    = getCaseCode(fieldCase);
    SFString caseSetCodeStr = getCaseSetCode(fieldCase);
    SFString subClsCodeStr  = fieldSubCls;

    //------------------------------------------------------------------------------------------------
    SFString sorts[4] = { baseLower.Left(2)+"_Name", "", baseLower+"ID", "" };
    SFString sortString = classFile.getConfigStr("settings","sort","");
    uint32_t cnt = 0;
    while (!sortString.empty())
        sorts[cnt++] = nextTokenClear(sortString, '|');

    //------------------------------------------------------------------------------------------------
    SFString headerFile = dataFile.Substitute(".txt", ".h").Substitute("./classDefinitions/", "./");
    SFString headSource = asciiFileToString(configPath("makeClass/blank.h"));
    headSource.ReplaceAll("[FIELD_COPY]",       fieldCopy);
    headSource.ReplaceAll("[FIELD_DEC]",        fieldDec);
    headSource.ReplaceAll("[FIELD_SET]",        fieldSet);
    headSource.ReplaceAll("[H_INCLUDES]",       hIncs);
    headSource.ReplaceAll("[{OPERATORS}]",      operatorH);
    headSource.ReplaceAll("[{BASE_CLASS}]",     baseClass);
    headSource.ReplaceAll("[{BASE_BASE}]",      baseBase);
    headSource.ReplaceAll("[{BASE}]",           baseUpper);
    headSource.ReplaceAll("[{CLASS_NAME}]",     className);
    headSource.ReplaceAll("[{COMMENT_LINE}]",   STR_COMMENT_LINE);
    headSource.ReplaceAll("[{LONG}]",           baseLower);
    headSource.ReplaceAll("[{PROPER}]",         baseProper);
    headSource.ReplaceAll("[{SHORT}]",          baseLower.Left(2));
    headSource.ReplaceAll("[{BASE_CLASS}]",     baseClass);
    headSource.ReplaceAll("[{BASE_BASE}]",      baseBase);
    headSource.ReplaceAll("[{BASE}]",           baseUpper);
    headSource.ReplaceAll("[{CLASS_NAME}]",     className);
    headSource.ReplaceAll("[{COMMENT_LINE}]",   STR_COMMENT_LINE);
    headSource.ReplaceAll("[{LONG}]",           baseLower);
    headSource.ReplaceAll("[{PROPER}]",         baseProper);
    headSource.ReplaceAll("[{SHORT3}]",         short3(baseLower));
    headSource.ReplaceAll("[{SHORT}]",          baseLower.Left(2));
    headSource.ReplaceAll("[{SCOPE}]",          scope);
    headSource.ReplaceAll("[{NAMESPACE1}]",     (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    headSource.ReplaceAll("[{NAMESPACE2}]",     (ns.empty() ? "" : "}  /""/ namespace qblocks\n"));
    writeTheCode(headerFile, headSource, ns);

    //------------------------------------------------------------------------------------------------
    SFString srcFile    = dataFile.Substitute(".txt", ".cpp").Substitute("./classDefinitions/", "./");
    SFString srcSource  = asciiFileToString(configPath("makeClass/blank.cpp"));
    srcSource.ReplaceAll("[ARCHIVE_READ]",      fieldArchiveRead);
    srcSource.ReplaceAll("[ARCHIVE_WRITE]",     fieldArchiveWrite);
    srcSource.ReplaceAll("[{OPERATORS}]",       operatorC);
    srcSource.ReplaceAll("[REGISTER_FIELDS]",   fieldReg);
    srcSource.ReplaceAll("[FIELD_CASE]",        caseCodeStr);
    srcSource.ReplaceAll("[OTHER_INCS]",        otherIncs);
    srcSource.ReplaceAll("[FIELD_SETCASE]",     caseSetCodeStr);
    srcSource.ReplaceAll("[{SUBCLASSFLDS}]",    subClsCodeStr);
    srcSource.ReplaceAll("[{PARENT_SER}]",      parSer);
    srcSource.ReplaceAll("[{PARENT_SER1}]",     parSer.Substitute("Serialize", "SerializeC"));
    srcSource.ReplaceAll("[{PARENT_REG}]",      parReg);
    srcSource.ReplaceAll("[{PARENT_CHNK}]\n",   parCnk);
    srcSource.ReplaceAll("[{PARENT_SET}]\n",    parSet);
    srcSource.ReplaceAll("[{BASE_CLASS}]",      baseClass);
    srcSource.ReplaceAll("[{BASE_BASE}]",       baseBase);
    srcSource.ReplaceAll("[{BASE}]",            baseUpper);
    srcSource.ReplaceAll("[{CLASS_NAME}]",      className);
    srcSource.ReplaceAll("[{COMMENT_LINE}]",    STR_COMMENT_LINE);
    srcSource.ReplaceAll("[{LONG}]",            baseLower);
    srcSource.ReplaceAll("[{PROPER}]",          baseProper);
    srcSource.ReplaceAll("[{SHORT3}]",          short3(baseLower));
    srcSource.ReplaceAll("[{SHORT}]",           baseLower.Left(2));
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
    srcSource.ReplaceAll("[{SHORT}]",           baseLower.Left(2));
    srcSource.ReplaceAll("[{SCOPE}]",           scope);
    srcSource.ReplaceAll("[{NAMESPACE1}]",      (ns.empty() ? "" : "\nnamespace qblocks {\n\n"));
    srcSource.ReplaceAll("[{NAMESPACE2}]",      (ns.empty() ? "" : "}  /""/ namespace qblocks\n"));
    writeTheCode(srcFile, srcSource, ns);
}

//------------------------------------------------------------------------------------------------
SFString getCaseCode(const SFString& fieldCase) {
    SFString baseTab = (tab+tab+tab);
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
                    if (type.Contains("List") || isPointer)
                        caseCode += "\t\t\treturn \"\";\n//";
                    caseCode += baseTab + tab + "if ( fieldIn % \"" + field + "\" )";

                    if (type == "time") {
                        caseCode += " return [{SHORT3}]->" + field + ".Format(FMT_JSON);";

                    } else if (type == "bbool") {
                        caseCode += " return asString([{SHORT3}]->" + field + ");";

                    } else if (type == "bool") {
                        caseCode += " return asString([{SHORT3}]->" + field + ");";

                    } else if (type == "bloom") {
                        caseCode += " return fromBloom([{SHORT3}]->" + field + ");";

                    } else if (type == "wei") {
                        caseCode += " return fromWei([{SHORT3}]->" + field + ");";

                    } else if (type == "addr" || type == "address") {
                        caseCode += " return fromAddress([{SHORT3}]->" + field + ");";

                    } else if (type == "hash") {
                        caseCode += " return fromHash([{SHORT3}]->" + field + ");";

                    } else if (type == "bytes" || type == "bytes32") {
                        caseCode += " return [{SHORT3}]->" + field + ";";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32" || type == "uint64") {
                        caseCode += " return asStringU([{SHORT3}]->" + field + ");";

                    } else if (type == "uint256") {
                        caseCode += " return asStringBN([{SHORT3}]->" + field + ");";

                    } else if (type == "int8" || type == "int16" || type == "int32" || type == "int64") {
                        caseCode += " return asString([{SHORT3}]->" + field + ");";

                    } else if (type == "int256") {
                        caseCode += " return asStringBN([{SHORT3}]->" + field + ");";

                    } else if (type == "float") {
                        caseCode += " return asStringF([{SHORT3}]->" + field + ");";

                    } else if (type == "double") {
                        caseCode += " return asStringD([{SHORT3}]->" + field + ");";

                    } else if (type.Contains("SFStringArray")) {
                        SFString str = STR_CASE_CODE_STRINGARRAY;
                        str.ReplaceAll("[{FIELD}]", field);
                        caseCode += str;

                    } else if (type.Contains("SFBigUintArray")) {
                        SFString str = STR_CASE_CODE_STRINGARRAY;
                        // hack for getCount clause
                        str.Replace("[{FIELD}]", field);
                        // hack for the array access
                        str.Replace("[{SHORT3}]->[{FIELD}][i]", "fromTopic([{SHORT3}]->"+field+"[i])");
                        caseCode += str;

                    } else if (type.Contains("Array")) {
                        SFString str = STR_CASE_CODE_ARRAY;
                        str.ReplaceAll("[{FIELD}]", field);
                        caseCode += str;

                    } else if (isObject) {
                        caseCode += " { expContext().noFrst=true; return [{SHORT3}]->" + field + ".Format(); }";

                    } else {
                        caseCode += " return [{SHORT3}]->" + field + ";";
                    }

                    caseCode += "\n";
                }
            }
            caseCode += baseTab + tab + "break;\n";
        }
    }
    caseCode.ReplaceAll("[BTAB]", baseTab);
    return caseCode;
}

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
                    if (type.Contains("List") || isPointer)
                        caseCode += "\t\t\treturn true;\n//";
                    caseCode += baseTab + tab + "if ( fieldName % \"" + field + "\" )";

                    if (type == "time") {
                        caseCode += " { " + field + " = snagDate(fieldValue); return true; }";

                    } else if (type == "bool" || type == "bbool") {
                        caseCode +=  " { " + field + " = toBool(fieldValue); return true; }";

                    } else if (type == "bloom") {
                        caseCode +=  " { " + field + " = toBloom(fieldValue); return true; }";

                    } else if (type == "wei") {
                        caseCode +=  " { " + field + " = toWei(fieldValue); return true; }";

                    } else if (type == "addr" || type == "address") {
                        caseCode += " { " + field + " = toAddress(fieldValue); return true; }";

                    } else if (type == "hash") {
                        caseCode += " { " + field + " = toHash(fieldValue); return true; }";

                    } else if (type.Contains("bytes")) {
                        caseCode += " { " + field + " = toLower(fieldValue); return true; }";

                    } else if (type == "int8" || type == "int16" || type == "int32" || type == "int64") {
                        caseCode +=  " { " + field + " = toLong(fieldValue); return true; }";

                    } else if (type == "int256") {
                        caseCode +=  " { " + field + " = toLong(fieldValue); return true; }";

                    } else if (type == "uint8" || type == "uint16" || type == "uint32" || type == "uint64") {
                        caseCode +=  " { " + field + " = toUnsigned(fieldValue); return true; }";

                    } else if (type == "uint256") {
                        caseCode +=  " { " + field + " = toUnsigned(fieldValue); return true; }";

                    } else if (type == "float") {
                        caseCode +=  " { " + field + " = toFloat(fieldValue); return true; }";

                    } else if (type == "double") {
                        caseCode +=  " { " + field + " = toDouble(fieldValue); return true; }";

//                  } else if (type.Contains("SFStringArray")) {
//                      caseCode += "\n\t\t\t{\n\t\t\t\treturn \"\";\n//\t\t\t\tSFStringArray not returned\n\t\t\t}";

                    } else if (type.Contains("Array")) {
//                      SFString str = STR_CASE_CODE_ARRAY;
//                      str.ReplaceAll("[{FIELD}]", field);
//                      str.ReplaceAll("[{TYPE}]", toUpper(type.substr(1).Substitute("Array","")));
//                      caseCode += str;
                        caseCode += " return true;";

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
const char* STR_CASE_CODE_ARRAY =
" {\n"
"[BTAB]\t\tuint32_t cnt = [{SHORT3}]->[{FIELD}].getCount();\n"
"[BTAB]\t\tif (!cnt) return \"\";\n"
"[BTAB]\t\tSFString ret;\n"
"[BTAB]\t\tfor (uint32_t i = 0 ; i < cnt ; i++) {\n"
"[BTAB]\t\t\tret += [{SHORT3}]->[{FIELD}][i].Format();\n"
"[BTAB]\t\t\tret += ((i < cnt - 1) ? \",\\n\" : \"\\n\");\n"
"[BTAB]\t\t}\n"
"[BTAB]\t\treturn ret;\n"
"[BTAB]\t}";

//------------------------------------------------------------------------------------------------------------
const char* STR_CASE_CODE_STRINGARRAY =
" {\n"
"[BTAB]\t\tuint32_t cnt = [{SHORT3}]->[{FIELD}].getCount();\n"
"[BTAB]\t\tif (!cnt) return \"\";\n"
"[BTAB]\t\tSFString ret;\n"
"[BTAB]\t\tfor (uint32_t i = 0 ; i < cnt ; i++) {\n"
"[BTAB]\t\t\tret += indent() + (\"\\\"\" + [{SHORT3}]->[{FIELD}][i] + \"\\\"\");\n"
"[BTAB]\t\t\tret += ((i < cnt-1) ? \",\\n\" : \"\\n\");\n"
"[BTAB]\t\t}\n"
"[BTAB]\t\treturn ret;\n"
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
"\tif (fieldIn.Contains(s)) {\n"
"\t\tSFString f = fieldIn;\n"
"\t\tf.ReplaceAll(s,\"\");\n"
"\t\tf = [SH3]->[FNAME].Format(\"++\"+f+\"++\");\n"
"\t\treturn f;\n"
"\t}\n";

//------------------------------------------------------------------------------------------------------------
SFString short3(const SFString& str) {
    SFString ret = str.Left(3);
    if (ret == "new")
        ret = "newp";
    if (ret == "ret")
        ret = "retp";
    return ret;
}
