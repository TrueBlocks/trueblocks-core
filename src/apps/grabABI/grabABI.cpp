/*-------------------------------------------------------------------------
 * This source code is confidential proprietary information which is
 * Copyright (c) 2017 by Great Hill Corporation.
 * All Rights Reserved
 *
 * The LICENSE at the root of this repo details your rights (if any)
 *------------------------------------------------------------------------*/
#include "etherlib.h"
#include "options.h"

//-----------------------------------------------------------------------
extern void addDefaultFuncs(CFunctionArray& funcs);
extern SFString getAssign(const CParameter *p, SFUint32 which);
extern SFString getEventAssign(const CParameter *p, SFUint32 which, SFUint32 prevIdxs);

//-----------------------------------------------------------------------
extern const char* STR_FACTORY1;
extern const char* STR_FACTORY2;
extern const char* STR_CLASSDEF;
extern const char* STR_HEADERFILE;
extern const char* STR_HEADER_SIGS;
extern const char* STR_CODE_SIGS;
extern const char* STR_BLOCK_PATH;

//-----------------------------------------------------------------------
SFString templateFolder = getHomeFolder() + ".quickBlocks/grabABI/";

//-----------------------------------------------------------------------
int sortFunctionByName(const void *v1, const void *v2) {
    const CFunction *f1 = reinterpret_cast<const CFunction*>(v1);
    const CFunction *f2 = reinterpret_cast<const CFunction*>(v2);
    return f1->name.compare(f2->name);
}

SFString classDir;
//-----------------------------------------------------------------------
inline void makeTheCode(const SFString& fn, const SFString& addr)
{
    SFString theCode = asciiFileToString(templateFolder + fn);
    theCode.ReplaceAll("[{ADDR}]", addr);
    writeTheCode(classDir + "../" + fn, theCode, "", fn != "makefile");
}

//-----------------------------------------------------------------------
void addIfUnique(CFunctionArray& functions, const CFunction& func)
{
//#error
    if (func.name.empty())
        return;

    for (int i = 0 ; i < functions.getCount() ; i++)
        if (functions[i].encoding == func.encoding)
            return;

    functions[functions.getCount()] = func;
}

//-----------------------------------------------------------------------
SFString acquireABI(CFunctionArray& functions, const SFAddress& addr, bool silent, bool builtIn=false)
{
    SFString results, ret;
    SFString fileName = configPath("abis/" + addr + ".json");
    if (fileExists(fileName)) {

        cerr << "Reading ABI from cache " + fileName + "\n";
        results = asciiFileToString(fileName);

    } else {
        cerr << "Reading ABI from EtherScan\n";
        SFString url = SFString("http:/")
                            + "/api.etherscan.io/api?module=contract&action=getabi&address="
                            + addr;
        results = urlToString(url).Substitute("\\", "").Substitute("\"[", "[").Substitute("]\"", "]");
        if (verbose) {
            cout << verbose << "---------->" << results << "\n";
            cout.flush();
        }

        if (!results.Contains("NOTOK")) {
            nextTokenClear(results, '[');
            results.ReplaceReverse("]}", "");
            cerr << "Caching abi in " << fileName << "\n";
            stringToAsciiFile(fileName, "["+results+"]");
        } else {
            cerr << "Could not grab ABI for " + addr + " from etherscan.io.\n";
            if (!silent)
                exit(0);
        }
    }

    ret = results.Substitute("\n", "").Substitute("\t", "").Substitute(" ", "");
    char *s = (char *)(results.c_str()); // NOLINT
    char *p = cleanUpJson(s);
    while (p && *p) {
        CFunction func;
        uint32_t nFields = 0;
        p = func.parseJson(p, nFields);
        func.isBuiltin = builtIn;
        addIfUnique(functions, func);
    }

    return ret;
}

