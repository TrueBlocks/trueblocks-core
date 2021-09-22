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
/*
 * Parts of this file were generated with makeClass --options. Edit only those parts of
 * the code outside of the BEG_CODE/END_CODE sections
 */
#include "basetypes.h"
#include "database.h"
#include "exportcontext.h"
#include "options_base.h"
#include "colors.h"
#include "filenames.h"
#include "accountname.h"
#include "rpcresult.h"

namespace qblocks {

//--------------------------------------------------------------------------------
bool COptionsBase::usage(const string_q& errMsg) const {
    bool quitting = !(errMsg.empty() || contains(errMsg, "Invalid option:") || isApiMode());
    cerr << get_errmsg(errMsg + (quitting ? " Quitting..." : ""));
    cerr << get_header();
    cerr << get_purpose();
    cerr << get_description();
    cerr << get_notes();
    cerr << get_configs();
    cerr << get_version();
    return false;
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_header(void) const {
    ostringstream os;
    if (isReadme) {
        os << "### Usage" << endl;
        os << endl;
        os << "`Usage:`"
           << "    " << progNameMap[getProgName()] << " " << get_options() << "  " << endl;
    } else {
        os << endl;
        os << cYellow << "  Usage:" << cOff << "    " << getProgName() << " " << get_options() << "  " << endl;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_purpose(void) const {
    ostringstream os;
    if (isReadme) {
        os << "`Purpose:`"
           << "  ";
    } else {
        os << cYellow << "  Purpose:" << cOff << "  ";
    }
    string_q purpose;
    for (auto option : parameters)
        if (option.longName.empty())  // program description
            purpose = substitute(option.description, "|", "\n            ");
    os << substitute(purpose, "\n", "\n        ") << "\n";
    if (!endsWith(purpose, "\n"))
        os << "\n";

    if (isReadme) {
        return os.str();
    }
    return colorize(os.str());
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_description(void) const {
    if (!overrides.empty())
        return get_override();

    ostringstream os, hidden, extra;

    size_t widths[5];
    bzero(widths, sizeof(widths));
    for (auto option : parameters) {
        if (isReadme) {
            option.is_readme = true;
        }
        if (option.isPublic() || (option.is_hidden && (isTestMode() || (verbose > 1)))) {
            widths[0] = max(widths[0], option.getHotKey().length());
            widths[1] = max(widths[1], option.getLongKey().length());
            widths[2] = max(widths[2], option.getDescription().length());
        }
    }
    widths[0] = max(widths[0], size_t(3));

    //---------------------------------------------------------------------------------------------------
    static COption opts[] = {
        COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_FLAG, "export format"),
        COption("verbose", "v", "<uint>", OPT_VERBOSE, "set verbose level (optional level defaults to 1)"),
        COption("help", "h", "<boolean>", OPT_HELP, "display this help screen"),
    };
    static size_t nOpts = sizeof(opts) / sizeof(COption);
    for (size_t i = 0; i < nOpts; i++) {
        bool show = false;
        show |= (i == 0 && isEnabled(OPT_FMT));
        show |= (i == 1 && isEnabled(OPT_VERBOSE));
        show |= (i == 2 && isEnabled(OPT_HELP));
        if (show) {
            COption* param = &opts[i];
            if (isReadme) {
                param->is_readme = true;
            }
            if (param->isPublic()) {
                widths[0] = max(widths[0], param->getHotKey().length());
                widths[1] = max(widths[1], param->getLongKey().length());
                widths[2] = max(widths[2], param->getDescription().length());
            }
            extra << param->oneDescription(widths);
        }
    }

    if (isReadme) {
        os << "`Where:`" << endl;
        os << endl;
        os << "{{<td>}}" << endl;
        os << markDownRow("", "Option", "Description", widths);
        os << markDownRow("-", "", "", widths);
    } else {
        os << cYellow << "  Where:" << cOff << endl;
    }

    for (auto option : parameters) {
        if (isReadme) {
            option.is_readme = true;
        }
        if (option.is_hidden) {
            if (option.is_hidden && !option.is_deprecated && !option.longName.empty()) {
                hidden << option.oneDescription(widths);
            }
        } else if (option.isPublic())
            os << option.oneDescription(widths);
    }

    if (!hidden.str().empty() && (isTestMode() || verbose > 1)) {
        if (isReadme) {
            os << markDownRow("###", "Hidden options", "", widths);
        } else {
            os << endl;
            os << "\t#### Hidden options" << endl;
        }
        os << hidden.str();
        if (isReadme) {
            os << markDownRow("###", "Hidden options", "", widths);
        } else {
            os << "\t#### Hidden options" << endl;
            os << endl;
        }
    }

    os << extra.str();
    if (isReadme) {
        os << "{{</td>}}" << endl;
    }
    os << endl;

    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_notes(void) const {
    if (!notes.size())
        return "";

    ostringstream os;
    if (isReadme) {
        os << "`Notes:`" << endl;
        os << endl;
        os << format_notes(notes);
        os << endl;
    } else if (verbose) {
        os << cYellow << "  Notes:" << cOff << endl;
        os << format_notes(notes);
        os << endl;
    } else {
        // do nothing;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_configs(void) const {
    if (!configs.size())
        return "";

    ostringstream os;
    if (isReadme) {
        os << "`Configurable Items:`" << endl;
        os << endl;
        os << format_notes(configs);
        os << endl;
    } else if (verbose) {
        os << cYellow << "  Configurable Items:" << cOff << endl;
        os << format_notes(configs);
        os << endl;
    } else {
        // do nothing;
    }

    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_override(void) const {
    ostringstream os;
    if (isReadme) {
        os << "`Where:`" << endl;
        os << "```" << endl;
        os << format_notes(overrides);
        os << "```" << endl;
        os << endl;

    } else {
        os << cYellow << "  Where:" << cOff << "  " << endl;
        os << format_notes(overrides);
        os << endl;
    }

    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_version(void) const {
    ostringstream os;
    if (isReadme) {
        // do nothing
    } else {
        os << bBlue << "  Powered by TrueBlocks";
        os << (isTestMode() ? "" : " (" + getVersionStr() + ")") << "\n" << cOff;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_errmsg(const string_q& errMsgIn) const {
    if (errMsgIn.empty())
        return "";

    string_q errMsg = substitute(errMsgIn, "`", "");

    if (isApiMode())
        errorMessage(getProgName() + " - " + errMsg);

    ostringstream os;
    if (isReadme) {
        // do nothing
    } else {
        os << endl;
        os << cRed << "  " << errMsg << cOff << endl;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_options(void) const {
    string_q positional;

    ostringstream shorts;
    for (const auto& option : parameters) {
        if (option.is_positional) {
            positional += (" " + option.longName);

        } else if (option.is_hidden || option.is_deprecated) {
            // invisible option

        } else if (!option.hotKey.empty()) {
            shorts << option.hotKey << "|";
        }
    }
    if (isEnabled(OPT_VERBOSE))
        shorts << "-v|";
    if (isEnabled(OPT_HELP))
        shorts << "-h";

    ostringstream os;
    if (!shorts.str().empty())
        os << "[" << shorts.str() << "] ";

    replaceAll(positional, "addrs2 blocks", "<address> <address> [address...] [block...]");
    replaceAll(positional, "addrs blocks", "<address> [address...] [block...]");
    replaceAll(positional, "block_list", "< block | date > [ block... | date... ]");
    replaceAll(positional, "transactions", "<tx_id> [tx_id...]");
    replaceAll(positional, "blocks", "<block> [block...]");
    replaceAll(positional, "addrs topics fourbytes", "<address> [address...] [topics] [fourbytes]");
    replaceAll(positional, "addrs", "<address> [address...]");
    replaceAll(positional, "files", "<file> [file...]");
    replaceAll(positional, "terms", "<term> [term...]");
    replaceAll(positional, "modes", "<mode> [mode...]");
    if (isReadme) {
        positional = substitute(substitute(positional, "<", "&lt;"), ">", "&gt;");
    }
    os << trim(positional);

    return trim(os.str());
}

//--------------------------------------------------------------------------------
string_q colorize(const string_q& strIn) {
    string_q str = strIn;
    while (contains(str, '`')) {
        replace(str, "`", cTeal);
        replace(str, "`", cOff);
    }
    return str;
}

//--------------------------------------------------------------------------------
string_q COptionsBase::format_notes(const CStringArray& noteList) const {
    string_q lead;
    if (isReadme) {
        // do nothing
    } else {
        lead = "   ";
    }

    ostringstream os;
    for (auto note : noteList) {
        replaceAll(note, " |", "|");
        replaceAll(note, "|", "\n" + lead + " ");
        if (isReadme) {
            // do nothing
        } else {
            note = colorize(note);
        }
        os << lead;
        if (overrides.empty())
            os << "-";
        os << " " << note << endl;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
void errorMessage(const string_q& msg) {
    if (isApiMode()) {
        const char* STR_ERROR_JSON = "{ \"errors\": [ \"[ERRORS]\" ] }\n";
        string_q message = substitute(msg, "$CONFIG", configPath("trueBlocks.toml"));
        if (!contains(message, "[")) {
            message = substitute(message, "|", " ");
        }
        string_q errMsg = substitute(STR_ERROR_JSON, "[ERRORS]", message);
        cout << substitute(errMsg, "`", "");
    } else {
        string_q message = substitute(substitute(msg, "|", "\n  "), "$CONFIG", configPath("trueBlocks.toml"));
        message = colorize(message);
        cerr << endl
             << cRed << "  Warning: " << cOff << message << (endsWith(msg, '.') ? "" : ".") << " Quitting..." << endl
             << endl;
    }
}

}  // namespace qblocks
