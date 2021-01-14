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

extern const char* STR_OPTION_STR;
extern const char* STR_AUTO_SWITCH;
extern const char* STR_AUTO_TOGGLE;
extern const char* STR_AUTO_FLAG;
extern const char* STR_AUTO_FLAG_ENUM;
extern const char* STR_AUTO_FLAG_ENUM_LIST;
extern const char* STR_AUTO_FLAG_BLOCKNUM;
extern const char* STR_AUTO_FLAG_UINT;
extern const char* STR_AUTO_FLAG_DOUBLE;
extern const char* STR_CHECK_BUILTIN;
extern const char* STR_BLOCK_PROCESSOR;
extern const char* STR_TX_PROCESSOR;
extern const char* STR_ADDR_PROCESSOR;
extern const char* STR_TOPIC_PROCESSOR;
extern const char* STR_STRING_PROCESSOR;
extern const char* STR_ENUM_PROCESSOR;
extern const char* STR_CUSTOM_INIT;
//---------------------------------------------------------------------------------------------------
bool COptions::handle_options(void) {
    LOG_INFO(cYellow, "handling options...", cOff);
    counter = CCounter();  // reset

    // read in and prepare the options for all tools
    map<string, bool> tools;
    string_q contents = asciiFileToString("../src/cmd-line-options.csv");
    CStringArray lines;
    explode(lines, contents, '\n');
    for (auto line : lines) {
        CCommandOption opt(line);
        if (!opt.tool.empty() && opt.tool != "all" && opt.tool != "tool" && opt.tool != "templates") {
            optionArray.push_back(opt);
            tools[opt.group + "/" + opt.tool] = true;
        }
    }

    // For each tool...
    for (auto tool : tools) {
        CStringArray positionals;
        bool allAuto = true;

        option_stream << "    // clang-format off" << endl;
        notes_stream << "    // clang-format off" << endl;
        CStringArray warnings;
        for (auto option : optionArray) {
            option.verifyOptions(warnings);
            option.verifyHotkey(warnings);
            if ((option.group + "/" + option.tool) == tool.first) {
                if (option.tool == "chifra")
                    allAuto = false;

                if (option.isNote) {
                    string_q note = option.Format("    notes.push_back(\"[{OPTS}]\");");
                    if (note.length() > 120)
                        note += "  // NOLINT";
                    notes_stream << note << endl;

                } else if (option.isError) {
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
                    if (option.generate == "local") {
                        if (option.option_kind == "switch") {
                            local_stream << option.Format("    " + option.real_type + " [{COMMAND}] = [{DEF_VAL}];")
                                         << endl;
                            auto_stream << option.Format(STR_AUTO_SWITCH) << endl;

                        } else if (option.option_kind == "toggle") {
                            local_stream << option.Format("    " + option.real_type + " [{COMMAND}] = [{DEF_VAL}];")
                                         << endl;
                            auto_stream << option.Format(STR_AUTO_TOGGLE) << endl;

                        } else if (option.option_kind == "flag") {
                            if (option.isEnumList) {
                                local_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                auto_stream << option.Format(STR_AUTO_FLAG_ENUM_LIST) << endl;

                            } else {
                                local_stream << option.Format("    " + option.real_type + " [{COMMAND}] = [{DEF_VAL}];")
                                             << endl;
                                if (option.isEnum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                                else if (option.isBlockNum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                                else if (option.isUint32 || option.isUint64)
                                    auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                                else if (option.isDouble)
                                    auto_stream << option.Format(STR_AUTO_FLAG_DOUBLE) << endl;
                                else
                                    auto_stream << substitute(option.Format(STR_AUTO_FLAG),
                                                              "substitute(substitute(arg, \"-:\", )", "substitute(arg")
                                                << endl;
                            }

                        } else if (option.option_kind == "skipN") {
                            local_stream << option.Format("    " + option.real_type + " [{COMMAND}] = [{DEF_VAL}];")
                                         << endl;
                            auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;

                        } else if (option.option_kind == "positional") {
                            ostringstream pos_stream;
                            if (option.data_type == "list<addr>") {
                                local_stream
                                    << substitute(option.Format("    CAddressArray [{COMMAND}];"), "addrs2", "addrs")
                                    << endl;
                                pos_stream << option.Format(STR_ADDR_PROCESSOR) << endl;
                            } else if (option.data_type == "list<topic>") {
                                local_stream << option.Format("    CTopicArray [{COMMAND}];") << endl;
                                pos_stream << option.Format(STR_TOPIC_PROCESSOR) << endl;
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
                            initFmt = substitute(
                                STR_CUSTOM_INIT, "[CTYPE]",
                                ((option.isEnum || option.isEnumList) ? "String" : (option.isBool) ? "Bool" : "Int"));

                        if (option.option_kind == "switch") {
                            init_stream << option.Format(initFmt) << endl;
                            declare_stream << option.Format("    " + option.real_type + " [{COMMAND}];") << endl;
                            auto_stream << option.Format(STR_AUTO_SWITCH) << endl;

                        } else if (option.option_kind == "toggle") {
                            init_stream << option.Format(initFmt) << endl;
                            declare_stream << option.Format("    " + option.real_type + " [{COMMAND}];") << endl;
                            auto_stream << option.Format(STR_AUTO_TOGGLE) << endl;

                        } else if (option.option_kind == "flag") {
                            if (option.isEnumList) {
                                init_stream << option.Format("    [{COMMAND}].clear();") << endl;
                                declare_stream << option.Format("    CStringArray [{COMMAND}];") << endl;
                                auto_stream << option.Format(STR_AUTO_FLAG_ENUM_LIST) << endl;

                            } else {
                                init_stream << option.Format(initFmt) << endl;
                                declare_stream << option.Format("    " + option.real_type + " [{COMMAND}];") << endl;
                                if (option.isEnum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                                else if (option.isBlockNum)
                                    auto_stream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                                else if (option.isUint32 || option.isUint64)
                                    auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                                else if (option.isDouble)
                                    auto_stream << option.Format(STR_AUTO_FLAG_DOUBLE) << endl;
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
                            } else if (option.data_type == "list<topic>") {
                                declare_stream << option.Format("    CTopicArray [{COMMAND}];") << endl;
                                pos_stream << option.Format(STR_TOPIC_PROCESSOR) << endl;
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
        notes_stream << "    // clang-format on" << endl;
        option_stream << "    // clang-format on" << endl;

        if (warnings.size() > 0) {
            for (auto warning : warnings)
                LOG_WARN(warning);
        } else {
            string_q fn = "../src/" + tool.first + "/options.cpp";
            if (!test) {
                writeCode(substitute(fn, ".cpp", ".h"));
                writeCode(fn);
            }
        }
        clearStreams();
    }
    if (test) {
        counter.nProcessed = 0;
        LOG_WARN("Testing only - no files written");
    }
    LOG_INFO(cYellow, "makeClass --options", cOff, " processed ", counter.nVisited, " files (changed ",
             counter.nProcessed, ").", string_q(40, ' '));

    writeApiFile();

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
bool COptions::writeCode(const string_q& fn) {
    string_q orig = asciiFileToString(fn);
    string_q converted = orig;
    if (endsWith(fn, ".cpp")) {
        converted = replaceCode(converted, "CODE_AUTO", auto_stream.str());
        converted = replaceCode(converted, "CODE_OPTIONS", option_stream.str());
        converted = replaceCode(converted, "CODE_LOCAL_INIT", local_stream.str());
        converted = replaceCode(converted, "CODE_INIT", init_stream.str());
        converted = replaceCode(converted, "CODE_NOTES", notes_stream.str());
        converted = replaceCode(converted, "CODE_ERROR_MSG", errors_stream.str());
    } else {
        converted = replaceCode(converted, "CODE_DECLARE", declare_stream.str());
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
void COptions::writeApiFile(void) {
    if (!api)
        return;

    CCommands commands;
    CApiRoute curRoute;
    for (auto option : optionArray) {
        if (curRoute.route != option.api_route) {
            if (!curRoute.route.empty())
                commands.routes.push_back(curRoute);
            curRoute = CApiRoute();
            curRoute.route = option.api_route;
        }
        curRoute.commands.push_back(option);
    }
    commands.routes.push_back(curRoute);

    expContext().lev = 2;
    HIDE_FIELD(CCommandOption, "is_customizable");

    ostringstream out;
    bool first2 = true;
    out << "{" << endl;
    for (auto route : commands.routes) {
        if (!first2)
            out << "," << endl;
        out << "  \"" << route.route << "\": {" << endl;
        bool first1 = true;
        for (auto command : route.commands) {
            if (command.option_kind != "note") {
                if (command.hotkey.empty())
                    command.hotkey = "<not-set>";
                if (command.command.empty())
                    command.command = "<not-set>";
                if (command.data_type.empty())
                    command.data_type = "<not-set>";
                ostringstream ts;
                ts << command;
                string_q tss = ts.str();
                replaceReverse(tss, "\n", "");

                ostringstream os;
                if (!first1)
                    os << "," << endl;
                os << "    "
                   << "\"" << command.command << "\": " << tss;
                string_q s = os.str();
                replaceAll(s, "\"true\"", "true");
                replaceAll(s, "\"false\"", "false");
                replaceAll(s, "NOPOS", "");
                replaceAll(s, "\"def_val\": false,", "\"def_val\": \"\",");
                replaceAll(s, "\"def_val\": true,", "\"def_val\": \"true\",");
                replaceAll(s, "\"def_val\": \"\"\"\",", "\"def_val\": \"\",");
                replaceAll(s, "<not-set>", "");
                out << s;
                first1 = false;
            }
        }
        out << endl << "  }";
        first2 = false;
    }
    out << endl << "}" << endl;
    stringToAsciiFile("../../trueblocks-explorer/api/api_options.json", out.str());
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
const char* STR_AUTO_FLAG_DOUBLE =
    "        } else if ([startsWith(arg, \"-{HOTKEY}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
    "            if (!confirmDouble(\"[{COMMAND}]\", [{COMMAND}], arg))\n"
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
const char* STR_TOPIC_PROCESSOR =
    "            } else if (!parseTopicList2(this, [{COMMAND}], arg))\n"
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
    "    [{COMMAND}] = getGlobalConfig(\"[{TOOL}]\")->getConfig[CTYPE](\"settings\", \"[{COMMAND}]\", "
    "[{DEF_VAL}]);";