//-----------------------------------------------------------------------
int main(int argc, const char *argv[]) {

    etherlib_init("");

    // We keep only a single slurper. If the user is using the --file option and they
    // are reading the same account repeatedly, we only need to read the cache once.
    COptions options;

    if (!options.prepareArguments(argc, argv))
        return 0;

    while (!options.commandList.empty()) {
        SFString command = nextTokenClear(options.commandList, '\n');
        if (!options.parseArguments(command))
            return 0;

        if (options.open) {
            for (int i = 0 ; i < options.nAddrs ; i++) {
                SFString fileName = configPath("abis/" + options.addrs[i] + ".json");
                SFString cmd = "open -a /Applications/TextEdit.app " + fileName;
                doCommand(cmd);
            }
            return 0;
        }

        CFunctionArray functions;
        SFString addrList;
        bool isGenerate = !options.classDir.empty();
        if (options.addrs[0] != "0xTokenLib" && options.addrs[0] != "0xWalletLib" && isGenerate)
        {
            acquireABI(functions, "0xTokenLib",  options.silent, true);
            acquireABI(functions, "0xWalletLib", options.silent, true);
        }
        for (int i = 0 ; i < options.nAddrs ; i++) {
            options.theABI += ("ABI for addr : " + options.addrs[i] + "\n");
            options.theABI += acquireABI(functions, options.addrs[i], options.silent) + "\n\n";
            addrList += (options.addrs[i] + "|");
        }

        if (!isGenerate) {
            // print to a buffer because we have to modify it before we print it
            cout << "ABI for address " << options.primaryAddr << (options.nAddrs>1 ? " and others" : "") << "\n";
            for (int i = 0 ; i < functions.getCount() ; i++) {
                CFunction *func = &functions[i];
                if (!func->constant || !options.noconst)
                    cout << func->getSignature(options.parts) << "\n";
            }
            cout << "\n";

        } else {
            verbose = false;
            functions.Sort(sortFunctionByName);
//            if (options.isToken())
//                addDefaultFuncs(functions);

            classDir = (options.classDir).Substitute("~/", getHomeFolder());
            SFString classDefs = classDir + "classDefinitions/";
            establishFolder(classDefs);

            SFString funcExterns, evtExterns, funcDecls, evtDecls, sigs, evts;
            SFString headers;
            if (!options.isToken()) headers += ("#include \"tokenlib.h\"\n");
            if (!options.isWallet()) headers += ("#include \"walletlib.h\"\n");
            SFString sources = "src= \\\n", registers, factory1, factory2;
            for (int i = 0 ; i < functions.getCount() ; i++) {
                CFunction *func = &functions[i];
                if (!func->isBuiltin) {
                    SFString name = func->Format("[{NAME}]") + (func->type == "event" ? "Event" : "");
                    if (name == "eventEvent")
                        name = "logEntry";
                    if (name.startsWith('_'))
                        name = name.substr(1);
                    char ch = static_cast<char>(toupper(name[0]));
                    SFString fixed(ch);
                    name = fixed + name.substr(1);
                    SFString theClass = (options.isBuiltin() ? "Q" : "C") + name;
                    bool isConst = func->constant;
                    bool isEmpty = name.empty() || func->type.empty();
                    bool isLog = name.ContainsI("logentry");
                    bool isConstructor = func->type % "constructor";
                    if (!isConst && !isEmpty && !isLog && !isConstructor) {
                        if (name != "DefFunction") {
                            if (func->type == "event") {
                                evtExterns += func->Format("extern const SFString evt_[{NAME}]{QB};\n");
                                SFString decl = "const SFString evt_[{NAME}]{QB} = \"" + func->encoding + "\";\n";
                                evtDecls += func->Format(decl);
                                if (!options.isBuiltin())
                                    evts += func->Format("\tevt_[{NAME}],\n");
                            } else {
                                funcExterns += func->Format("extern const SFString func_[{NAME}]{QB};\n");
                                SFString decl = "const SFString func_[{NAME}]{QB} = \"" + func->encoding + "\";\n";
                                funcDecls += func->Format(decl);
                                if (!options.isBuiltin())
                                    sigs += func->Format("\tfunc_[{NAME}],\n");
                            }
                        }
                        SFString fields, assigns1, assigns2, items1;
                        SFUint32 nIndexed = 0;
                        for (int j = 0 ; j < func->inputs.getCount() ; j++) {
                            fields   += func->inputs[j].Format("[{TYPE}][ {NAME}]|");
                            assigns1 += func->inputs[j].Format(getAssign(&func->inputs[j], j));
                            items1   += "\t\t\titems[nItems++] = \"" + func->inputs[j].type + "\";\n";
                            nIndexed += func->inputs[j].indexed;
                            SFString res = func->inputs[j].Format(getEventAssign(&func->inputs[j], j+1, nIndexed));
                            res.Replace("++", "[");
                            res.Replace("++", "]");
                            assigns2 += res;
                        }

                        SFString base = (func->type == "event" ? "LogEntry" : "Transaction");
                        if (name == "LogEntry")
                            base = "LogEntry";

                        SFString out = STR_CLASSDEF;
                        out.Replace("[{DIR}]", options.classDir.Substitute(getHomeFolder(), "~/"));
                        out.Replace("[{CLASS}]", theClass);
                        out.Replace("[{FIELDS}]", fields);
                        out.Replace("[{BASE}]", base);
                        out.Replace("[{BASE_LOWER}]", toLower(base));

                        SFString fileName = toLower(name)+".txt";
                        headers += ("#include \"" + fileName.Substitute(".txt", ".h") + "\"\n");
                        sources += fileName.Substitute(".txt", ".cpp") + " \\\n";
                        registers += "\t" + theClass + "::registerClass();\n";
                        if (base == "Transaction") {
                            SFString f1, fName = func->Format("[{NAME}]");
                            f1 = SFString(STR_FACTORY1);
                            f1.ReplaceAll("[{CLASS}]", theClass);
                            f1.ReplaceAll("[{NAME}]", func->Format("[{NAME}]"));
                            if (fName == "defFunction")
                                f1.ReplaceAll("encoding == func_[{LOWER}]", "encoding.length() < 10");
                            else
                                f1.ReplaceAll("[{LOWER}]", fName);
                            f1.ReplaceAll("[{ASSIGNS1}]", assigns1);
                            f1.ReplaceAll("[{ITEMS1}]", items1);
                            f1.ReplaceAll("[{PARSEIT}]", (items1.empty() ? "" : " + parse(params,nItems,items)"));
                            f1.ReplaceAll("[{BASE}]", base);
                            f1.Replace("[{SIGNATURE}]", func->getSignature(SIG_DEFAULT)
                                                            .Substitute("\t", "")
                                                            .Substitute("  ", " ")
                                                            .Substitute(" (", "(")
                                                            .Substitute(",", ", "));
                            f1.Replace("[{ENCODING}]", func->getSignature(SIG_ENCODE));
                            f1.Replace(" defFunction(string)", "()");
                            factory1 += f1;

                        } else if (name != "LogEntry") {
                            SFString f2;
                            f2 = SFString(STR_FACTORY2)
                                            .Substitute("[{CLASS}]", theClass)
                                            .Substitute("[{LOWER}]", func->Format("[{NAME}]"));
                            f2.Replace("[{ASSIGNS2}]", assigns2);
                            f2.Replace("[{BASE}]", base);
                            f2.Replace("[{SIGNATURE}]", func->getSignature(SIG_DEFAULT|SIG_IINDEXED)
                                       .Substitute("\t", "").Substitute("  ", " ")
                                       .Substitute(" (", "(").Substitute(",", ", "));
                            f2.Replace("[{ENCODING}]", func->getSignature(SIG_ENCODE));
                            factory2 += f2;
                        }

                        if (name != "logEntry") {
                            // hack warning
                            out.ReplaceAll("bytes32[]", "SFStringArray");
                            out.ReplaceAll("uint256[]", "SFBigUintArray");  // order matters
                            out.ReplaceAll("int256[]", "SFBigIntArray");
                            out.ReplaceAll("uint32[]", "SFUintArray");  // order matters
                            out.ReplaceAll("int32[]", "SFIntArray");
                            stringToAsciiFile(classDefs+fileName, out);
                            if (func->type == "event")
                                cout << "Generating class for event type: '" << theClass << "'\n";
                            else
                                cout << "Generating class for derived transaction type: '" << theClass << "'\n";

                            SFString makeClassCmd = getHomeFolder() + ".quickBlocks/makeClass/makeClass -r ";
                            SFString res = doCommand(makeClassCmd + toLower(name));
                            if (!res.empty())
                                cout << "\t" << res << "\n";
                        }
                    }
                }
            }

            // The library header file
            if (!options.isBuiltin())
                headers += ("#include \"support.h\"\n");
            SFString headerCode = SFString(STR_HEADERFILE).Substitute("[{HEADERS}]", headers);
            SFString parseInit = "parselib_init(const SFString& method)";
            if (!options.isBuiltin())
                headerCode.ReplaceAll("[{PREFIX}]_init(void)", parseInit);
            headerCode.ReplaceAll("[{ADDR}]", options.primaryAddr.Substitute("0x", ""));
            headerCode.ReplaceAll("[{HEADER_SIGS}]", options.isBuiltin() ? "" : STR_HEADER_SIGS);
            headerCode.ReplaceAll("[{PREFIX}]", toLower(options.prefix));
            SFString pprefix = (options.isBuiltin() ? toProper(options.prefix).Substitute("lib", "") : "Func");
            headerCode.ReplaceAll("[{PPREFIX}]", pprefix);
            headerCode.ReplaceAll("FuncEvent", "Event");
            SFString comment = "/""/------------------------------------------------------------------------\n";
            funcExterns = (funcExterns.empty() ? "/""/ No functions" : funcExterns);
            evtExterns = (evtExterns.empty() ? "/""/ No events" : evtExterns);
            headerCode.ReplaceAll("[{EXTERNS}]", comment+funcExterns+"\n"+comment+evtExterns);
            headerCode = headerCode.Substitute("{QB}", (options.isBuiltin() ? "_qb" : ""));
            writeTheCode(classDir + options.prefix + ".h", headerCode);

            // The library make file
            sources.ReplaceReverse(" \\\n", " \\\n" + options.prefix + ".cpp\n");
            SFString makefile = asciiFileToString(templateFolder + "parselib/makefile").Substitute("[{SOURCES}]", sources);
            if (!options.isBuiltin())
                makefile.Replace("product=../[{PREFIX}].a\n", "product=parselib.a\n");
            makefile.ReplaceAll("[{EXTRA}]", " -I$(libraries)/tokenlib -I$(libraries)/walletlib");
            makefile.ReplaceAll("[{PREFIX}]", options.prefix);
            writeTheCode(classDir + "makefile", makefile.Substitute("{QB}", (options.isBuiltin()?"_qb":"")), "", false);

            // The library source file
            factory1.Replace("} else ", "");
            factory1.Replace("func.Contains(\"defFunction|\")", "!func.Contains(\"|\")");
            factory1.Replace(" if (encoding == func_[{LOWER}])", "");
            factory2.Replace("} else ", "");

            SFString sourceCode = asciiFileToString(templateFolder + "parselib/parselib.cpp");
            parseInit = "parselib_init(const SFString& method)";
            if (!options.isBuiltin())
                sourceCode.ReplaceAll("[{PREFIX}]_init(void)", parseInit);
            if (options.isToken()) {
                sourceCode.Replace("return promoteToToken(p);", "return promoteToWallet(p);");
                sourceCode.Replace("return promoteToTokenEvent(p);", "return promoteToWalletEvent(p);");
            } else if (options.isWallet()) {
                sourceCode.Replace("return promoteToToken(p);", "return NULL;");
                sourceCode.Replace("return promoteToTokenEvent(p);", "return NULL;");
            }
            sourceCode.Replace("[{BLKPATH}]", options.isBuiltin() ? "" : STR_BLOCK_PATH);
            sourceCode.ReplaceAll("[{CODE_SIGS}]", (options.isBuiltin() ? "" : STR_CODE_SIGS));
            sourceCode.ReplaceAll("[{ADDR}]", options.primaryAddr.Substitute("0x", ""));
            sourceCode.ReplaceAll("[{ABI}]", options.theABI);
            sourceCode.ReplaceAll("[{REGISTERS}]", registers);
            SFString chainInit = (options.isToken() ?
                                    "\twalletlib_init();\n" :
                                  (options.isWallet() ? "" : "\ttokenlib_init();\n"));
            sourceCode.ReplaceAll("[{CHAINLIB}]", chainInit);
            sourceCode.ReplaceAll("[{FACTORY1}]", factory1.empty() ? "\t\t{\n\t\t\t/""/ No functions\n" : factory1);
            sourceCode.ReplaceAll("[{FACTORY2}]", factory2.empty() ? "\t\t{\n\t\t\t/""/ No events\n" : factory2);

            headers = ("#include \"tokenlib.h\"\n");
            headers += ("#include \"walletlib.h\"\n");
            if (!options.isBuiltin())
                headers += "#include \"[{PREFIX}].h\"\n";
            sourceCode.ReplaceAll("[{HEADERS}]", headers);
            sourceCode.ReplaceAll("[{PREFIX}]", options.prefix);
            pprefix = (options.isBuiltin() ? toProper(options.prefix).Substitute("lib", "") : "Func");
            sourceCode.ReplaceAll("[{PPREFIX}]", pprefix);
            sourceCode.ReplaceAll("FuncEvent", "Event");
            sourceCode.ReplaceAll("[{FUNC_DECLS}]", funcDecls.empty() ? "/""/ No functions" : funcDecls);
            sourceCode.ReplaceAll("[{SIGS}]", sigs.empty() ? "\t/""/ No functions\n" : sigs);
            sourceCode.ReplaceAll("[{EVENT_DECLS}]", evtDecls.empty() ? "/""/ No events" : evtDecls);
            sourceCode.ReplaceAll("[{EVTS}]", evts.empty() ? "\t/""/ No events\n" : evts);
            sourceCode = sourceCode.Substitute("{QB}", (options.isBuiltin() ? "_qb" : ""));
            writeTheCode(classDir + options.prefix + ".cpp", sourceCode.Substitute("XXXX","[").Substitute("YYYY","]"));

            // The support.h file
            if (!options.isBuiltin()) {
                makeTheCode("support.h",   options.primaryAddr.Substitute("0x", ""));
                makeTheCode("support.cpp", options.primaryAddr.Substitute("0x", ""));
                makeTheCode("rebuild",     StripTrailing(addrList,'|').Substitute("|", " "));
                makeTheCode("makefile",    options.primaryAddr);
                makeTheCode("options.h",   options.primaryAddr);
                makeTheCode("options.cpp", options.primaryAddr);
                makeTheCode("main.cpp",    options.primaryAddr);
            }
        }
    }
    return 0;
}

