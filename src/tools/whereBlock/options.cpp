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

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
// BEG_CODE_OPTIONS
    COption("block_list", "", "list<block>", OPT_REQUIRED | OPT_POSITIONAL, "a space-separated list of one or more blocks to search for"),
    COption("fmt", "x", "enum[none|json*|txt|csv|api]", OPT_HIDDEN | OPT_FLAG, "export format (one of [none|json*|txt|csv|api])"),
    COption("", "", "", OPT_DESCRIPTION, "Reports if a block was found in the cache, at a local, or at a remote node."),
// END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

extern const char* STR_DISPLAY_WHERE;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool no_header = false;
    string_q format = getGlobalConfig("whereBlock")->getConfigStr("display", "format", STR_DISPLAY_WHERE);
    Init();
    blknum_t latestBlock = getLastBlock_client();
    if (!isNodeRunning()) // it's okay if it's not
        latestBlock = NOPOS;
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {
            string_q ret = blocks.parseBlockList(arg, latestBlock);
            if (!isApiMode() && endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }
        }
    }

    // Data verifictions
    if (!blocks.hasBlocks())
        return usage("You must enter a valid block number. Quitting...");

    // Display formatting
    switch (exportFmt) {
        case NONE1: format = STR_DISPLAY_WHERE; break;
        case API1:
        case JSON1: format = ""; break;
        case TXT1:
        case CSV1:
            format = getGlobalConfig("whereBlock")->getConfigStr("display", "format", format.empty() ? STR_DISPLAY_WHERE : format);
            break;
    }
    manageFields("CCacheEntry:" + cleanFmt((format.empty() ? STR_DISPLAY_WHERE : format), exportFmt));
    expContext().fmtMap["meta"] = ", \"cachePath\": \"" + (isTestMode() ? "--" : getCachePath("")) + "\"";
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (no_header)
        expContext().fmtMap["header"] = "";

    // collect together results for later display
    applyFilter();

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    optionOff(OPT_DENOM);
    registerOptions(nParams, params);

    items.clear();
    blocks.Init();
    type = CT_BLOCKS;
    RENAME_FIELD(CCacheEntry, "extra", "blockNumber");
    HIDE_FIELD(CCacheEntry, "type");
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    // will sort the fields in these classes if --parity is given
    sorts[0] = GETRUNTIME_CLASS(CBlock);
    sorts[1] = GETRUNTIME_CLASS(CTransaction);
    sorts[2] = GETRUNTIME_CLASS(CReceipt);

    Init();
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {
    if (which == "options") {
        return substitute(str, "block_list", "<block> [block...]");

    } else if (which == "notes") {
        string_q ret = str;
        if (verbose || COptions::isReadme) {
            ret += "You may customize the location of your cache in the file ~/.quickBlocks/quickBlocks.toml.\n";
        }
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
void COptions::applyFilter() {
    string_q list = getBlockNumList();
    CStringArray blockStrs;
    explode(blockStrs, list, '|');
    for (auto blockStr : blockStrs) {
        blknum_t bn = str_2_Uint(blockStr);
        CCacheEntry ce;
        ce.type = type;
        ce.extra = blockStr;
        ce.path = getBinaryCacheFilename(CT_BLOCKS, bn);
        ce.cached = fileExists(ce.path);
        if (!ce.cached)
            ce.path = (isNodeRunning() ? getVersionFromClient() : "not found");
        if (isTestMode())
            ce.path = substitute(substitute(ce.path, getVersionFromClient(), "--nodeVersion--"), getCachePath(""), "./");
        items[bn] = ce;
    }
}

//-----------------------------------------------------------------------
const char* STR_DISPLAY_WHERE =
"[{BLOCKNUMBER}]\t"
"[{PATH}]\t"
"[{CACHED}]";
