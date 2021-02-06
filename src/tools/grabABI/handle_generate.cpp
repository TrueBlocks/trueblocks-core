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
#include <algorithm>
#include "etherlib.h"
#include "options.h"

#if 0    
//-----------------------------------------------------------------------
extern const char* STR_FACTORY1;
extern const char* STR_FACTORY2;
extern const char* STR_CLASSDEF;
extern const char* STR_HEADERFILE;
extern const char* STR_HEADER_SIGS;
extern const char* STR_CODE_SIGS;
extern const char* STR_BLOCK_PATH;
extern const char* STR_ITEMS;
extern const char* STR_ITEMS2;
extern const char* STR_FORMAT_FUNCDATA;

//-----------------------------------------------------------------------
static void makeTheCode(const string_q& fn, const string_q& addr, const string_q& projName, const string_q& pathIn);
static string_q projectName(const string_q& pathIn);
// TODO(tjayrush): global data
static string_q templateFolder = configPath("grabABI/");

//-----------------------------------------------------------------------
void COptions::handle_generate(void) {
    verbose = false;
    classDir = substitute(classDir, "~/", getHomeFolder()) + "generated/";
    string_q classDefs = classDir + "classDefinitions/";

    establishFolder(classDir);
    establishFolder(classDefs);

    string_q funcExterns, evtExterns, funcDecls, evtDecls, sigs, evts;
    string_q headers;
    if (!isToken())
        headers += ("#include \"tokenlib.h\"\n");
    if (!isWallet())
        headers += ("#include \"walletlib.h\"\n");
    string_q sources = "src= \\\n", registers, factory1, factory2;

    for (auto abi : abiList) {
        for (auto interface : abi.in terfaces) {
            if (!interface.is BuiltIn) {
                string_q name = interface.Format("[{NAME}]") + (interface.type == "event" ? "Event" : "");
                if (name == "eventEvent")
                    name = "logEntry";
                if (name.empty() && interface.type == "constructor")
                    name = "constructor";
                if (startsWith(name, '_'))
                    name = extract(name, 1);
                char ch = static_cast<char>(toupper(name[0]));
                string_q fixed;
                fixed = ch;
                name = fixed + extract(name, 1);
                string_q theClass = (is BuiltIn() ? "Q" : "C") + name;
                bool isConst = interface.constant;
                bool isEmpty = name.empty() || interface.type.empty();
                bool isLog = contains(toLower(name), "logentry");
                if (!isEmpty && !isLog) {
                    if (name != "DefFunction") {
                        if (interface.type == "event") {
                            evtExterns += interface.Format("extern const string_q evt_[{NAME}]{QB};\n");
                            string_q decl = "const string_q evt_[{NAME}]{QB} = \"" + interface.encoding + "\";\n";
                            evtDecls += interface.Format(decl);
                            if (!is BuiltIn())
                                evts += interface.Format("\tevt_[{NAME}],\n");
                        } else {
                            funcExterns += interface.Format("extern const string_q func_[{NAME}]{QB};\n");
                            string_q decl = "const string_q func_[{NAME}]{QB} = \"" + interface.encoding + "\";\n";
                            funcDecls += interface.Format(decl);
                            if (!is BuiltIn())
                                sigs += interface.Format("\tfunc_[{NAME}],\n");
                        }
                    }
                    string_q fields, assigns1, assigns2, items1, items2;
                    uint64_t nIndexed = 0;
                    for (size_t j = 0; j < interface.inputs.size(); j++) {
                        fields += interface.inputs[j].Format("[{TYPE}][ {NAME}]|");
                        assigns1 += interface.inputs[j].Format(interface.inputs[j].getFunctionAssign(j));
                        items1 += "\t\t\ttypes.push_back(\"" + interface.inputs[j].type + "\");\n";
                        nIndexed += interface.inputs[j].indexed;
                        string_q res = interface.inputs[j].Format(interface.inputs[j].getEventAssign(j + 1, nIndexed));
                        replace(res, "++", "[");
                        replace(res, "++", "]");
                        assigns2 += res;
                        items2 += "\t\t\ttypes.push_back(\"" + interface.inputs[j].type + "\");\n";
                    }

                    string_q base = (interface.type == "event" ? "LogEntry" : "Transaction");
                    if (name == "LogEntry")
                        base = "LogEntry";

                    string_q out = STR_CLASSDEF;
                    replace(out, "[{DIR}]", substitute(classDir, getHomeFolder(), "~/"));
                    replace(out, "[{CLASS}]", theClass);
                    replace(out, "[{FIELDS}]", fields);
                    replace(out, "[{BASE}]", base);
                    replace(out, "[{BASE_LOWER}]", toLower(base));

                    string_q fileName = toLower(name) + ".txt";
                    if (!isConst) {
                        headers += ("#include \"" + substitute(fileName, ".txt", ".h") + "\"\n");
                        registers += "\t" + theClass + "::registerClass();\n";
                    }
                    sources += substitute(fileName, ".txt", ".cpp") + " \\\n";
                    if (base == "Transaction") {
                        string_q f1, fName = interface.Format("[{NAME}]");
                        f1 = string_q(STR_FACTORY1);
                        replaceAll(f1, "[{CLASS}]", theClass);
                        replaceAll(f1, "[{NAME}]", fName);
                        if (fName == "defFunction")
                            replaceAll(f1, "encoding == func_[{LOWER}]", "encoding.length() < 10");
                        else
                            replaceAll(f1, "[{LOWER}]", fName);
                        replaceAll(f1, "[{ASSIGNS1}]", assigns1);
                        replaceAll(f1, "[{ITEMS1}]", items1);
                        string_q parseIt = "";
                        replaceAll(f1, "[{PARSEIT}]", parseIt);
                        replaceAll(f1, "[{BASE}]", base);
                        replaceAll(
                            f1, "[{SIGNATURE}]",
                            substitute(substitute(substitute(substitute(interface.getSignature(SIG_DEFAULT), "\t", ""),
                                                             "  ", " "),
                                                  " (", "("),
                                       ",", ", "));
                        replace(f1, "[{ENCODING}]", interface.getSignature(SIG_ENCODE));
                        replace(f1, " defFunction(string)", "()");
                        if (!isConst)
                            factory1 += f1;

                    } else if (name != "LogEntry") {
                        string_q f2, fName = interface.Format("[{NAME}]");
                        f2 = substitute(substitute(string_q(STR_FACTORY2), "[{CLASS}]", theClass), "[{LOWER}]", fName);
                        replace(f2, "[{ASSIGNS2}]", assigns2);
                        replace(f2, "[{ITEMS2}]", items2);
                        string_q parseIt = "";
                        replace(f2, "[{PARSEIT}]", parseIt);
                        replace(f2, "[{BASE}]", base);
                        replace(f2, "[{SIGNATURE}]",
                                substitute(
                                    substitute(substitute(substitute(interface.getSignature(SIG_DEFAULT | SIG_IINDEXED),
                                                                     "\t", ""),
                                                          "  ", " "),
                                               " (", "("),
                                    ",", ", "));
                        replace(f2, "[{ENCODING}]", interface.getSignature(SIG_ENCODE));
                        if (!isConst)
                            factory2 += f2;
                    }

                    if (name != "logEntry" && !isConst) {
                        // hack warning
                        replaceAll(out, "bytes32[]", "CStringArray");
                        replaceAll(out, "uint256[]", "CBigUintArray");  // order matters
                        replaceAll(out, "int256[]", "CBigIntArray");
                        replaceAll(out, "uint32[]", "CUintArray");  // order matters
                        replaceAll(out, "int32[]", "CIntArray");
                        stringToAsciiFile(classDefs + fileName, out);
                        if (interface.type == "event")
                            LOG_INFO("Generating class for derived event type: '", theClass, "'");
                        else
                            LOG_INFO("Generating class for derived transaction type: '", theClass, "'");

                        string_q makeClass = configPath("makeClass/makeClass");
                        if (!fileExists(makeClass)) {
                            LOG_ERR(makeClass,
                                    " was not found. This executable is required to run grabABI. Quitting...");
                            quickQuitHandler(EXIT_FAILURE);
                        }
                        if (!name.empty()) {
                            string_q cmd =
                                makeClass + " -r " + "generated/" + toLower(name) + (isTestMode() ? " --force" : "");
                            string_q res = doCommand(cmd);
                            if (!res.empty())
                                LOG_INFO("\t", res);
                        }
                    }
                }
            }
        }
    }

    // The library header file
    if (!is BuiltIn())
        headers += ("#include \"visitor.h\"\n");
    string_q headerCode = substitute(string_q(STR_HEADERFILE), "[{HEADERS}]", headers);
    string_q parseInit = "parselib_init(QUITHANDLER qh)";
    if (!is BuiltIn())
        replaceAll(headerCode, "[{PREFIX}]_init(void)", parseInit);
    // replaceAll(headerCode, "[{ADDRESS}]", substitute(primaryAddr, "0x", ""));
    replaceAll(headerCode, "[{HEADER_SIGS}]", is BuiltIn() ? "" : STR_HEADER_SIGS);
    replaceAll(headerCode, "[{PREFIX}]", toLower(prefix));
    string_q pprefix = (is BuiltIn() ? substitute(toProper(prefix), "lib", "") : "Func");
    replaceAll(headerCode, "[{PPREFIX}]", pprefix);
    replaceAll(headerCode, "FuncEvent", "Event");
    string_q comment = "//------------------------------------------------------------------------\n";
    funcExterns = (funcExterns.empty() ? "// No functions" : funcExterns);
    evtExterns = (evtExterns.empty() ? "// No events" : evtExterns);
    replaceAll(headerCode, "[{EXTERNS}]", comment + funcExterns + "\n" + comment + evtExterns);
    headerCode = substitute(headerCode, "{QB}", (is BuiltIn() ? "_qb" : ""));
    writeTheCode(codewrite_t(classDir + prefix + ".h", headerCode, "", 4, false, true, isTestMode()));

    // The library make file
    replaceReverse(sources, " \\\n", " \\\n" + prefix + ".cpp\n");
    if (!is BuiltIn()) {
        string_q makefile;
        asciiFileToString(templateFolder + "CMakeLists.txt", makefile);
        replaceAll(makefile, "[{PROJECT_NAME}]", projectName(classDir));
        writeTheCode(codewrite_t(classDir + "CMakeLists.txt", makefile, "", 0, false, true, isTestMode()));
    }

    // The library source file
    replace(factory1, "} else ", "");
    replace(factory1, "contains(func, \"defFunction|\")", "!contains(func, \"|\")");
    replace(factory1, " if (encoding == func_[{LOWER}])", "");
    replace(factory2, "} else ", "");

    string_q sourceCode;
    asciiFileToString(templateFolder + "parselib.cpp", sourceCode);
    parseInit = "parselib_init(QUITHANDLER qh)";
    if (!is BuiltIn())
        replaceAll(sourceCode, "[{PREFIX}]_init(QUITHANDLER qh)", parseInit);
    if (isToken()) {
        replace(sourceCode, "return promoteToToken(p);", "return promoteToWallet(p);");
        replace(sourceCode, "return promoteToTokenEvent(p);", "return promoteToWalletEvent(p);");
    } else if (isWallet()) {
        replace(sourceCode, "return promoteToToken(p);", "return new CTransaction(*p);");
        replace(sourceCode, "return promoteToTokenEvent(p);", "return new CLogEntry(*p);");
        replaceAll(sourceCode, "never returns NULL",
                   "If we haven't found the thing, we can send back an extended thing");
    }
    replace(sourceCode, "[{BLKPATH}]", is BuiltIn() ? "" : STR_BLOCK_PATH);
    replaceAll(sourceCode, "[{CODE_SIGS}]", (is BuiltIn() ? "" : STR_CODE_SIGS));
    // replaceAll(sourceCode, "[{ADDRESS}]", substitute(primaryAddr, "0x", ""));
    replaceAll(sourceCode, "[{ABI}]", "");  // theABI);
    replaceAll(sourceCode, "[{REGISTERS}]", registers);
    string_q chainInit = (isToken() ? "\twalletlib_init(qh);\n" : (isWallet() ? "" : "\ttokenlib_init(qh);\n"));
    replaceAll(sourceCode, "[{CHAINLIB}]", chainInit);
    replaceAll(sourceCode, "[{FACTORY1}]", factory1.empty() ? "\t\t{\n\t\t\t// No functions\n" : factory1);
    replaceAll(sourceCode, "[{INIT_CODE}]", factory1.empty() ? "" : STR_ITEMS);
    replaceAll(sourceCode, "[{FACTORY2}]", factory2.empty() ? "\t\t{\n\t\t\t// No events\n" : factory2);
    replaceAll(sourceCode, "[{INIT_CODE2}]", factory2.empty() ? "" : STR_ITEMS2);

    headers = ("#include \"tokenlib.h\"\n");
    headers += ("#include \"walletlib.h\"\n");
    if (!is BuiltIn())
        headers += "#include \"[{PREFIX}].h\"\n";
    replaceAll(sourceCode, "[{HEADERS}]", headers);
    replaceAll(sourceCode, "[{PREFIX}]", prefix);
    pprefix = (is BuiltIn() ? substitute(toProper(prefix), "lib", "") : "Func");
    replaceAll(sourceCode, "[{PPREFIX}]", pprefix);
    replaceAll(sourceCode, "FuncEvent", "Event");
    replaceAll(sourceCode, "[{FUNC_DECLS}]", funcDecls.empty() ? "// No functions" : funcDecls);
    replaceAll(sourceCode, "[{SIGS}]", sigs.empty() ? "\t// No functions\n" : sigs);
    replaceAll(sourceCode, "[{EVENT_DECLS}]", evtDecls.empty() ? "// No events" : evtDecls);
    replaceAll(sourceCode, "[{EVTS}]", evts.empty() ? "\t// No events\n" : evts);
    sourceCode = substitute(sourceCode, "{QB}", (is BuiltIn() ? "_qb" : ""));
    writeTheCode(codewrite_t(classDir + prefix + ".cpp", sourceCode, "", 4, false, true, isTestMode()));

    string_q primaryAddr = "";
    // The code
    if (!is BuiltIn()) {
        string_q addrList;
        for (auto addr : addrs)
            addrList += (addr + "");
        makeTheCode("rebuild", trim(addrList), projectName(classDir), classDir);
        makeTheCode("CMakeLists.txt", primaryAddr, projectName(classDir), classDir);
        makeTheCode("debug.h", primaryAddr, projectName(classDir), classDir);
        makeTheCode("debug.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("options.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("options.h", primaryAddr, projectName(classDir), classDir);
        makeTheCode("main.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("main.h", primaryAddr, projectName(classDir), classDir);
        makeTheCode("visitor.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("visitor.h", primaryAddr, projectName(classDir), classDir);
        makeTheCode("accounting.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("display.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("processing.cpp", primaryAddr, projectName(classDir), classDir);
        makeTheCode("processing.h", primaryAddr, projectName(classDir), classDir);
    }
}

//-----------------------------------------------------------------------
static string_q projectName(const string_q& pathIn) {
    // pick off the last part of the path
    CStringArray parts;
    explode(parts, substitute(pathIn, "//", "/"), '/');
    return parts[parts.size() - 1];
}

//-----------------------------------------------------------------------
static void makeTheCode(const string_q& fn, const string_q& addr, const string_q& projName, const string_q& pathIn) {
    string_q theCode;
    asciiFileToString(templateFolder + fn, theCode);
    replaceAll(theCode, "[{ADDR}]", addr);
    replaceAll(theCode, "[{PROJECT_NAME}]", projName);
    writeTheCode(codewrite_t(pathIn + fn, theCode, "", fn != "CMakeFile.txt", false, true, isTestMode()));
}

//-----------------------------------------------------------------------
const char* STR_FACTORY1 =
    "\t\t} else if (encoding == func_[{LOWER}]{QB}) {\n"
    "\t\t\t// [{SIGNATURE}]\n"
    "\t\t\t// [{ENCODING}]\n"
    "\t\t\t[{CLASS}] *a = new [{CLASS}];\n"
    "\t\t\ta->C[{BASE}]::operator=(*p);\n"
    "[{ASSIGNS1}]"
    "[{ITEMS1}]"
    "\t\t\treturn a;\n"
    "\n";

//-----------------------------------------------------------------------
const char* STR_FACTORY2 =
    "\t\t} else if (topic_2_Str(p->topics[0]) % evt_[{LOWER}]{QB}) {\n"
    "\t\t\t// [{SIGNATURE}]\n"
    "\t\t\t// [{ENCODING}]\n"
    "\t\t\t[{CLASS}] *a = new [{CLASS}];\n"
    "\t\t\ta->C[{BASE}]::operator=(*p);\n"
    "[{ASSIGNS2}]"
    "[{ITEMS2}]"
    "\t\t\treturn a;\n"
    "\n";

//-----------------------------------------------------------------------
const char* STR_CLASSDEF =
    "[settings]\n"
    "class = [{CLASS}]\n"
    "base_class = C[{BASE}]\n"
    "fields = [{FIELDS}]\n"
    "includes = [{BASE_LOWER}].h\n"
    "cpp_includes = #include \"etherlib.h\"\n";

//-----------------------------------------------------------------------
const char* STR_HEADERFILE =
    "#pragma once\n"
    "/*-------------------------------------------------------------------------\n"
    " * This source code is confidential proprietary information which is\n"
    " * copyright (c) 2018, 2019 TrueBlocks, LLC (http://trueblocks.io)\n"
    " * All Rights Reserved\n"
    " *------------------------------------------------------------------------*/\n"
    " /*\n"
    "  *\n"
    "  * This code was generated automatically from grabABI and makeClass You may \n"
    "  * edit the file, but keep your edits inside the 'EXISTING_CODE' tags.\n"
    "  *\n"
    "  */\n"
    "#include \"etherlib.h\"\n"
    "[{HEADERS}]\n"
    "//------------------------------------------------------------------------\n"
    "extern void [{PREFIX}]_init(void);\n"
    "extern const CTransaction *promoteTo[{PPREFIX}](const CTransaction *p);\n"
    "extern const CLogEntry *promoteTo[{PPREFIX}]Event(const CLogEntry *p);\n"
    "\n[{EXTERNS}][{HEADER_SIGS}]\n\n// EXISTING_CODE\n// EXISTING_CODE\n";

//-----------------------------------------------------------------------
const char* STR_HEADER_SIGS =
    "\n\n//-----------------------------------------------------------------------------\n"
    "extern const string_q sigs[];\n"
    "extern const string_q topics[];\n"
    "\n"
    "extern const size_t nSigs;\n"
    "extern const size_t nTopics;";

//-----------------------------------------------------------------------
const char* STR_CODE_SIGS =
    "\n\n//-----------------------------------------------------------------------------\n"
    "string_q sigs[] = {\n"
    "\t// Token support\n"
    "\tfunc_approve_qb,\n"
    "\tfunc_transferFrom_qb,\n"
    "\tfunc_transfer_qb,\n"
    "\t// Wallet support\n"
    "\tfunc_addOwner_qb,\n"
    "\tfunc_changeOwner_qb,\n"
    "\tfunc_changeRequirement_qb,\n"
    "\tfunc_confirm_qb,\n"
    "\tfunc_execute_qb,\n"
    "\tfunc_isOwner_qb,\n"
    "\tfunc_kill_qb,\n"
    "\tfunc_removeOwner_qb,\n"
    "\tfunc_resetSpentToday_qb,\n"
    "\tfunc_revoke_qb,\n"
    "\tfunc_setDailyLimit_qb,\n"
    "\t// Contract support\n"
    "[{SIGS}]"
    "};\n"
    "size_t nSigs = sizeof(sigs) / sizeof(string_q);\n"
    "\n"
    "//-----------------------------------------------------------------------------\n"
    "const string_q topics[] = {\n"
    "\t// Token support\n"
    "\tevt_Transfer_qb,\n"
    "\tevt_Approval_qb,\n"
    "\t// Wallet support\n"
    "\tevt_Confirmation_qb,\n"
    "\tevt_ConfirmationNeeded_qb,\n"
    "\tevt_Deposit_qb,\n"
    "\tevt_MultiTransact_qb,\n"
    "\tevt_OwnerAdded_qb,\n"
    "\tevt_OwnerChanged_qb,\n"
    "\tevt_OwnerRemoved_qb,\n"
    "\tevt_RequirementChanged_qb,\n"
    "\tevt_Revoke_qb,\n"
    "\tevt_SingleTransact_qb,\n"
    "\t// Contract support\n"
    "[{EVTS}]"
    "};\n"
    "const size_t nTopics = sizeof(topics) / sizeof(string_q);\n"
    "\n";

//-----------------------------------------------------------------------
const char* STR_BLOCK_PATH = "\n\tetherlib_init(qh);";

//-----------------------------------------------------------------------
const char* STR_ITEMS =
    "\t\tCStringArray types;\n"
    "\n"
    "\t\tstring_q encoding = extract(p->input, 0, 10);\n"
    "\t\tstring_q params   = extract(p->input, 10);\n";

//-----------------------------------------------------------------------
const char* STR_ITEMS2 =
    "\t\tCStringArray types;\n"
    "\t\tstring_q data = extract(p->data, 2);\n"
    "\t\tstring_q params;\n"
    "\t\tbool first = true;\n"
    "\t\tfor (auto t : p->topics) {\n"
    "\t\t    if (!first)\n"
    "\t\t        params += extract(topic_2_Str(t),2);\n"
    "\t\t    first = false;\n"
    "\t\t}\n"
    "\t\tparams += data;\n";
#endif