//-----------------------------------------------------------------------
SFString getAssign(const CParameter *p, SFUint32 which) {

    SFString ass;
    SFString type = p->Format("[{TYPE}]");

    if (type.Contains("[") && type.Contains("]"))
    {
        const char* STR_ASSIGNARRAY =
            "\t\t\twhile (!params.empty()) {\n"
            "\t\t\t\tSFString val = params.substr(0,64);\n"
            "\t\t\t\tparams = params.substr(64);\n"
            "\t\t\t\ta->[{NAME}]XXXXa->[{NAME}].getCount()YYYY = val;\n"
            "\t\t\t}\n";
        return p->Format(STR_ASSIGNARRAY);
    }

    if (type == "uint" || type == "uint256") { ass = "toWei(\"0x\"+[{VAL}]);";
    } else if (type.Contains("uint")) { ass = "toLongU([{VAL}]);";
    } else if (type.Contains("int") || type.Contains("bool")) { ass = "toLong([{VAL}]);";
    } else if (type.Contains("address")) { ass = "toAddress([{VAL}]);";
    } else { ass = "[{VAL}];";
    }

    ass.Replace("[{VAL}]", "params.substr(" + asStringU(which) + "*64" + (type == "bytes" ? "" : ",64") + ")");
    return p->Format("\t\t\ta->[{NAME}] = " + ass + "\n");
}

