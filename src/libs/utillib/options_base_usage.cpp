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

ostream& out = cerr;
//--------------------------------------------------------------------------------
bool COptionsBase::usage(const string_q& errMsg) const {
    bool quitting = !(errMsg.empty() || contains(errMsg, "Invalid option:") || isApiMode());
    out << get_errmsg(errMsg + (quitting ? " Quitting..." : ""));
    if (isReadme) {
        out << get_header();
        out << get_purpose();
    } else {
        out << get_purpose();
        out << get_header();
    }
    out << get_description();
    out << get_notes();
    out << get_configs();
    out << get_version();
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
        os << "Usage:" << endl;
        os << "  " << getProgName() << " " << get_options() << endl;
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
        os << "Purpose:" << endl;
    }
    string_q purpose;
    for (auto option : parameters)
        if (option.longName.empty())  // program description
            purpose = substitute(option.description, "|", "\n            ");
    if (isReadme) {
        os << substitute(purpose, "\n", "\n        ") << endl;
        if (!endsWith(purpose, "\n"))
            os << "\n";
    } else {
        os << "  " << purpose << endl;
    }

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
            if (option.isPublic() || (!option.is_visible && (isTestMode() || (verbose > 1)))) {
                widths[0] = max(widths[0], option.getHotKey().length());
                widths[1] = max(widths[1], option.getLongKey().length());
                widths[2] = max(widths[2], option.getDescription().length());
            }
        } else {
            if (option.isPublic() || (!option.is_visible && (isTestMode() || (verbose > 1)))) {
                widths[0] = max(widths[0], option.getHotKey().length());
                widths[1] = max(widths[1], option.getLongKey().length());
                widths[2] = max(widths[2], option.getDescription().length());
            }
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
                if (param->isPublic()) {
                    widths[0] = max(widths[0], param->getHotKey().length());
                    widths[1] = max(widths[1], param->getLongKey().length());
                    widths[2] = max(widths[2], param->getDescription().length());
                }
            } else {
                if (param->isPublic()) {
                    widths[0] = max(widths[0], param->getHotKey().length());
                    widths[1] = max(widths[1], param->getLongKey().length());
                    widths[2] = max(widths[2], param->getDescription().length());
                }
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
        COptionArray positionals;
        get_positionals(positionals);
        if (positionals.size()) {
            os << endl << "Arguments:" << endl;
            for (auto option : positionals) {
                string_q req = (option.is_required ? " (required)" : "");
                os << "  " << substitute(option.longName, "addrs2", "addrs") << " - " << option.description << req
                   << endl;
            }
        }
        os << endl << "Flags:" << endl;
    }

    for (auto option : parameters) {
        if (isReadme) {
            option.is_readme = true;
        }
        if (!option.is_visible) {
            if (!option.is_deprecated && !option.longName.empty()) {
                if (isReadme) {
                    hidden << option.oneDescription(widths);
                } else {
                    os << option.oneDescription(widths);
                }
            }
        } else if (option.isPublic()) {
            os << option.oneDescription(widths);
        }
    }

    if (!hidden.str().empty() && (isTestMode() || verbose > 1)) {
        if (isReadme) {
            os << markDownRow("###", "Hidden options", "", widths);
        } else {
            os << endl;
            os << "    #### Hidden options" << endl;
        }
        os << hidden.str();
        if (isReadme) {
            os << markDownRow("###", "Hidden options", "", widths);
        } else {
            os << "    #### Hidden options" << endl;
            os << endl;
        }
    }

    os << extra.str();
    if (isReadme) {
        os << "{{</td>}}" << endl;
    }
    os << endl;

    if (!isReadme) {
        // clang-format off
        if (isEnabled(OPT_FMT) || isEnabled(OPT_VERBOSE) || isEnabled(OPT_HELP))
            os << "Global Flags:\n";
        os << (isEnabled(OPT_FMT) ? "  -x, --fmt string     export format, one of [none|json*|txt|csv|api]\n" : "");
        os << (isEnabled(OPT_VERBOSE) ? "  -v, --verbose uint   set verbose level (optional level defaults to 1)\n" : "");
        os << (isEnabled(OPT_HELP) ? "  -h, --help           display this help screen\n" : "");
        // clang-format on
    }

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
    } else if (verbose || isTestMode()) {
        os << endl << "Notes:" << endl;
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
        if (notes.size() == 0)
            os << endl;
        os << "Configurable Items:" << endl;
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
        os << endl << "Commands:" << endl;
        os << format_notes(overrides);
        os << endl;
    }

    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_version(void) const {
    if (!isReadme)
        return "";

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
        os << endl;
    }
    return os.str();
}

