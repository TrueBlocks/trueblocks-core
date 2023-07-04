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
    COption("", "", "", OPT_DESCRIPTION, "Export full detail of transactions for one or more addresses."),
    // clang-format on
    // END_CODE_OPTIONS
};
static const size_t nParams = sizeof(params) / sizeof(COption);

//---------------------------------------------------------------------------------------------------
bool COptions::parseArguments(string_q& command) {
    if (!standardOptions(command))
        return false;

    // BEG_CODE_LOCAL_INIT
    // END_CODE_LOCAL_INIT

    Init();
    explode(arguments, command, ' ');

    for (auto arg : arguments) {
        if (false) {
            // do nothing -- make auto code generation easier
            // BEG_CODE_AUTO
        } else if (startsWith(arg, '-')) {  // do not collapse

            if (!builtInCmd(arg)) {
                return invalid_option(arg);
            }

            // END_CODE_AUTO
        }
    }

    if (!loadNames()) {
        return usage("Could not load names database.");
    }

    // TODO: This can be removed
    CMonitor m;
    cleanFolder(m.getPathToMonitor("", true));

    if (!loadMonitors())
        return false;

    return true;
}

//---------------------------------------------------------------------------------------------------
void COptions::Init(void) {
    // BEG_CODE_GLOBALOPTS
    registerOptions(nParams, params, OPT_PREFUND);
    // END_CODE_GLOBALOPTS

    // BEG_CODE_INIT
    // END_CODE_INIT

    // clang-format off
    skip_ddos = getGlobalConfig("acctExport")->getConfigBool("settings", "skip_ddos", true);
    max_traces = getGlobalConfig("acctExport")->getConfigInt("settings", "max_traces", 250);
    // clang-format on

    // if (!cache && getGlobalConfig("acctExport")->getConfigBool("settings", "cache_txs", false))
    //     cache = true;  // backwards compat

    meta = getMetaData();

    allMonitors.clear();

    ledgerManager.appArray.clear();
    ledgerManager.accountedFor = "";
    ledgerManager.name.address = "";
    ledgerManager.name.petname = "";

    // We don't clear these because they are part of meta data
    // prefundAddrMap.clear();
    // blkRewardMap.clear();
    // toNameExistsMap.clear();
    // fromNameExistsMap.clear();
    // abiMap.clear();

    minArgs = 0;
    fileRange = make_pair(NOPOS, NOPOS);
    allMonitors.clear();

    // Establish folders. This may be redundant, but it's cheap.
    establishCacheFolders();
    establishIndexFolders();
}

//---------------------------------------------------------------------------------------------------
COptions::COptions(void) {
    Init();

    // BEG_CODE_NOTES
    // clang-format off
    notes.push_back("An `address` must be either an ENS name or start with '0x' and be forty-two characters long.");
    notes.push_back("Articulating the export means turn the EVM's byte data into human-readable text (if possible).");
    notes.push_back("For the --logs option, you may optionally specify one or more --emitter, one or more --topics, or both.");  // NOLINT
    notes.push_back("The --logs option is significantly faster if you provide an --emitter or a --topic.");
    notes.push_back("Neighbors include every address that appears in any transaction in which the export address also appears.");  // NOLINT
    notes.push_back("If provided, --max_records dominates, also, if provided, --first_record overrides --first_block.");
    notes.push_back("The --first_record and --max_record options are zero-based (as are the block options).");
    notes.push_back("The _block and _record options are ignored when used with the --count option.");
    // clang-format on
    // END_CODE_NOTES

    // clang-format off
    configs.push_back("`cache`: write transactions to the cache (see notes).");
    configs.push_back("`cache_traces`: write traces to the cache (see notes).");
    configs.push_back("`skip_ddos`: toggle skipping over 2016 dDos transactions ('on' by default).");
    configs.push_back("`max_traces`: if --skip_ddos is on, this many traces defines what a ddos transaction | is (default = 250).");  // NOLINT
    // clang-format on

    // BEG_ERROR_STRINGS
    // END_ERROR_STRINGS
}

//--------------------------------------------------------------------------------
COptions::~COptions(void) {
}