//-----------------------------------------------------------------------
SFString getEventAssign(const CParameter *p, SFUint32 which, SFUint32 nIndexed) {
    SFString type = p->Format("[{TYPE}]"), ass;
    if (type == "uint" || type == "uint256") { ass = "toWei([{VAL}]);";
    } else if (type.Contains("uint")) { ass = "toLongU([{VAL}]);";
    } else if (type.Contains("int") || type.Contains("bool")) { ass = "toLong([{VAL}]);";
    } else if (type.Contains("address")) { ass = "toAddress([{VAL}]);";
    } else { ass = "[{VAL}];";
    }

    if (p->indexed) {
        ass.Replace("[{VAL}]", "nTopics > [{WHICH}] ? fromTopic(p->topics[{IDX}]) : \"\"");

    } else if (type == "bytes") {
        ass.Replace("[{VAL}]", "\"0x\"+data.substr([{WHICH}]*64)");
        which -= (nIndexed+1);

    } else {
        ass.Replace("[{VAL}]", SFString(type == "address" ? "" : "\"0x\"+") + "data.substr([{WHICH}]*64,64)");
        which -= (nIndexed+1);
    }
    ass.Replace("[{IDX}]", "++" + asStringU(which)+"++");
    ass.Replace("[{WHICH}]", asStringU(which));
    SFString fmt = "\t\t\ta->[{NAME}] = " + ass + "\n";
    return p->Format(fmt);
}

