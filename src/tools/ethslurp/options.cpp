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
    COption("~addr_list",       "one or more addresses to slurp"),
    COption("-blocks:<range>",  "an optional range of blocks to slurp"),
    COption("-type:<tx_type>",  "extract either [ ext | int | token | miner | all ] type of transactions"),
    COption("-fmt:<str>",       "pretty print, optionally add ':txt,' ':csv,' or ':html'"),
    COption("-blocks:<range>",  "export records in block range (:0[:max])"),
    COption("-silent",          "Run silently (only freshen the data, do not display it)"),
    COption("",                 "Fetches data from EtherScan for an arbitrary address. Formats the output to your specification.\n"),
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {

    if (!standardOptions(command))
        return false;

    Init();
    blknum_t latestBlock = getLastBlock_client();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (startsWith(arg, "-t:") || startsWith(arg, "--type:")) {
            type = substitute(substitute(arg, "-t:", ""), "--type:", "");
            if (type == "all")
                return usage("Type 'all' is currently disabled. Quitting...");

            if (type != "int" && type != "ext" && type != "token" && type != "miner")
                return usage("Type must be either 'int', 'ext', 'token', 'miner' or 'all'. Quitting...");

        } else if (arg == "-f") {
            exportFormat = "json";

        } else if (startsWith(arg, "-f:") || startsWith(arg, "--fmt:")) {
            exportFormat = substitute(substitute(arg, "-f:", ""), "--fmt:", "");
            if (exportFormat.empty())
                return usage("Please provide a formatting option with " + orig + ". Quitting...");

        } else if (startsWith(arg, "-b:") || startsWith(arg, "--blocks:")) {
            arg = substitute(substitute(arg, "-b:", ""), "--blocks:", "");
            string_q ret = blocks.parseBlockList(arg, latestBlock);
            if (endsWith(ret, "\n")) {
                cerr << "\n  " << ret << "\n";
                return false;
            } else if (!ret.empty()) {
                return usage(ret);
            }

        } else if (arg == "-s" || arg == "--silent") {
            silent = true;

        } else if (startsWith(arg, '-')) {  // do not collapse
            if (!builtInCmd(arg)) {
                return usage("Invalid option: " + arg);
            }

        } else {

            if (!isAddress(arg))
                 return usage("Please provide a valid Ethereum address. Quitting...");
            addrs.push_back(str_2_Addr(toLower(arg)));
        }
    }

    // Note this may not return if user chooses to quit
    api.checkKey();

    if (addrs.empty())
        return usage("You must supply an Ethereum account or contract address. ");

    if (!establishFolder(getCachePath("slurps/")))
        return usage("Unable to create data folders at " + getCachePath("slurps/"));

    // Dumps an error message if the fmt_X_file format string is not found.
    getFormatString("file", false);

    // Load per address configurations if any
    string_q customConfig = getCachePath("slurps/" + addrs[0] + ".toml");
    if (fileExists(customConfig)) {
        CToml perAddr("");
        perAddr.setFilename(customConfig);
        if (fileExists(customConfig)) {
            perAddr.readFile(customConfig);
            ((CToml*)getGlobalConfig("ethslurp"))->mergeFile(&perAddr);  // NOLINT
        }
    }

    if (blocks.start == 0 && blocks.stop == 0)
        blocks.stop = INT_MAX;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    registerOptions(nParams, params);

    type = "";
    blocks.Init();
    exportFormat = "json";
    addrs.clear();
    silent = false;
    fromFile = false;
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

//    CAccount::registerClass();
    UNHIDE_FIELD(CTransaction, "isError");
    UNHIDE_FIELD(CTransaction, "isInternal");
    UNHIDE_FIELD(CTransaction, "date");
    UNHIDE_FIELD(CTransaction, "ether");

    HIDE_FIELD  (CTransaction, "toContract");
    HIDE_FIELD  (CTransaction, "receipt");
    HIDE_FIELD  (CTransaction, "traces");
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}

//--------------------------------------------------------------------------------
string_q COptions::postProcess(const string_q& which, const string_q& str) const {

    if (which == "options") {
        return str;

    } else if (which == "notes" && (verbose || COptions::isReadme)) {

        string_q ret;
        ret += "Portions of this software are Powered by Etherscan.io APIs.\n";
        return ret;
    }
    return str;
}

