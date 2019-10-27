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
#include "optiondef.h"

extern const char* STR_OPTION_STR;
extern const char* STR_AUTO_SWITCH;
extern const char* STR_AUTO_TOGGLE;
extern const char* STR_AUTO_FLAG;
extern const char* STR_AUTO_FLAG_ENUM;
extern const char* STR_AUTO_FLAG_BLOCKNUM;
extern const char* STR_AUTO_FLAG_UINT;
extern const char* STR_CHECK_BUILTIN;
extern const char* STR_BLOCK_PROCESSOR;
extern const char* STR_TX_PROCESSOR;
uint32_t nFiles = 0, nChanges = 0;
//---------------------------------------------------------------------------------------------------
bool COptions::handle_options(void) {
    COptionDef::registerClass();

    string_q contents = asciiFileToString("../src/other/build_assets/option-master-list.csv");

    CStringArray lines;
    explode(lines, contents, '\n');

    map<string, bool> tools;
    COptionDefArray optionArray;
    for (auto line : lines) {
        COptionDef optDef(line);
        if (!optDef.tool.empty() && optDef.tool != "all" && optDef.tool != "tool" && optDef.tool != "templates") {
            optionArray.push_back(optDef);
            tools[optDef.group + "/" + optDef.tool] = true;
        }
    }

    for (auto tool : tools) {
        warnings.clear();
        warnings.str("");

        bool allAuto = true, hasBlockList = false, hasTxList = false;
        map<string, string> shortCmds;
        ostringstream opt_stream, init_stream, local_stream, auto_stream, declare_stream;
        for (auto option : optionArray) {
            if ((option.group + "/" + option.tool) == tool.first) {
                check_option(option);

                bool isEnum = contains(option.data_type, "enum");
                bool isBlockNum = contains(option.data_type, "blknum");
                bool isUint32 = contains(option.data_type, "uint32");
                bool isUint64 = contains(option.data_type, "uint64");
                opt_stream << option.Format(STR_OPTION_STR) << endl;

                if (!option.generate.empty()) {

                    string_q type = substitute(substitute(substitute(option.data_type, "boolean", "bool"), "<", ""), ">", "");
                    if (contains(option.data_type, "enum"))
                        type = "string";
                    replace(type, "blknum", "blknum_t");
                    replace(type, "string", "string_q");
                    replace(type, "uint32", "uint32_t");
                    replace(type, "uint64", "uint64_t");
                    string_q def = (type == "bool" ? "false" : (type == "string_q" ? "\"\"" : "NOPOS"));
                    if (!option.def_val.empty())
                        def = option.def_val;

                    if (option.generate == "local") {

                        hasBlockList = (hasBlockList || (option.option_kind == "positional" && option.data_type == "list<blknum>"));
                        hasTxList = (hasTxList || (option.option_kind == "positional" && option.data_type == "list<tx_id>"));
                        if (option.option_kind == "switch") {

                            local_stream << option.Format("    " + type + " [{COMMAND}] = " + def + ";") << endl;
                            auto_stream << option.Format(STR_AUTO_SWITCH) << endl;

                        } else if (option.option_kind == "toggle") {

                            local_stream << option.Format("    " + type + " [{COMMAND}] = " + def + ";") << endl;
                            auto_stream << option.Format(STR_AUTO_TOGGLE) << endl;

                        } else if (option.option_kind == "flag") {

                            local_stream << option.Format("    " + type + " [{COMMAND}] = " + def + ";") << endl;
                            if (isEnum)
                                auto_stream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                            else if (isBlockNum)
                                auto_stream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                            else if (isUint32 || isUint64)
                                auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                            else
                                auto_stream << substitute(option.Format(STR_AUTO_FLAG), "substitute(substitute(arg, \"-:\", )", "substitute(arg") << endl;

                        } else {
                            // do nothing
                        }

                    } else if (option.generate == "yes") {

                        if (option.option_kind == "switch") {

                            init_stream << option.Format("    [{COMMAND}] = " + def + ";") << endl;
                            declare_stream << option.Format("    " + type + " [{COMMAND}];") << endl;
                            auto_stream << option.Format(STR_AUTO_SWITCH) << endl;

                        } else if (option.option_kind == "toggle") {

                            init_stream << option.Format("    [{COMMAND}] = " + def + ";") << endl;
                            declare_stream << option.Format("    " + type + " [{COMMAND}];") << endl;
                            auto_stream << option.Format(STR_AUTO_TOGGLE) << endl;

                        } else if (option.option_kind == "flag") {

                            init_stream << option.Format("    [{COMMAND}] = " + def + ";") << endl;
                            declare_stream << option.Format("    " + type + " [{COMMAND}];") << endl;
                            if (isEnum)
                                auto_stream << option.Format(STR_AUTO_FLAG_ENUM) << endl;
                            else if (isBlockNum)
                                auto_stream << option.Format(STR_AUTO_FLAG_BLOCKNUM) << endl;
                            else if (isUint32 || isUint64)
                                auto_stream << option.Format(STR_AUTO_FLAG_UINT) << endl;
                            else
                                auto_stream << substitute(option.Format(STR_AUTO_FLAG), "substitute(substitute(arg, \"-:\", )", "substitute(arg") << endl;

                        } else {
                            // do nothing
                        }

                    } else {
                        // do nothing
                    }
                } else {
                    if (option.option_kind == "toggle" || option.option_kind == "switch" || option.option_kind == "flag")
                        allAuto = false;
                }

                if (!option.hotkey.empty() && !contains(option.option_kind, "positional") && !contains(option.option_kind, "description")) {
                    if (!shortCmds[option.hotkey].empty())
                        warnings << "Hotkey '" << cRed << option.command << "-" << option.hotkey << cOff << "' conflicts with existing '" << cRed << shortCmds[option.hotkey] << cOff << "'|";
                    shortCmds[option.hotkey] = option.command + "-" + option.hotkey;
                    bool isUpper = (toLower(option.hotkey) != option.hotkey);
                    bool isFirst = option.hotkey == option.command.substr(0,1);
                    bool isSecond = option.hotkey == option.command.substr(1,1);
                    bool isContained = !verbose && contains(option.command, option.hotkey);
                    if (!isFirst && !isSecond && !isUpper && !isContained) {
                        warnings << "Hotkey '" << cRed << option.hotkey << "' " << cOff;
                        warnings << "of command '" << cRed << option.command << cOff << "' is not first or second character|";
                    }
                }
            }
        }

        if (allAuto) {
            auto_stream << STR_CHECK_BUILTIN << endl;
            if (hasBlockList)
                auto_stream << STR_BLOCK_PROCESSOR << endl;
            if (hasTxList)
                auto_stream << STR_TX_PROCESSOR << endl;
        }

        string_q fn = "../src/" + tool.first + "/options.cpp";
        writeCode(fn, auto_stream.str(), opt_stream.str(), local_stream.str(), init_stream.str());
        writeCode(substitute(fn, ".cpp", ".h"), declare_stream.str());

        if (!warnings.str().empty()) {
            CStringArray w;
            explode(w, warnings.str(), '|');
            for (auto warning : w)
                LOG_WARN(warning);
        }
    }
    cerr << cGreen << "makeClass --options: processed " << nFiles << " files (changed " << nChanges << ")." << string_q(50, ' ') << cOff << endl;

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::check_option(const COptionDef& option) {

    // Check valid data types
    CStringArray validTypes = {
        "<addr>", "<blknum>", "<pair>", "<path>", "<range>", "<string>", "<uint32>", "<uint64>", "<boolean>",
    };

    bool valid_type = false;
    for (auto type : validTypes) {
        if (type == option.data_type) {
            valid_type = true;
        }
    }
    if (!valid_type) {
        if (startsWith(option.data_type, "enum")) valid_type = true;
        if (startsWith(option.data_type, "list")) valid_type = true;
    }
    if (!valid_type && option.option_kind == "description" && option.data_type.empty()) valid_type = true;

    if (!valid_type)
        warnings << "Unknown type '" << cRed << option.data_type << cOff << "' for option '" << cRed << option.command << cOff << "'|";
    if (option.option_kind == "description" && !endsWith(option.description, "."))
        warnings << "Description '" << cRed << option.description << cOff << "' should end with a period.|";
    if (option.option_kind != "description" && endsWith(option.description, "."))
        warnings << "Option '" << cRed << option.description << cOff << "' should not end with a period.|";

    return true;
}

//---------------------------------------------------------------------------------------------------
string_q replaceCode(const string_q& orig, const string_q& which, const string_q& new_code) {
    string_q converted = orig;
    converted = substitute(converted, "// BEG_" + which, "// BEG_" + which + "\n[{NEW_CODE}]\n<remove>");
    converted = substitute(converted, "// END_" + which, "</remove>\n// END_" + which);
    snagFieldClear(converted, "remove");
    replace(converted, "[{NEW_CODE}]\n\n", new_code);
    return converted;
}
//---------------------------------------------------------------------------------------------------
bool COptions::writeCode(const string_q& fn, const string_q& code, const string_q& opt, const string_q& local, const string_q& init) {
    string_q orig = asciiFileToString(fn);
    string_q converted = orig;
    if (endsWith(fn, ".cpp")) {
        converted = replaceCode(converted, "CODE_AUTO", code);
        converted = replaceCode(converted, "CODE_OPTIONS", opt);
        converted = replaceCode(converted, "CODE_LOCAL_INIT", local);
        converted = replaceCode(converted, "CODE_INIT", init);
    } else {
        converted = replaceCode(converted, "CODE_DECLARE", code);
    }
    cerr << bBlue << "Processing " << cOff << fn << " ";
    nFiles++;
    if (converted != orig) {
        cerr << cGreen << "wrote " << converted.size() << " bytes..." << cOff << endl;
        stringToAsciiFile(fn, converted);
        nChanges++;
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
"            if (!confirmEnum(\"[{COMMAND}]\", [{COMMAND}], arg))\n                return false;\n";

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
"        } else if (!parseBlockList2(this, blocks, arg, latest)) {\n"
"            return false;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_TX_PROCESSOR =
"        } else if (!parseTransList2(this, transList, arg)) {\n"
"            return false;\n";
