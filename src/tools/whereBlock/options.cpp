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
// BEG_CODE_OPTIONS
static const COption params[] = {
//#define NEW_CODE
#ifdef NEW_CODE
ssssCOption2("block_list",	null,	"flag",	true,	true,	"a space-separated list of one or more blocks to search for"),
ssssCOption2("fmt",	null,	"<fmt>",	false,	false,	"export format (one of [none|json|txt|csv|api])"),
ssssCOption2("",	null,	"",	false,	true,	"Reports if a block was found in the cache, at a local, or at a remote node."),

#else // NEW_CODE
    COption("~block_list", "a space-separated list of one or more blocks to search for"),
    COption("@fmt:<fmt>",  "export format (one of [none|json|txt|csv|api])"),
    COption("",            "Reports if a block was found in the cache, at a local, or at a remote node.\n"),
#endif
};
static const size_t nParams = sizeof(params) / sizeof(COption);
// END_CODE_OPTIONS

extern const char* STR_DISPLAY;
//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    bool noHeader = false;
    string_q format = getGlobalConfig()->getConfigStr("display", "format", STR_DISPLAY);
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
            if (endsWith(ret, "\n")) {
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
        case NONE1: format = STR_DISPLAY; break;
        case API1:
        case JSON1: format = ""; break;
        case TXT1:
        case CSV1:
            format = getGlobalConfig()->getConfigStr("display", "format", format.empty() ? STR_DISPLAY : format);
            break;
    }
    manageFields("CCacheEntry:" + cleanFmt((format.empty() ? STR_DISPLAY : format), exportFmt));
    expContext().fmtMap["meta"] = ", \"cachePath\": \"" + (isTestMode() ? "--" : getCachePath("")) + "\"";
    expContext().fmtMap["format"] = expContext().fmtMap["header"] = cleanFmt(format, exportFmt);
    if (noHeader)
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
const char* STR_DISPLAY = "[{BLOCKNUMBER}]\t[{PATH}]\t[{CACHED}]";