//-----------------------------------------------------------------------
//void addDefaultFuncs(CFunctionArray& funcs) {
//    CFunction func;
//    func.constant = false;
//    //  func.type = "function";
//    //  func.inputs[0].type = "string";
//    //  func.inputs[0].name = "_str";
//    //  func.name = "defFunction";
//    //  funcs[funcs.getCount()] = func;
//    func.type = "event";
//    func.name = "event";
//    func.inputs.Clear();
//    funcs[funcs.getCount()] = func;
//}

//-----------------------------------------------------------------------
const char* STR_FACTORY1 =
"\t\t} else if (encoding == func_[{LOWER}]{QB})\n"
"\t\t{\n"
"\t\t\t/""/ [{SIGNATURE}]\n"
"\t\t\t/""/ [{ENCODING}]\n"
"\t\t\t[{CLASS}] *a = new [{CLASS}];\n"
"\t\t\t*(C[{BASE}]*)a = *p; // copy in\n"
"[{ASSIGNS1}]"
"[{ITEMS1}]"
"\t\t\ta->function = \"[{NAME}]\"[{PARSEIT}];\n"
"\t\t\treturn a;\n"
"\n";

//-----------------------------------------------------------------------
const char* STR_FACTORY2 =
"\t\t} else if (fromTopic(p->topics[0]) % evt_[{LOWER}]{QB})\n"
"\t\t{\n"
"\t\t\t/""/ [{SIGNATURE}]\n"
"\t\t\t/""/ [{ENCODING}]\n"
"\t\t\t[{CLASS}] *a = new [{CLASS}];\n"
"\t\t\t*(C[{BASE}]*)a = *p; // copy in\n"
"[{ASSIGNS2}]"
"\t\t\treturn a;\n"
"\n";

