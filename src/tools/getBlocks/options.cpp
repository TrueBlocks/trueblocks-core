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
#include "options.h"

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("blocks", "", "list<blknum>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more block identifiers"),  // NOLINT
    COption("cache", "o", "", OPT_SWITCH, "force a write of the block to the cache"),
    COption("", "", "", OPT_DESCRIPTION, "Retrieve one or more blocks from the chain or local cache."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_FORMAT_LIST_JSON;
extern const char* STR_FORMAT_LIST;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    blknum_t latest = getLatestBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        // do not collapse
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-o" || arg == "--cache") {
            cache = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

        } else {
            if (!parseBlockList2(this, blocks, arg, latest))
                return false;

            // END_CODE_AUTO
        }
    }

    if (!cache) {
        return usage("Nope");
    }

    if (cache)
        etherlib_init(defaultQuitHandler);

    if (expContext().exportFmt == NONE1)
        expContext().exportFmt = JSON1;
    configureDisplay("getBlocks", "CBlock", STR_DISPLAY_BLOCK);

    if (noHeader)
        expContext().fmtMap["header"] = "";

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_RAW);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    cache = false;
    // END_CODE_INIT

    secsFinal = (60 * 5);  // Used to be configurable, but no longer
    blocks.Init();
    CBlockOptions::Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();
    expContext().exportFmt = NONE1;

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("`Blocks` is a space-separated list of values, a start-end range, a `special`, or any combination.");  // NOLINT
    notes.push_back("`Blocks` may be specified as either numbers or hashes.");
    notes.push_back("`Special` blocks are detailed under `chifra when --list`.");
    notes.push_back("With the --logs option, optionally specify one or more --emitter, one or more --topics, either or both.");  // NOLINT
    notes.push_back("The --logs option is significantly faster if you provide an --emitter and/or a --topic.");
    notes.push_back("Multiple topics match on topic0, topic1, and so on, not on different topic0's.");
    notes.push_back("For the --logs option, large block ranges may crash the node, use --big_range to specify a larger range.");  // NOLINT
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS

    usageErrs[ERR_NOCACHEUNCLE] = "The --cache option is not available for uncle blocks.";
    usageErrs[ERR_NOCACHEADDRESS] = "The --cache option is not available when using one of the address options.";
    usageErrs[ERR_TRACINGREQUIRED] = "A tracing node is required for the --traces options to work properly.";
    usageErrs[ERR_NOTRACEADDRESS] = "The --traces option is not available when using one of the address options.";
    usageErrs[ERR_TRACEHASHEXCLUSIVE] = "The --hashes and --traces options are exclusive.";
    usageErrs[ERR_ATLEASTONEBLOCK] = "You must specify at least one block.";
    usageErrs[ERR_EMTOPONLYWITHLOG] = "The --emitter and --topic options are only available with the --log option.";
    usageErrs[ERR_ARTWITHOUTLOGS] = "The --artcilate option is only available with the --logs option.";
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
void interumReport(ostream& os, blknum_t i) {
    os << (!(i % 150) ? "." : (!(i % 1000)) ? "+" : "");  // dots '.' at every 150, '+' at every 1000
    os.flush();
}
