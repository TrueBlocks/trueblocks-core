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

//--------------------------------------------------------------------
void COptions::finishClean(CAccountName& name) {
    LOG_INFO("Cleaning ", name.address, "                                  \r");
    name.is_prefund = prefundWeiMap[name.address] > 0;
    if (!maliciousMap[name.address] && isContractAt(name.address, latestBlock)) {
        standards.address = name.address;
        string_q nm = getTokenState("name", standards, latestBlock);
        string_q decimals = getTokenState("decimals", standards, latestBlock);
        string_q symbol = getTokenState("symbol", standards, latestBlock);
        if (!nm.empty() || !decimals.empty()) {
            name.decimals = str_2_Uint(decimals);
            name.symbol = symbol.empty() ? name.symbol : symbol;
            name.is_erc20 = true;
            name.tags = "50-ERC20";
            if (!contains(name.source, "kickback")) {
                name.source = "On chain";
                name.name = nm.empty() ? name.name : (contains(name.name, "Airdrop") ? nm + " Airdrop" : nm);
            }
            // name.is_erc721 = isERC721(name.address);
        }
        name.is_contract = true;
    } else {
        name.is_contract = false;
        name.is_erc20 = false;
        name.is_erc721 = false;
    }
    if (contains(name.source, "EtherScan"))
        name.source = "EtherScan.io";
    name.name = trim(substitute(name.name, "  ", " "));
}

//--------------------------------------------------------------------
bool COptions::cleanNames(const string_q& sourceIn, const string_q& destIn) {
    string_q source = sourceIn;
    string_q dest = destIn;

    replace(source, "~", getHomeFolder());
    replace(source, "$HOME", getHomeFolder());
    replaceAll(source, "//", "/");
    if (!fileExists(source))
        return false;

    cerr << "Processing names file (" << source << ") into " << dest << endl;

    string_q contents = asciiFileToString(source);
    nextTokenClear(contents, '\n');  // skip header

    CStringArray fields;
    string_q fieldStr = toLower(substitute(substitute(STR_DISPLAY_ACCOUNTNAME, "[{", ""), "}]", ""));
    explode(fields, fieldStr, '\t');

    CAccountName name;
    CAccountNameArray names;
    while (name.parseText(fields, contents)) {
        finishClean(name);
        names.push_back(name);
    }
    sort(names.begin(), names.end());
    CAccountNameArray deduped;
    address_t prev;
    for (auto item : names) {
        if (item.address != prev)
            deduped.push_back(item);
        prev = item.address;
    }

    ostringstream os;
    os << fieldStr << endl;
    for (auto n : deduped)
        os << n.Format(STR_DISPLAY_ACCOUNTNAME) << endl;
    stringToAsciiFile(dest, os.str());

    return true;
}

//--------------------------------------------------------------------
bool COptions::handle_clean(void) {
    ENTER("handle_clean");

    string_q mainSource = getGlobalConfig("ethNames")->getConfigStr("settings", "source", "<UNSET>");
    string_q mainDest = configPath("names/names.tab");
    if (!cleanNames(mainSource, mainDest))
        EXIT_USAGE("Primary names file (" + mainSource + ") not found. Quitting...");

    string_q customSource = getGlobalConfig("ethNames")->getConfigStr("settings", "custom", "<UNSET>");
    string_q customDest = configPath("names/names_custom.tab");
    if (!cleanNames(customSource, customDest))
        EXIT_USAGE("Custom names file (" + customSource + ") not found. Quitting...");

    ::remove(getCachePath("names/names.bin").c_str());
    CAccountName acct;
    getNamedAccount(acct, "0x0");  // reloads

    return false;  // don't proceed
}