//--------------------------------------------------------------------------------
string_q COptions::getFormatString(const string_q& which, bool ignoreBlank) {

    if (which == "file")
        buildDisplayStrings();

    string_q errMsg;

    string_q formatName = "fmt_" + exportFormat + "_" + which;
    string_q ret = getGlobalConfig("ethslurp")->getConfigStr("display", formatName, "");
    if (contains(ret, "file:")) {
        string_q file = substitute(ret, "file:", "");
        if (!fileExists(file))
            errMsg = string_q("Formatting file '") + file +
            "' for display string '" + formatName + "' not found. Quiting...\n";
        else
            asciiFileToString(file, ret);

    } else if (contains(ret, "fmt_")) {  // it's referring to another format string...
        string_q newName = ret;
        ret = getGlobalConfig("ethslurp")->getConfigStr("display", newName, "");
        formatName += ":" + newName;
    }
    ret = substitute(substitute(ret, "\\n", "\n"), "\\t", "\t");

    // some sanity checks
    if (countOf(ret, '{') != countOf(ret, '}') || countOf(ret, '[') != countOf(ret, ']')) {
        errMsg = string_q("Mismatched brackets in display string '") + formatName + "': '" + ret + "'. Quiting...\n";

    } else if (ret.empty() && !ignoreBlank) {
        const char *ERR_NO_DISPLAY_STR =
        "You entered an empty display string with the --format (-f) option. The format string 'fmt_[{FMT}]_file'\n"
        "  was not found in the configuration file (which is stored here: ~/.quickBlocks/quickBlocks.toml).\n"
        "  Please see the full documentation for more information on display strings.";
        errMsg = usageStr(substitute(string_q(ERR_NO_DISPLAY_STR), "[{FMT}]", exportFormat));
    }

    if (!errMsg.empty()) {
        cerr << errMsg;
        exit(0);
    }

    return ret;
}

//---------------------------------------------------------------------------------------------------
bool buildFieldList(const CFieldData& fld, void *data) {
    string_q *s = reinterpret_cast<string_q *>(data);
    *s += (fld.getName() + "|");
    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::buildDisplayStrings(void) {
    // Set the default if it's not set
    if (exportFormat.empty())
        exportFormat = "json";

    // This is what we're really after...
    const string_q fmtForRecords = getFormatString("record", false);
    ASSERT(!fmtForRecords.empty());

    // ...we may need this to build it.
    const string_q fmtForFields  = getFormatString("field", !contains(fmtForRecords, "{FIELDS}"));
    ASSERT(!fmtForFields.empty());

    string_q defList = getGlobalConfig("ethslurp")->getConfigStr("display", "fmt_fieldList", "");
    string_q fieldList = getGlobalConfig("ethslurp")->getConfigStr("display",
                                                                   "fmt_" + exportFormat + "_fieldList", defList);
    if (fieldList.empty())
        GETRUNTIME_CLASS(CTransaction)->forEveryField(buildFieldList, &fieldList);

    string_q origList = fieldList;

    displayString = "";
    header = "";
    while (!fieldList.empty()) {
        string_q fieldName = nextTokenClear(fieldList, '|');
        bool force = contains(fieldName, "*");
        replace(fieldName, "*", "");

        const CFieldData *field = GETRUNTIME_CLASS(CTransaction)->findField(fieldName);
        if (!field) {
            cerr << "Field '" << fieldName << "' not found in fieldList '" << origList << "'. Quitting...\n";
            exit(0);
        }
        if (field->isHidden() && force)
            ((CFieldData*)field)->setHidden(false);  // NOLINT
        if (!field->isHidden()) {
            string_q resolved = fieldName;
            if (exportFormat != "json")
                resolved = getGlobalConfig("ethslurp")->getConfigStr("field_str", fieldName, fieldName);
            displayString +=
            substitute(
                       substitute(fmtForFields, "{FIELD}", "{" + toUpper(resolved)+"}"), "{p:FIELD}", "{p:"+resolved+"}");
            header +=
            substitute(
                       substitute(
                                  substitute(
                                             substitute(fmtForFields, "{FIELD}", resolved), "[", ""), "]", ""), "<td ", "<th ");
        }
    }
    displayString = trimWhitespace(displayString);
    header        = trimWhitespace(header);

    displayString = trim(substitute(fmtForRecords, "[{FIELDS}]", displayString), '\t');
    if (exportFormat == "json") {
        // One little hack to make raw json more readable
        replaceReverse(displayString, "}]\",", "}]\"\n");
    }
}
