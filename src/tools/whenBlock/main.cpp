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
#include "options.h"

//-----------------------------------------------------------------------
int main(int argc, const char* argv[]) {
    etherlib_init(quickQuitHandler);

    COptions options;
    if (!options.prepareArguments(argc, argv))
        return 0;

    if (!options.parseArguments(options.commandLines[0]))
        return 0;

    if (options.firstOut)
        cout << exportPreamble(expContext().fmtMap["header"], GETRUNTIME_CLASS(CBlock));

    ASSERT(options.timestamps);
    size_t n = nTimestamps();
    for (size_t index = 0; index < n; index++) {
        CBlock block;
        block.blockNumber = getTimestampBlockAt(index);
        block.timestamp = getTimestampAt(index);
        if (!visitBlock(block, &options))
            break;
    }

    cout << exportPostamble(options.errors, expContext().fmtMap["meta"]);

    etherlib_cleanup();
    return 0;
}

//-----------------------------------------------------------------------
bool visitBlock(CBlock& block, void* data) {
    COptions* opt = reinterpret_cast<COptions*>(data);
    if (opt->checker.expected == 0) {
        opt->checker.expected++;
        return true;
    }

    CTimeStamper* c = &opt->checker;
    LOG_INFO("Checking block (", block.blockNumber, ", ", block.timestamp, ")", "\r");

    string_q reason;
    if (block.blockNumber != c->expected) {
        reason = "num != expected";
    }

    if (block.blockNumber != c->prevBn + 1) {
        reason = "num != prev+1";
    }

    if (block.timestamp <= c->prevTs) {
        CBlock fix;
        getBlockHeader(fix, c->prevBn);
        reason = "ts > c->prevTs";
    }

    if (!reason.empty()) {
        ostringstream os;
        CBlock blk;
        getBlockHeader(blk, c->expected);
        LOG_WARN(reason, " at block ", c->expected, " ==> ", " sb:", block.blockNumber, ".", block.timestamp,
                 " is: ", blk.blockNumber, ".", blk.timestamp, " c: ", c->expected, ".", c->prevBn, ".", c->prevTs);
    }

    c->expected++;
    c->prevBn = block.blockNumber;
    c->prevTs = block.timestamp;

    return true;
}

//---------------------------------------------------------------------------------------------------
static const COption params[] = {
    // BEG_CODE_OPTIONS
    // clang-format off
    COption("check", "c", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, checks the validity of the timestamp data"),  // NOLINT
    COption("fix", "f", "", OPT_HIDDEN | OPT_SWITCH, "available only with --timestamps, fixes incorrect timestamps if any"),  // NOLINT
    COption("", "", "", OPT_DESCRIPTION, "Find block(s) based on date, blockNum, timestamp, or 'special'."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& commandIn) {
    string_q command = commandIn;
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    commandIn = substitute(commandIn, "--timestamps", "");

    getBlockHeader(latest, "latest");
    Init();
    explode(arguments, command, ' ');
    for (auto arg : arguments) {
        string_q orig = arg;

        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (arg == "-c" || arg == "--check") {
            check = true;

        } else if (arg == "-f" || arg == "--fix") {
            fix = true;

        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, NOOPT, OPT_DENOM | OPT_CHECKLATEST);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    check = false;
    fix = false;
    // END_CODE_INIT

    blocks.Init();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("The block list may contain any combination of `number`, `hash`, `date`, special `named` blocks.");
    notes.push_back("Dates must be formatted in JSON format: YYYY-MM-DD[THH[:MM[:SS]]].");
    // clang-format on
    // END_CODE_NOTES

    // BEG_ERROR_STRINGS
    usageErrs[ERR_OPENINGTIMESTAMPS] = "Could not open timestamp file.";
    usageErrs[ERR_ONLYTS] = "The --check, --fix, and --count options are only available with the --timestamps option.";
    // END_ERROR_STRINGS

    // Differnt default for this software, but only change it if user hasn't already therefor not in Init
    if (!isApiMode())
        expContext().exportFmt = TXT1;
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
