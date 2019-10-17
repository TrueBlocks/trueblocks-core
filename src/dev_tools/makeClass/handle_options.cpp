/*-------------------------------------------------------------------------------------------
 * qblocks - fast, easily-accessible, fully-decentralized data from blockchains
 * copyright (c) 2018 Great Hill Corporation (http://greathill.com)
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
extern const char* STR_AUTO_FLAG;
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

        map<string, string> shortCmds;
        ostringstream opt_stream, init_stream, local_stream, auto_stream;
        for (auto option : optionArray) {
            if ((option.group + "/" + option.tool) == tool.first) {
                check_option(option);

                opt_stream << option.Format(STR_OPTION_STR) << endl;

                if (!option.auto_generate.empty()) {
                    if (option.option_kind == "switch")
                        auto_stream << option.Format(STR_AUTO_SWITCH);
                    else if (option.option_kind == "flag")
                        auto_stream << substitute(option.Format(STR_AUTO_FLAG), "substitute(substitute(arg, \"-:\", )", "substitute(arg");
                    if (option.auto_generate != "dummy") {
                        init_stream  << (option.auto_generate == "yes"   ? option.Format("    [{COMMAND}] = false;\n") : "");
                        local_stream << (option.auto_generate == "local" ? option.Format("    bool [{COMMAND}] = false;\n") : "");
                    } else {
                        auto_stream << "            // do nothing" << endl;
                    }
                    auto_stream << endl;
                }

                if (!option.command_short.empty() && !contains(option.option_kind, "positional") && !contains(option.option_kind, "description")) {
                    if (!shortCmds[option.command_short].empty())
                        warnings << "Short command '" << cRed << option.command << "-" << option.command_short << cOff << "' conflicts with existing '" << cRed << shortCmds[option.command_short] << cOff << "'|";
                    shortCmds[option.command_short] = option.command + "-" + option.command_short;
                }
            }
        }

        string_q fn = "../src/" + tool.first + "/options.cpp";
        bool changed = false;
        changed |= writeNewCode(fn, "CODE_OPTIONS", opt_stream.str());
        changed |= writeNewCode(fn, "CODE_INIT", init_stream.str());
        changed |= writeNewCode(fn, "CODE_LOCAL_INIT", local_stream.str());
        changed |= writeNewCode(fn, "CODE_AUTO", auto_stream.str());
        if (changed)
            nChanges++;
        nFiles++;

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
        "<addr>", "<blknum>", "<pair>", "<path>", "<range>", "<string>", "<uint>",
        "boolean", "string",
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
    if (option.option_kind == "description" && !endsWith(option.description_core, "."))
        warnings << "Description '" << cRed << option.description_core << cOff << "' should end with a period.|";
    if (option.option_kind != "description" && endsWith(option.description_core, "."))
        warnings << "Option '" << cRed << option.description_core << cOff << "' should not end with a period.|";

    return true;
}

//---------------------------------------------------------------------------------------------------
bool COptions::writeNewCode(const string_q& fn, const string_q& which, const string_q& new_code) {
    string_q orig = asciiFileToString(fn);
    string_q converted = orig;
    converted = substitute(converted, "// BEG_" + which, "// BEG_" + which + "\n[{NEW_CODE}]\n<remove>");
    converted = substitute(converted, "// END_" + which, "</remove>\n// END_" + which);
    snagFieldClear(converted, "remove");
    replace(converted, "[{NEW_CODE}]\n\n", new_code);
    cerr << bBlue << "Processing " + which + ": " << cOff << fn << " ";
    if (converted != orig) {
        cerr << cGreen << "wrote " << converted.size() << " bytes..." << cOff << endl;
        stringToAsciiFile(fn, converted);
        return true;
    }
    cerr << cTeal << "no changes..." << cOff << "\r";
    cerr.flush();
    return false;
}

//---------------------------------------------------------------------------------------------------
const char* STR_OPTION_STR =
"    COption(\"[{COMMAND}]\", \"[{COMMAND_SHORT}]\", \"[{DATATYPE}]\", [{OPTS}], \"[{DESCRIPTION_CORE}]\"),";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_SWITCH =
"        } else if ([arg == \"-{COMMAND_SHORT}\" || ]arg == \"--[{COMMAND}]\") {\n"
"            [{COMMAND}] = true;\n";

//---------------------------------------------------------------------------------------------------
const char* STR_AUTO_FLAG =
"        } else if ([startsWith(arg, \"-{COMMAND_SHORT}:\") || ]startsWith(arg, \"--[{COMMAND}]:\")) {\n"
"            arg = substitute(substitute(arg, \"-[{COMMAND_SHORT}]:\", ""), \"--[{COMMAND}]:\", \"\");\n";
