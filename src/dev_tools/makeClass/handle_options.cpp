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
#include "options.h"

extern const char* STR_DECLARATION;
extern const char* STR_DEFAULT_ASSIGNMENT;
extern const char* STR_OPTION_CONFIGSTR;
extern const char* STR_OPTION_NOTESTR;
extern const char* STR_OPTION_STR;
extern const char* STR_AUTO_SWITCH;
extern const char* STR_AUTO_DEPRECATED;
extern const char* STR_AUTO_TOGGLE;
extern const char* STR_AUTO_FLAG;
extern const char* STR_AUTO_FLAG_ENUM;
extern const char* STR_AUTO_FLAG_ENUM_LIST;
extern const char* STR_AUTO_FLAG_STRING_LIST;
extern const char* STR_AUTO_FLAG_ADDRESS_LIST;
extern const char* STR_AUTO_FLAG_TOPIC_LIST;
extern const char* STR_AUTO_FLAG_BLOCKNUM;
extern const char* STR_AUTO_FLAG_UINT;
extern const char* STR_AUTO_FLAG_DOUBLE;
extern const char* STR_AUTO_FLAG_ADDRESS;
extern const char* STR_CHECK_BUILTIN;
extern const char* STR_BLOCKLIST_PROCESSOR;
extern const char* STR_TXLIST_PROCESSOR;
extern const char* STR_ADDRLIST_PROCESSOR;
extern const char* STR_TOPICLIST_PROCESSOR;
extern const char* STR_FOURBYTELIST_PROCESSOR;
extern const char* STR_STRINGLIST_PROCESSOR;
extern const char* STR_ENUMLIST_PROCESSOR;
extern const char* STR_ENUM_PROCESSOR;
extern const char* STR_CUSTOM_INIT;
//---------------------------------------------------------------------------------------------------
bool COptions::handle_options(void) {
    LOG_INFO(cYellow, "handling options...", string_q(50, ' '), cOff);
    counter = CCounter();  // reset

    // For each tool...
    for (auto tool : toolMap) {
        optionStream << "    // clang-format off" << endl;
        notesStream << "    // clang-format off" << endl;
        configStream << "    // clang-format off" << endl;
        CStringArray warnings;
        map<string, string> existing;
        size_t errCnt = 1;
        bool allAuto = true;

        for (auto option : cmdOptionArray) {
            option.verifyOptions(warnings);
            if ((option.group + "/" + option.tool) == tool.first) {
                option.verifyHotkey(warnings, existing);
                if (option.tool == "chifra")
                    allAuto = false;

                if (option.isDeprecated) {
                    // do nothing

                } else if (option.isNote) {
                    string_q note = option.Format(STR_OPTION_NOTESTR);
                    if (note.length() > 120)
                        note += "  // NOLINT";
                    notesStream << note << endl;

                } else if (option.isErr) {
                    string_q err = option.Format("    usageErrs[[{LONGNAME}]] = \"[{DESCRIPTION}]\";");
                    if (err.length() > 120)
                        err += "  // NOLINT";
                    errorStrStream << err << endl;
                    errorDefStream << option.Format("#define [{LONGNAME}] " + uint_2_Str(errCnt++)) << endl;

                } else if (option.isConfig) {
                    string_q config = option.Format(STR_OPTION_CONFIGSTR);
                    if (config.length() > 120)
                        config += "  // NOLINT";
                    configStream << config << endl;

                } else {
                    if (option.is_customizable) {
                        string_q config = option.Format(substitute(STR_OPTION_CONFIGSTR, "OPTS", "DESCRIPTION"));
                        if (config.length() > 120)
                            config += "  // NOLINT";
                        configStream << config << endl;
                    }
                    string_q opt = option.Format(STR_OPTION_STR);
                    replaceAll(opt, "deleteMe", "delete");
                    if (opt.length() > 120)
                        opt += "  // NOLINT";
                    optionStream << opt << endl;
                }

                string_q initFmt = "    [{LONGNAME}] = [{DEF_VAL}];";
                if (option.is_customizable) {
                    initFmt = substitute(STR_CUSTOM_INIT, "[CTYPE]",
                                         (option.isStringType() ? "String"
                                          : (option.isBool)     ? "Bool"
                                                                : "Int"));
                }

                if (option.option_type == "deprecated") {
                    generate_deprecated(option);

                } else if (option.option_type == "switch") {
                    generate_switch(option);

                } else if (option.option_type == "toggle") {
                    generate_toggle(option);

                } else if (option.option_type == "flag") {
                    generate_flag(option);

                } else if (option.option_type == "positional") {
                    generate_positional(option);
                }
            }
        }

        if (allAuto) {
            autoStream << STR_CHECK_BUILTIN << endl;
            size_t cnt = 0;
            for (auto pos : positionals) {
                autoStream << "        } else {" << endl;  // positionals live in their own frame
                if (positionals.size() > 1) {
                    if (cnt == 0) {
                        // adjust the first positional to seperate addresses or dates from blocks
                        string_q str = autoStream.str();
                        if (contains(pos, "Address"))
                            replaceReverse(str, "else {\n",
                                           "else if (isAddress(arg)) {\n"
                                           "            if (!parseAddressList(this, addrs, arg))\n"
                                           "                return false;\n"
                                           "\n");
                        else
                            replaceReverse(pos, "} else if", "if");
                        autoStream.str("");
                        autoStream.clear();
                        autoStream << str;
                        pos = "";
                    } else {
                        if (contains(pos, "Topic")) {
                            // adjust the first positional to seperate addresses or dates from blocks
                            string_q str = autoStream.str();
                            replaceReverse(str, "else {\n",
                                           "else if (isTopic(arg)) {\n"
                                           "            if (!parseTopicList2(this, topics, arg))\n"
                                           "                return false;\n"
                                           "\n");
                            autoStream.str("");
                            autoStream.clear();
                            autoStream << str;
                            pos = "";
                        } else {
                            replace(pos, "} else if", "if");
                            autoStream << pos;
                        }
                    }
                    cnt++;
                } else {
                    replace(pos, "} else if", "if");
                    autoStream << pos;
                }
            }
        }
        configStream << "    // clang-format on" << endl;
        notesStream << "    // clang-format on" << endl;
        optionStream << "    // clang-format on" << endl;

        if (warnings.size() > 0) {
            for (auto warning : warnings)
                LOG_WARN(warning);
            clearStreams();
            return false;

        } else {
            string_q fn = getSourcePath(tool.first + "/options.cpp");
            if (tool.first == "/./")
                fn = "./options.cpp";
            writeCodeOut(this, substitute(fn, ".cpp", ".h"));
            writeCodeOut(this, fn);
        }

        clearStreams();
    }

    LOG_INFO(cYellow, "makeClass --options", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    writeOpenApiFile();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::generate_toggle(const CCommandOption& option) {
    string_q initFmt = "    [{LONGNAME}] = [{DEF_VAL}];";
    if (option.is_customizable) {
        initFmt = substitute(STR_CUSTOM_INIT, "[CTYPE]",
                             (option.isStringType() ? "String"
                              : (option.isBool)     ? "Bool"
                                                    : "Int"));
    }

    if (option.generate == "local") {
        localStream << option.Format(STR_DEFAULT_ASSIGNMENT) << endl;
        if (!option.isConfig)
            autoStream << option.Format(STR_AUTO_TOGGLE) << endl;

    } else if (option.generate == "header" || option.isConfig) {
        initStream << option.Format(initFmt) << endl;
        headerStream << option.Format(STR_DECLARATION) << endl;
        if (!option.isConfig)
            autoStream << option.Format(STR_AUTO_TOGGLE) << endl;
    }
    debugStream << option.debugCode() << endl;
}

//---------------------------------------------------------------------------------------------------
void COptions::generate_switch(const CCommandOption& option) {
    string_q initFmt = "    [{LONGNAME}] = [{DEF_VAL}];";
    if (option.is_customizable) {
        initFmt = substitute(STR_CUSTOM_INIT, "[CTYPE]",
                             (option.isStringType() ? "String"
                              : (option.isBool)     ? "Bool"
                                                    : "Int"));
    }

    if (option.generate == "local") {
        localStream << option.Format(STR_DEFAULT_ASSIGNMENT) << endl;
        if (!option.isConfig)
            autoStream << option.Format(STR_AUTO_SWITCH) << endl;

    } else if (option.generate == "header" || option.isConfig) {
        initStream << option.Format(initFmt) << endl;
        headerStream << option.Format(STR_DECLARATION) << endl;
        if (!option.isConfig)
            autoStream << option.Format(STR_AUTO_SWITCH) << endl;
    }
    debugStream << option.debugCode() << endl;
}

//---------------------------------------------------------------------------------------------------
void COptions::generate_flag(const CCommandOption& option) {
    string_q initFmt = "    [{LONGNAME}] = [{DEF_VAL}];";
    if (option.is_customizable) {
        initFmt = substitute(STR_CUSTOM_INIT, "[CTYPE]",
                             (option.isStringType() ? "String"
                              : (option.isBool)     ? "Bool"
                                                    : "Int"));
    }

    if (option.generate == "local") {
        if (option.isEnumList) {
            localStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_ENUM_LIST) << endl;
        } else if (option.isStringList) {
            localStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_STRING_LIST) << endl;
        } else if (option.isTopicList) {
            localStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_TOPIC_LIST) << endl;
        } else if (option.isAddressList) {
            localStream << option.Format("    CAddressArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_ADDRESS_LIST) << endl;
        } else {
            localStream << option.Format(STR_DEFAULT_ASSIGNMENT) << endl;
            if (!option.isConfig) {
                if (option.isEnum)
                    autoStream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                else if (option.isBlockNum)
                    autoStream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                else if (option.isUint32 || option.isUint64)
                    autoStream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                else if (option.isDouble)
                    autoStream << option.Format(STR_AUTO_FLAG_DOUBLE) << endl;
                else if (option.isAddress)
                    autoStream << option.Format(STR_AUTO_FLAG_ADDRESS) << endl;
                else
                    autoStream << substitute(option.Format(STR_AUTO_FLAG), "substitute(substitute(arg, \"-:\", )",
                                             "substitute(arg")
                               << endl;
            }
        }

    } else if (option.generate == "header" || option.isConfig) {
        if (option.isEnumList) {
            initStream << option.Format("    [{LONGNAME}].clear();") << endl;
            headerStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_ENUM_LIST) << endl;
        } else if (option.isStringList) {
            initStream << option.Format("    [{LONGNAME}].clear();") << endl;
            headerStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_STRING_LIST) << endl;
        } else if (option.isTopicList) {
            initStream << option.Format("    [{LONGNAME}].clear();") << endl;
            headerStream << option.Format("    CTopicArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_TOPIC_LIST) << endl;
        } else if (option.isAddressList) {
            initStream << option.Format("    [{LONGNAME}].clear();") << endl;
            headerStream << option.Format("    CAddressArray [{LONGNAME}];") << endl;
            if (!option.isConfig)
                autoStream << option.Format(STR_AUTO_FLAG_ADDRESS_LIST) << endl;
        } else {
            initStream << option.Format(initFmt) << endl;
            headerStream << option.Format(STR_DECLARATION) << endl;
            if (!option.isConfig) {
                if (option.isEnum)
                    autoStream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                else if (option.isBlockNum)
                    autoStream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                else if (option.isUint32 || option.isUint64)
                    autoStream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                else if (option.isDouble)
                    autoStream << option.Format(STR_AUTO_FLAG_DOUBLE) << endl;
                else if (option.isAddress)
                    autoStream << option.Format(STR_AUTO_FLAG_ADDRESS) << endl;
                else
                    autoStream << substitute(option.Format(STR_AUTO_FLAG), "substitute(substitute(arg, \"-:\", )",
                                             "substitute(arg")
                               << endl;
            }
        }
    }
    debugStream << option.debugCode() << endl;
}