//--------------------------------------------------------------------------------
string_q COptionsBase::clean_positional(const string_q& strIn) const {
    string_q strOut = strIn;
    replaceAll(strOut, "addrs2 blocks", "<address> <address> [address...] [block...]");
    replaceAll(strOut, "addrs blocks", "<address> [address...] [block...]");
    replaceAll(strOut, "transactions", "<tx_id> [tx_id...]");
    if (contains(toLower(getProgName()), "when"))
        replaceAll(strOut, "blocks", "< block | date > [ block... | date... ]");
    else
        replaceAll(strOut, "blocks", "<block> [block...]");
    replaceAll(strOut, "addrs topics fourbytes", "<address> [address...] [topics] [fourbytes]");
    replaceAll(strOut, "addrs", "<address> [address...]");
    replaceAll(strOut, "files", "<file> [file...]");
    replaceAll(strOut, "terms", "<term> [term...]");
    replaceAll(strOut, "modes", "<mode> [mode...]");
    if (isReadme) {
        strOut = substitute(substitute(strOut, "<", "&lt;"), ">", "&gt;");
    }
    return trim(strOut);
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_positionals(COptionArray& pos) const {
    ostringstream os;
    for (auto option : parameters) {
        if (option.is_positional) {
            pos.push_back(option);
            if (!os.str().empty())
                os << " ";
            string_q str = clean_positional(option.longName);
            if (!option.is_required) {
                replace(str, "<mode> [mode...]", "[mode...]");
                replace(str, "<term> [term...]", "[term...]");
            }
            os << str;
        }
    }
    string_q ret = os.str();
    replace(ret, "&lt;address&gt; [address...] &lt;block&gt; [block...]", "&lt;address&gt; [address...] [block...]");
    replace(ret, "<address> [address...] <block> [block...]", "<address> [address...] [block...]");
    replace(ret, "<address> [address...]2 <block>", "<address> <address> [address...]");
    replace(ret, "&lt;address&gt; [address...]2 &lt;block&gt; [block...]",
            "&lt;address&gt; &lt;address&gt; [address...] [block...]");
    replace(ret, "<address> [address...] topics fourbytes", "<address> [address...] [topics] [fourbytes]");
    replace(ret, "&lt;address&gt; [address...] topics fourbytes", "&lt;address&gt; [address...] [topics] [fourbytes]");
    return ret;
}

//--------------------------------------------------------------------------------
string_q COptionsBase::get_options(void) const {
    ostringstream shorts;
    for (const auto& option : parameters) {
        if (!option.is_positional && (!option.is_visible || option.is_deprecated)) {
            // not an option
        } else if (!option.is_positional && !option.hotKey.empty()) {
            shorts << option.hotKey << "|";
        }
    }
    if (isEnabled(OPT_VERBOSE))
        shorts << "-v|";
    if (isEnabled(OPT_HELP))
        shorts << "-h";
    if (isReadme) {
        // do nothing
    } else {
        shorts.str("");
        shorts.clear();
        shorts << "flags";
    }

    ostringstream os;
    if (!shorts.str().empty())
        os << "[" << shorts.str() << "] ";

    COptionArray unused;
    os << get_positionals(unused);

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
        lead = "  ";
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
        out << endl
            << cRed << "  Warning: " << cOff << message << (endsWith(msg, '.') ? "" : ".") << " Quitting..." << endl
            << endl;
    }
}

}  // namespace qblocks