//-----------------------------------------------------------------------
const char* STR_CLASSDEF =
"class:\t\t[{CLASS}]\n"
"baseClass:\tC[{BASE}]\n"
"fields:\t\t[{FIELDS}]\n"
"includes:\t[{BASE_LOWER}].h\n"
"cIncs:\t\t#include \"etherlib.h\"\n";

//-----------------------------------------------------------------------
const char* STR_HEADERFILE =
"#pragma once\n"
"/*-------------------------------------------------------------------------\n"
" * This source code is confidential proprietary information which is\n"
" * Copyright (c) 2017 by Great Hill Corporation.\n"
" * All Rights Reserved\n"
" *\n"
" * The LICENSE at the root of this repo details your rights (if any)\n"
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
"\n[{EXTERNS}][{HEADER_SIGS}]\n";

const char* STR_HEADER_SIGS =
"\n\n//-----------------------------------------------------------------------------\n"
"extern SFString sigs[];\n"
"extern SFString topics[];\n"
"\n"
"extern uint32_t nSigs;\n"
"extern uint32_t nTopics;";

const char* STR_CODE_SIGS =
"\n\n//-----------------------------------------------------------------------------\n"
"SFString sigs[] = {\n"
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
"uint32_t nSigs = sizeof(sigs) / sizeof(SFString);\n"
"\n"
"//-----------------------------------------------------------------------------\n"
"SFString topics[] = {\n"
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
"uint32_t nTopics = sizeof(topics) / sizeof(SFString);\n"
"\n";

const char* STR_BLOCK_PATH = "setStorageRoot(BLOCK_CACHE);\n\tetherlib_init(method);\n\n";
