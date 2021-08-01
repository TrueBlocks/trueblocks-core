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

//--------------------------------------------------------------------
void COptions::finishClean(CAccountName& account) {
    LOG_INFO("Cleaning ", account.address, "                                  \r");
    account.is_prefund = expContext().prefundMap[account.address] > 0;
    account.is_contract = isContractAt(account.address, latestBlock);
    account.is_erc20 = isTokenContract(account);
    if (account.is_contract && account.is_erc20) {
        account.decimals = standards.decimals ? standards.decimals : 18;
        account.symbol = standards.symbol;
        bool isEtherscan = contains(toLower(account.source), "etherscan");
        bool isTrueblocks = contains(toLower(account.source), "trueblocks");
        if (account.source.empty() || isEtherscan || isTrueblocks) {
            // if the data is from us or etherscan or doesn't exist, fix it if we can
            bool isAirdrop = contains(toLower(account.name), "airdrop") || contains(toLower(account.tags), "airdrop");
            account.source = "On chain";
            account.name =
                standards.name + (isAirdrop && !contains(toLower(standards.name), "airdrop") ? " Airdrop" : "");
        }
    }
    if (account.decimals > 0 || !account.symbol.empty()) {
        account.is_erc721 = contains(toLower(account.tags), "erc721");
        account.is_erc20 = true;
        account.tags = (account.tags.empty() || contains(toLower(account.tags), "token") ||
                        contains(account.tags, "30-Contracts") || contains(account.tags, "55-Defi"))
                           ? (account.is_erc721 ? "51-Tokens:ERC721" : "50-Tokens:ERC20")
                           : account.tags;
    }
    if (contains(toLower(account.source), "etherscan"))
        account.source = "EtherScan.io";
    if (contains(account.source, "trueblocks"))
        account.source = "TrueBlocks";
    account.name = trim(substitute(account.name, "  ", " "));
    account.description = trim(substitute(account.description, "  ", " "));
    if (account.description == "false")
        account.description = "";
}

//--------------------------------------------------------------------
bool COptions::isTokenContract(const CAccountName& account) {
    if (!account.is_contract)  // maliciousMap[account.address] || !account.is_contract)
        return false;
    standards = CMonitor();  // reset
    standards.address = account.address;

    string_q name = getTokenState(standards.address, "name", abi_spec, latestBlock);
    string_q symbol = getTokenState(standards.address, "symbol", abi_spec, latestBlock);
    uint64_t decimals = str_2_Uint(getTokenState(standards.address, "decimals", abi_spec, latestBlock));

    standards.name = (name.empty() ? account.name : name);
    standards.symbol = (symbol.empty() ? account.symbol : symbol);
    standards.decimals = decimals;

    return !name.empty() || decimals;
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
        bool isContract = name.is_contract;
        finishClean(name);
        name.is_contract = isContract;
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
        EXIT_USAGE("This installation of TrueBlocks may not clean the names file. Primary names file was not set.");
    LOG_WARN("The primary names file was cleaned.", string_q(50, ' '));

    //    string_q customSource = getGlobalConfig("ethNames")->getConfigStr("settings", "custom", "<UNSET>");
    //    string_q customDest = configPath("names/names_custom.tab");
    //    if (!cleanNames(customSource, customDest))
    //        EXIT_USAGE("This installation of TrueBlocks may not clean the names file. Customized names file was not
    //        set.");
    LOG_WARN("The custom names file was NOT cleaned.", string_q(50, ' '));

    ::remove(getCachePath("names/names.bin").c_str());
    CAccountName acct;
    getNamedAccount(acct, "0x0");  // reloads

    return false;  // don't proceed
}
