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
#include "options.h"
#include "commandoption.h"

extern const char* STR_OPTION_STR;
extern const char* STR_AUTO_SWITCH;
extern const char* STR_AUTO_TOGGLE;
extern const char* STR_AUTO_FLAG;
extern const char* STR_AUTO_FLAG_ENUM;
extern const char* STR_AUTO_FLAG_ENUM_LIST;
extern const char* STR_AUTO_FLAG_BLOCKNUM;
extern const char* STR_AUTO_FLAG_UINT;
extern const char* STR_CHECK_BUILTIN;
extern const char* STR_BLOCK_PROCESSOR;
extern const char* STR_TX_PROCESSOR;
extern const char* STR_ADDR_PROCESSOR;
extern const char* STR_STRING_PROCESSOR;
extern const char* STR_ENUM_PROCESSOR;
extern const char* STR_CUSTOM_INIT;
//---------------------------------------------------------------------------------------------------
bool COptions::handle_options(void) {
    LOG_INFO(cYellow, "handling options...", cOff);
    CCommandOption::registerClass();
    counter = CCounter();  // reset

    string_q contents = asciiFileToString("../src/other/build_assets/option-master-list.csv");

    CStringArray lines;
    explode(lines, contents, '\n');

    map<string, bool> tools;
    CCommandOptionArray optionArray;
    for (auto line : lines) {
        CCommandOption optDef(line);
        if (!optDef.tool.empty() && optDef.tool != "all" && optDef.tool != "tool" && optDef.tool != "templates") {
            optionArray.push_back(optDef);
            tools[optDef.group + "/" + optDef.tool] = true;
        }
    }

    for (auto tool : tools) {
        warnings.clear();
        warnings.str("");

        map<string, string> shortCmds;
        ostringstream option_stream, init_stream, local_stream, auto_stream;
        ostringstream declare_stream, notes_stream, errors_stream;
        option_stream << "    // clang-format off" << endl;
        notes_stream << "    // clang-format off" << endl;
        CStringArray positionals;
        bool allAuto = true;
        for (auto option : optionArray) {
            if ((option.group + "/" + option.tool) == tool.first) {
                check_option(option);

                if (option.tool == "chifra")
                    allAuto = false;

                bool isEnumList = contains(option.data_type, "list<enum");
                bool isEnum = contains(option.data_type, "enum") && !isEnumList;
                bool isBool = contains(option.data_type, "boolean");
                bool isBlockNum = contains(option.data_type, "blknum");
                bool isUint32 = contains(option.data_type, "uint32");
                bool isUint64 = contains(option.data_type, "uint64");

                bool isNote = option.option_kind == "note";
                bool isError = option.option_kind == "error";
                if (isNote) {
                    string_q note = option.Format("    notes.push_back(\"[{OPTS}]\");");
                    if (note.length() > 120)
                        note += "  // NOLINT";
                    notes_stream << note << endl;
                } else if (isError) {
                    string_q err = option.Format("    errorStrs[[{COMMAND]] = \"[{OPTS}]\";");
                    if (err.length() > 120)
                        err += "  // NOLINT";
                    errors_stream << err << endl;
                } else {
                    string_q opt = option.Format(STR_OPTION_STR);
                    if (opt.length() > 120)
                        opt += "  // NOLINT";
                    option_stream << opt << endl;
                }

                if (!option.generate.empty()) {
                    string_q type = substituteAny(substitute(option.data_type, "boolean", "bool"), "<>", "");
                    if (contains(option.data_type, "enum"))
                        type = "string";
                    replace(type, "blknum", "blknum_t");
                    replace(type, "string", "string_q");
                    replace(type, "uint32", "uint32_t");
                    replace(type, "uint64", "uint64_t");

                    if (option.generate == "local") {
                        if (option.option_kind == "switch") {
                            local_stream << option.Format("    " + type + " [{COMMAND}] = [{DEF_VAL}];") << endl;
                            auto_stream << option.Format(STR_AUTO_SWITCH) << endl;

                        } else if (option.option_kind == "toggle") {
                            local_stream << option.Format("    " + type + " [{COMMAND}] = [{DEF_VAL}];") << endl;
                            auto_stream << option.Format(STR_AUTO_TOGGLE) << endl;

                        } else if (option.option_kind == "flag") {
                            if (isEnumList) {
                                local_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                auto_stream << option.Format(STR_AUTO_FLAG_ENUM_LIST) << endl;

                            } else {
                                local_stream << option.Format("    " + type + " [{COMMAND}] = [{DEF_VAL}];") << endl;
                                if (isEnum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                                else if (isBlockNum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                                else if (isUint32 || isUint64)
                                    auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                                else
                                    auto_stream << substitute(option.Format(STR_AUTO_FLAG),
                                                              "substitute(substitute(arg, \"-:\", )", "substitute(arg")
                                                << endl;
                            }

                        } else if (option.option_kind == "skipN") {
                            local_stream << option.Format("    " + type + " [{COMMAND}] = [{DEF_VAL}];") << endl;
                            auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;

                        } else if (option.option_kind == "positional") {
                            ostringstream pos_stream;
                            if (option.data_type == "list<addr>") {
                                local_stream
                                    << substitute(option.Format("    CAddressArray [{COMMAND}];"), "addrs2", "addrs")
                                    << endl;
                                pos_stream << option.Format(STR_ADDR_PROCESSOR) << endl;
                            } else if (startsWith(option.data_type, "list<enum[")) {
                                local_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                pos_stream << option.Format(STR_ENUM_PROCESSOR) << endl;
                            } else if (option.data_type == "list<string>" || option.data_type == "list<path>") {
                                local_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                pos_stream << option.Format(STR_STRING_PROCESSOR) << endl;

                            } else if (option.data_type == "list<blknum>") {
                                // local_stream << "    CStringArray strings;" << endl;
                                pos_stream << STR_BLOCK_PROCESSOR << endl;
                            } else if (option.data_type == "list<date>") {
                                // local_stream << "    CStringArray strings;" << endl;
                                pos_stream << STR_AUTO_FLAG_ENUM_LIST << endl;
                            } else if (option.data_type == "list<tx_id>") {
                                // local_stream << "    CStringArray strings;" << endl;
                                pos_stream << STR_TX_PROCESSOR << endl;
                            } else {
                                // don't know type
                            }
                            if (!pos_stream.str().empty())
                                positionals.push_back(pos_stream.str());

                        } else {
                            cerr << cRed << "skipping option_kind " << option.option_kind << cOff << endl;
                            exit(0);
                        }

                    } else if (option.generate == "header") {
                        string_q initFmt = "    [{COMMAND}] = [{DEF_VAL}];";
                        if (option.is_customizable % "true")
                            initFmt = substitute(STR_CUSTOM_INIT, "[CTYPE]",
                                                 ((isEnum || isEnumList) ? "String" : (isBool) ? "Bool" : "Int"));

                        if (option.option_kind == "switch") {
                            init_stream << option.Format(initFmt) << endl;
                            declare_stream << option.Format("    " + type + " [{COMMAND}];") << endl;
                            auto_stream << option.Format(STR_AUTO_SWITCH) << endl;

                        } else if (option.option_kind == "toggle") {
                            init_stream << option.Format(initFmt) << endl;
                            declare_stream << option.Format("    " + type + " [{COMMAND}];") << endl;
                            auto_stream << option.Format(STR_AUTO_TOGGLE) << endl;

                        } else if (option.option_kind == "flag") {
                            if (isEnumList) {
                                init_stream << option.Format("    [{COMMAND}].clear();") << endl;
                                declare_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                auto_stream << option.Format(STR_AUTO_FLAG_ENUM_LIST) << endl;

                            } else {
                                init_stream << option.Format(initFmt) << endl;
                                declare_stream << option.Format("    " + type + " [{COMMAND}];") << endl;
                                if (isEnum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                                else if (isBlockNum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                                else if (isUint32 || isUint64)
                                    auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                                else
                                    auto_stream << substitute(option.Format(STR_AUTO_FLAG),
                                                              "substitute(substitute(arg, \"-:\", )", "substitute(arg")
                                                << endl;
                            }

                        } else if (option.option_kind == "positional") {
                            ostringstream pos_stream;
                            if (option.data_type == "list<addr>") {
                                declare_stream
                                    << substitute(option.Format("    CAddressArray [{COMMAND}];"), "addrs2", "addrs")
                                    << endl;
                                pos_stream << option.Format(STR_ADDR_PROCESSOR) << endl;
                            } else if (startsWith(option.data_type, "list<enum[")) {
                                declare_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                pos_stream << option.Format(STR_ENUM_PROCESSOR) << endl;
                            } else if (option.data_type == "list<string>" || option.data_type == "list<path>") {
                                declare_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                pos_stream << option.Format(STR_STRING_PROCESSOR) << endl;

                            } else if (option.data_type == "list<blknum>") {
                                // declare_stream << "    CStringArray strings;" << endl;
                                pos_stream << STR_BLOCK_PROCESSOR << endl;
                            } else if (option.data_type == "list<date>") {
                                // declare_stream << "    CStringArray strings;" << endl;
                            } else if (option.data_type == "list<tx_id>") {
                                // declare_stream << "    CStringArray strings;" << endl;
                                pos_stream << STR_TX_PROCESSOR << endl;
                            } else {
                                // unknown type
                            }
                            if (!pos_stream.str().empty())
                                positionals.push_back(pos_stream.str());

                        } else {
                            cerr << "skipping option_kind " << option.option_kind << endl;
                        }

                    } else {
                        // do nothing (ignore 'na' for example)
                    }

                } else {
                    if (option.option_kind == "toggle" || option.option_kind == "switch" ||
                        option.option_kind == "flag")
                        allAuto = false;
                }

                if (!option.hotkey.empty() && !contains(option.option_kind, "positional") &&
                    !contains(option.option_kind, "description") && !contains(option.option_kind, "note") &&
                    !contains(option.option_kind, "error")) {
                    if (option.hotkey == "v")
                        warnings << option.tool << ":hotkey '" << cRed << option.command << "-" << option.hotkey << cOff
                                 << "' conflicts with --verbose hotkey|";
                    if (option.hotkey == "h")
                        warnings << option.tool << ":hotkey '" << cRed << option.command << "-" << option.hotkey << cOff
                                 << "' conflicts with --help hotkey|";
                    if (option.hotkey == "x")
                        warnings << option.tool << ":hotkey '" << cRed << option.command << "-" << option.hotkey << cOff
                                 << "' conflicts with --fmt hotkey|";
                    if (!shortCmds[option.hotkey].empty())
                        warnings << "Hotkey '" << cRed << option.command << "-" << option.hotkey << cOff
                                 << "' conflicts with existing '" << cRed << shortCmds[option.hotkey] << cOff << "'|";
                    shortCmds[option.hotkey] = option.command + "-" + option.hotkey;
                    bool isUpper = (toLower(option.hotkey) != option.hotkey);
                    bool isFirst = option.hotkey == option.command.substr(0, 1);
                    bool isSecond = option.hotkey == option.command.substr(1, 1);
                    bool isContained = !verbose && contains(option.command, option.hotkey);
                    if (!isFirst && !isSecond && !isUpper && !isContained) {
                        warnings << "Hotkey '" << cRed << option.hotkey << "' " << cOff;
                        warnings << "of command '" << cRed << option.command << cOff
                                 << "' is not first or second character|";
                    }
                }
            }
        }

        if (allAuto) {
            auto_stream << STR_CHECK_BUILTIN << endl;
            size_t cnt = 0;
            for (auto pos : positionals) {
                auto_stream << "        } else {" << endl;  // positionals live in their own frame
                if (positionals.size() > 1) {
                    if (cnt == 0) {
                        // adjust the first positional to seperate addresses or dates from blocks
                        string_q str = auto_stream.str();
                        if (contains(pos, "Address"))
                            replaceReverse(str, "else {\n",
                                           "else if (isAddress(arg)) {\n            if (!parseAddressList2(this, "
                                           "addrs, arg))\n                return false;\n\n");
                        else
                            replaceReverse(pos, "} else if", "if");
                        auto_stream.str("");
                        auto_stream.clear();
                        auto_stream << str;
                        pos = "";
                    } else {
                        replace(pos, "} else if", "if");
                        auto_stream << pos;
                    }
                    cnt++;
                } else {
                    replace(pos, "} else if", "if");
                    auto_stream << pos;
                }
            }
        }
        option_stream << "    // clang-format on" << endl;
        notes_stream << "    // clang-format on" << endl;

        if (!warnings.str().empty()) {
            CStringArray w;
            explode(w, warnings.str(), '|');
            for (auto warning : w)
                LOG_WARN(warning);
        } else {
            string_q fn = "../src/" + tool.first + "/options.cpp";
            if (!test) {
                writeCode(fn, auto_stream.str(), option_stream.str(), local_stream.str(), init_stream.str(),
                          notes_stream.str(), errors_stream.str());
                writeCode(substitute(fn, ".cpp", ".h"), declare_stream.str());
            }
        }
    }

    if (test) {
        counter.nProcessed = 0;
        LOG_WARN("Testing only - no files written");
    }
    LOG_INFO(cYellow, "makeClass --options", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    return true;
}

namespace qblocks {
extern string_q getReservedCommands(void);
}

//---------------------------------------------------------------------------------------------------
bool COptions::check_option(const CCommandOption& option) {
    // Check valid data types
    CStringArray validTypes = {
        "<addr>", "<blknum>", "<pair>", "<path>", "<range>", "<string>", "<uint32>", "<uint64>", "<boolean>", "<path>",
    };

    bool valid_type = false;
    for (auto type : validTypes) {
        if (type == option.data_type) {
            valid_type = true;
        }
    }
    if (!valid_type) {
        if (startsWith(option.data_type, "enum"))
            valid_type = true;
        if (startsWith(option.data_type, "list"))
            valid_type = true;
    }
    if (!valid_type &&
        (option.option_kind == "description" || option.option_kind == "note" || option.option_kind == "error") &&
        option.data_type.empty())
        valid_type = true;

    if (!valid_type)
        warnings << "Unknown type '" << cRed << option.data_type << cOff << "' for option '" << cRed << option.command
                 << cOff << "'|";

    if (option.option_kind == "description" && !endsWith(option.description, ".") && !endsWith(option.description, ":"))
        warnings << "Description '" << cRed << option.description << cOff << "' should end with a period or colon.|";
    if (option.option_kind == "note" && !endsWith(option.description, ".") && !endsWith(option.description, ":"))
        warnings << "Note '" << cRed << option.description << cOff << "' should end with a period or colon.|";
    if (option.option_kind == "error" && !endsWith(option.description, ".") && !endsWith(option.description, ":"))
        warnings << "Error string '" << cRed << option.description << cOff << "' should end with a period or colon.|";

    if ((option.option_kind != "description" && option.option_kind != "note" && option.option_kind != "error") &&
        endsWith(option.description, "."))
        warnings << "Option '" << cRed << option.description << cOff << "' should not end with a period.|";

    string_q reserved = "|" + getReservedCommands() + "|";
    if (contains(reserved, "|" + option.command + "|"))
        warnings << "Option '" << cRed << option.command << cOff << "' is a reserved word.|";

    return true;
}

//---------------------------------------------------------------------------------------------------
string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code) {
    string_q converted = orig;
    converted = substitute(converted, "// BEG_" + which, "// BEG_" + which + "\n[{NEW_CODE}]\n<remove>");
    converted = substitute(converted, "\n    // END_" + which, "</remove>\n+// END_" + which);
    converted = substitute(converted, "\n            // END_" + which, "</remove>\n-// END_" + which);
    snagFieldClear(converted, "remove");
    replace(converted, "[{NEW_CODE}]\n\n", new_code);
    replaceAll(converted, "+//", "    //");
    replaceAll(converted, "-//", "            //");
    return converted;
}
//---------------------------------------------------------------------------------------------------
bool COptions::writeCode(const string_q& fn, const string_q& code, const string_q& opt, const string_q& local,
                         const string_q& init, const string_q& notes, const string_q& errors) {
    string_q orig = asciiFileToString(fn);
    string_q converted = orig;
    if (endsWith(fn, ".cpp")) {
        converted = replaceCode(converted, "CODE_AUTO", code);
        converted = replaceCode(converted, "CODE_OPTIONS", opt);
        converted = replaceCode(converted, "CODE_LOCAL_INIT", local);
        converted = replaceCode(converted, "CODE_INIT", init);
        converted = replaceCode(converted, "CODE_NOTES", notes);
        converted = replaceCode(converted, "CODE_ERROR_MSG", errors);
    } else {
        converted = replaceCode(converted, "CODE_DECLARE", code);
    }
    cerr << bBlue << "Processing " << cOff << fn << " ";
    counter.nVisited++;
    if (converted != orig) {
        cerr << cGreen << "wrote " << converted.size() << " bytes..." << cOff << endl;
        stringToAsciiFile(fn, converted);
        counter.nProcessed++;
        return true;
    }
    cerr << cTeal << "no changes..." << cOff << "\r";
    cerr.flush();
    return false;
}

//---------------------------------------------------------------------------------------------------
const char* STR_OPTION_STR =
    "    COption(\"[{COMMAND}]\", \"[{HOTKEY}]\", \"[{DATATYPE}]\", [{OPTS}], \"[{DESCRIPTION}]\"),";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_SWITCH =
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{COMMAND}]\") {\n"
    "            [{COMMAND}] = true;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_TOGGLE =
    "        } else if ([arg == \"-{HOTKEY}\" || ]arg == \"--[{COMMAND}]\") {\n"
    "            [{COMMAND}] = ![{COMMAND}];\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
    "            [{COMMAND}] = substitute(substitute(arg, \"-[{HOTKEY}]:\", \"\"), \"--[{COMMAND}]:\", \"\");\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_ENUM =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
    "            if (!confirmEnum(\"[{COMMAND}]\", [{COMMAND}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_ENUM_LIST =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
    "            string_q [{COMMAND}_tmp];\n"
    "            if (!confirmEnum(\"[{COMMAND}]\", [{COMMAND}]_tmp, arg))\n"
    "                return false;\n"
    "            [{COMMAND}].push_back([{COMMAND}]_tmp);\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_BLOCKNUM =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
    "            if (!confirmBlockNum(\"[{COMMAND}]\", [{COMMAND}], arg, latest))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG_UINT =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
    "            if (!confirmUint(\"[{COMMAND}]\", [{COMMAND}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_CHECK_BUILTIN =
    "        } else if (startsWith(arg, '-')) {  // do not collapse\n"
    "\n"
    "            if (!builtInCmd(arg)) {\n"
    "                return usage(\"Invalid option: \" + arg);\n"
    "            }\n";

//---------------------------------------------------------------------------------------------------
const char* STR_BLOCK_PROCESSOR =
    "            } else if (!parseBlockList2(this, blocks, arg, latest))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TX_PROCESSOR =
    "            } else if (!parseTransList2(this, transList, arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_ADDR_PROCESSOR =
    "            } else if (!parseAddressList2(this, [{COMMAND}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_STRING_PROCESSOR =
    "            } else if (!parseStringList2(this, [{COMMAND}], arg))\n"
    "                return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_ENUM_PROCESSOR =
    "            string_q [{COMMAND}_tmp];\n"
    "            if (!confirmEnum(\"[{COMMAND}]\", [{COMMAND}]_tmp, arg))\n"
    "                return false;\n"
    "            [{COMMAND}].push_back([{COMMAND}]_tmp);\n";

//---------------------------------------------------------------------------------------------------
const char* STR_CUSTOM_INIT =
    "    [{COMMAND}] = getGlobalConfig(\"[{TOOL}]\")->getConfig[CTYPE](\"settings\", \"[{COMMAND}]\", [{DEF_VAL}]);";