//---------------------------------------------------------------------------------------------------
void COptions::generate_positional(const CCommandOption& option) {
    ostringstream posStream;
    if (option.generate == "local") {
        if (option.data_type == "list<addr>") {
            localStream << substitute(option.Format("    CAddressArray [{LONGNAME}];"), "addrs2", "addrs") << endl;
            posStream << option.Format(STR_ADDRLIST_PROCESSOR) << endl;

        } else if (option.data_type == "list<topic>") {
            localStream << option.Format("    CTopicArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_TOPICLIST_PROCESSOR) << endl;

        } else if (option.data_type == "list<fourbyte>") {
            localStream << option.Format("    CFourbyteArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_FOURBYTELIST_PROCESSOR) << endl;

        } else if (startsWith(option.data_type, "list<enum[")) {
            localStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_ENUMLIST_PROCESSOR) << endl;

        } else if (startsWith(option.data_type, "enum[")) {
            headerStream << option.Format("    string_q [{LONGNAME}];") << endl;
            posStream << option.Format(STR_ENUM_PROCESSOR) << endl;

        } else if (option.data_type == "list<string>" || option.data_type == "list<path>") {
            localStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_STRINGLIST_PROCESSOR) << endl;

        } else if (option.data_type == "list<blknum>") {
            // localStream << "    CStringArray strings;" << endl;
            posStream << STR_BLOCKLIST_PROCESSOR << endl;

        } else if (option.data_type == "list<date>") {
            // localStream << "    CStringArray strings;" << endl;
            posStream << STR_AUTO_FLAG_ENUM_LIST << endl;

        } else if (option.data_type == "list<tx_id>") {
            // localStream << "    CStringArray strings;" << endl;
            posStream << STR_TXLIST_PROCESSOR << endl;

        } else {
            // don't know type
        }

    } else if (option.generate == "header" || option.isConfig) {
        if (option.data_type == "list<addr>") {
            headerStream << substitute(option.Format("    CAddressArray [{LONGNAME}];"), "addrs2", "addrs") << endl;
            posStream << option.Format(STR_ADDRLIST_PROCESSOR) << endl;

        } else if (option.data_type == "list<topic>") {
            headerStream << option.Format("    CTopicArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_TOPICLIST_PROCESSOR) << endl;

        } else if (option.data_type == "list<fourbyte>") {
            headerStream << option.Format("    CFourbyteArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_FOURBYTELIST_PROCESSOR) << endl;

        } else if (startsWith(option.data_type, "list<enum[")) {
            headerStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_ENUMLIST_PROCESSOR) << endl;

        } else if (startsWith(option.data_type, "enum[")) {
            headerStream << option.Format("    string_q [{LONGNAME}];") << endl;
            posStream << option.Format(STR_ENUM_PROCESSOR) << endl;

        } else if (option.data_type == "list<string>" || option.data_type == "list<path>") {
            headerStream << option.Format("    CStringArray [{LONGNAME}];") << endl;
            posStream << option.Format(STR_STRINGLIST_PROCESSOR) << endl;

        } else if (option.data_type == "list<blknum>") {
            // headerStream << "    CStringArray strings;" << endl;
            posStream << STR_BLOCKLIST_PROCESSOR << endl;

        } else if (option.data_type == "list<date>") {
            // headerStream << "    CStringArray strings;" << endl;

        } else if (option.data_type == "list<tx_id>") {
            // headerStream << "    CStringArray strings;" << endl;
            posStream << STR_TXLIST_PROCESSOR << endl;

        } else {
            // unknown type
        }
    }
    if (!posStream.str().empty())
        positionals.push_back(posStream.str());

    debugStream << option.debugCode() << endl;
}

//---------------------------------------------------------------------------------------------------
void COptions::generate_deprecated(const CCommandOption& option) {
    if (option.isDeprecated) {
        autoStream << option.Format(STR_AUTO_DEPRECATED) << endl;
    }
}

//---------------------------------------------------------------------------------------------------
const char* STR_OPTION_CONFIGSTR = "    configs.push_back(\"`[{LONGNAME}]`: [{OPTS}].\");";

//---------------------------------------------------------------------------------------------------
const char* STR_OPTION_NOTESTR = "    notes.push_back(\"[{OPTS}]\");";

//---------------------------------------------------------------------------------------------------
const char* STR_OPTION_STR =
    "    COption(\"[{LONGNAME}]\", \"[{HOTKEY}]\", \"[{DATATYPE}]\", [{OPTS}], \"[{DESCRIPTION}]\"),";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_SWITCH =
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            [{LONGNAME}] = true;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_DEPRECATED =
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            // clang-format off\n"
    "            return usage(\"the --[{LONGNAME}] option is deprecated, [{DESCRIPTION}]\");  // NOLINT\n"
    "            // clang-format on\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_TOGGLE =
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            [{LONGNAME}] = ![{LONGNAME}];\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            [{LONGNAME}] = substitute(substitute(arg, \"-[{HOTKEY}]:\", \"\"), \"--[{LONGNAME}]:\", \"\");\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_ENUM =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            if (!confirmEnum(\"[{LONGNAME}]\", [{LONGNAME}], arg))\n"
    "                return false;\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_ENUM_LIST =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            string_q [{LONGNAME}_tmp];\n"
    "            if (!confirmEnum(\"[{LONGNAME}]\", [{LONGNAME}]_tmp, arg))\n"
    "                return false;\n"
    "            [{LONGNAME}].push_back([{LONGNAME}]_tmp);\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_STRING_LIST =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            arg = substitute(substitute(arg, \"-[{HOTKEY}]:\", \"\"), \"--[{LONGNAME}]:\", \"\");\n"
    "            [{LONGNAME}].push_back(arg);\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_ADDRESS_LIST =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            arg = substitute(substitute(arg, \"-[{HOTKEY}]:\", \"\"), \"--[{LONGNAME}]:\", \"\");\n"
    "            if (!parseAddressList(this, [{LONGNAME}], arg))\n"
    "                return false;\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_TOPIC_LIST =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            arg = substitute(substitute(arg, \"-[{HOTKEY}]:\", \"\"), \"--[{LONGNAME}]:\", \"\");\n"
    "            if (!parseTopicList2(this, [{LONGNAME}], arg))\n"
    "                return false;\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_BLOCKNUM =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            if (!confirmBlockNum(\"[{LONGNAME}]\", [{LONGNAME}], arg, latest))\n"
    "                return false;\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_ADDRESS =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            [{LONGNAME}] = substitute(substitute(arg, \"-[{HOTKEY}]:\", \"\"), \"--[{LONGNAME}]:\", \"\");\n"
    "            if (!isAddress([{LONGNAME}]))\n"
    "                return usage(\"The provided value (\" + [{LONGNAME}] + \") is not a properly formatted "
    "address.\");\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_UINT =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            if (!confirmUint(\"[{LONGNAME}]\", [{LONGNAME}], arg))\n"
    "                return false;\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_DOUBLE =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{LONGNAME}]:\")) {\n"
    "            if (!confirmDouble(\"[{LONGNAME}]\", [{LONGNAME}], arg))\n"
    "                return false;\n"
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{LONGNAME}]\") {\n"
    "            return flag_required(\"[{LONGNAME}]\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_CHECK_BUILTIN =
    "        } else if (startsWith(arg, '-')) {  // do not collapse\n"
    "\n"
    "            if (!builtInCmd(arg)) {\n"
    "                return invalid_option(arg);\n"
    "            }\n";

//---------------------------------------------------------------------------------------------------
const char* STR_BLOCKLIST_PROCESSOR =
    "            } else if (!parseBlockList2(this, blocks, arg, latest))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TXLIST_PROCESSOR =
    "            } else if (!parseTransList2(this, transList, arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_ADDRLIST_PROCESSOR =
    "            } else if (!parseAddressList(this, [{LONGNAME}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TOPICLIST_PROCESSOR =
    "            } else if (!parseTopicList2(this, [{LONGNAME}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_FOURBYTELIST_PROCESSOR =
    "            } else if (!parseFourbyteList(this, [{LONGNAME}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_STRINGLIST_PROCESSOR =
    "            } else if (!parseStringList2(this, [{LONGNAME}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_ENUMLIST_PROCESSOR =
    "            string_q [{LONGNAME}_tmp];\n"
    "            if (!confirmEnum(\"[{LONGNAME}]\", [{LONGNAME}]_tmp, arg))\n"
    "                return false;\n"
    "            [{LONGNAME}].push_back([{LONGNAME}]_tmp);\n";

//---------------------------------------------------------------------------------------------------
const char* STR_ENUM_PROCESSOR =
    "            if (![{LONGNAME}].empty())\n"
    "                return usage(\"Please specify only one [{LONGNAME}].\");\n"
    "            if (!confirmEnum(\"[{LONGNAME}]\", [{LONGNAME}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_CUSTOM_INIT =
    "    // clang-format off\n"
    "    [{LONGNAME}] = getGlobalConfig(\"[{TOOL}]\")->getConfig[CTYPE](\"settings\", \"[{LONGNAME}]\", "
    "[{DEF_VAL}]);\n"
    "    // clang-format on";

//---------------------------------------------------------------------------------------------------
const char* STR_DEFAULT_ASSIGNMENT = "    [{REAL_TYPE}] [{LONGNAME}] = [{DEF_VAL}];";

//---------------------------------------------------------------------------------------------------
const char* STR_DECLARATION = "    [{REAL_TYPE}] [{LONGNAME}];";
