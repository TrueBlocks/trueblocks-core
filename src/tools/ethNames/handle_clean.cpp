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

static const string_q erc721QueryBytes = "0x" + padRight(substitute(_INTERFACE_ID_ERC721, "0x", ""), 64, '0');
inline bool isErc721(const address_t& addr, const CAbi& abi_spec, blknum_t latest) {
    string_q val = getTokenState(addr, "supportsInterface", abi_spec, latest, erc721QueryBytes);
    return val == "T" || val == "true";
}

//--------------------------------------------------------------------
bool startsWithNumber(const string_q& str) {
    return str_2_Uint(str) > 0 || startsWith(str, "0");
}

//--------------------------------------------------------------------
bool COptions::finishClean(CAccountName& account) {
    if (startsWithNumber(account.tags) &&
        account.tags > "799999")  // tags named higher than or equal to 80 are hand edited
        return true;

    // Clean up source
    if (containsI(account.source, "etherscan"))
        account.source = "EtherScan.io";
    if (containsI(account.source, "trueblocks"))
        account.source = "TrueBlocks.io";
    account.source = trim(substitute(account.source, "  ", " "));

    // Clean up and time the description
    if (account.description == "false")
        account.description = "";
    account.description = trim(substitute(account.description.substr(0, 255), "  ", " "));

    // Are we a pre-fund?
    account.isPrefund = prefundAt(account.address) > 0;

    bool wasContract = account.isContract;
    bool isContract = isContractAt(account.address, latestBlock);
    bool isAirdrop = containsI(account.name, "airdrop");
    if (account.tags == "60-Airdrops")
        account.tags = "";

    if (!isContract) {
        // If the tag is not empty and not 30-Contracts, perserve it. Otherwise it's an individual
        account.tags = !account.tags.empty() && account.tags != "30-Contracts" ? account.tags : "90-Individuals:Other";
        if (wasContract) {
            // This used to be a contract and now is not, so it must be a self destruct
            account.isContract = true;
            account.tags = "37-SelfDestructed";
        }

    } else {
        // This is a contract...
        account.isContract = true;

        // ...let's see if it's an ERC20...
        string_q name = getTokenState(account.address, "name", abi_spec, latestBlock);
        if (countOf(name, '-') > 3) {
            // some sort of hacky renaming for Kickback
            name = account.name;
        }
        string_q symbol = getTokenState(account.address, "symbol", abi_spec, latestBlock);
        uint64_t decimals = str_2_Uint(getTokenState(account.address, "decimals", abi_spec, latestBlock));

        if (!name.empty() || !symbol.empty() || decimals > 0) {
            account.isErc20 = true;
            account.source =
                (account.source.empty() || account.source == "TrueBlocks.io" || account.source == "EtherScan.io")
                    ? "On chain"
                    : account.source;

            // Use the values from on-chain if we can...
            account.name = (!name.empty() ? name : account.name);
            account.symbol = (!symbol.empty() ? symbol : account.symbol);
            account.decimals = decimals ? decimals : (account.decimals ? account.decimals : 18);
            account.isErc721 = isErc721(account.address, abi_spec, latestBlock);
            if (account.isErc721) {
                account.tags = "50-Tokens:ERC721";

            } else {
                // This is an ERC20, so if we've not tagged it specifically, make it thus
                if (account.tags.empty() || containsI(account.tags, "token") ||
                    containsI(account.tags, "30-contracts") || containsI(account.tags, "55-defi") || isAirdrop) {
                    account.tags = "50-Tokens:ERC20";
                }
            }

        } else {
            account.isErc20 = false;
            account.isErc721 = false;
        }
        if (account.tags.empty())
            account.tags = "30-Contracts";
    }

    if (isAirdrop && !containsI(account.name, "Airdrop")) {
        replaceAll(account.name, " airdrop", "");
        replaceAll(account.name, " Airdrop", "");
        account.name = account.name + " Airdrop";
    }

    // Clean up name and symbol
    account.name = trim(substitute(account.name, "  ", " "));
    account.symbol = trim(substitute(account.symbol, "  ", " "));

    return !account.name.empty();
}

//--------------------------------------------------------------------
bool COptions::cleanNames(const string_q& sourceIn, const string_q& destIn) {
    string_q source = sourceIn;
    replace(source, "~", getHomeFolder());
    replace(source, "$HOME", getHomeFolder());
    replaceAll(source, "//", "/");
    if (!fileExists(source))
        return false;

    string_q dest = destIn;

    cerr << "Processing names file (" << source << ") into " << dest << endl;

    string_q contents = asciiFileToString(source);
    size_t nRecords = countOf(contents, '\n');

    CStringArray fields;
    string_q fieldStr = toLower(nextTokenClear(contents, '\n'));
    explode(fields, fieldStr, '\t');

    CAccountName name;
    CAccountNameArray names;
    size_t cnt = 0;
    while (name.parseText(fields, contents)) {
        LOG_INFO("Cleaning ", ++cnt, " of ", nRecords, ": ", name.address, "                                  \r");
        if (!finishClean(name))
            continue;
        names.push_back(name);
    }

    sort(names.begin(), names.end());
    CAccountNameArray deduped;

    address_t prev;
    for (auto item : names) {
        if (item.address != prev && item.decimals <= 18)  // some bogus data here not sure why
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
    string_q mainSource = getGlobalConfig("ethNames")->getConfigStr("settings", "source", "<not_set>");
    string_q mainDest = getPathToChainConfig_newOff("names/names.tab");
    if (!cleanNames(mainSource, mainDest))
        return usage("This installation of TrueBlocks may not clean the names file.");
    LOG_WARN("The primary names file was cleaned.", string_q(50, ' '));

    //    string_q customSource = getGlobalConfig("ethNames")->getConfigStr("settings", "custom", "<not_set>");
    //    string_q customDest = getPathToChainConfig_newOff("names/names_custom.tab");
    //    if (!cleanNames(customSource, customDest))
    //        return usage("This installation of TrueBlocks may not clean the names file. Customized names file was not
    //        set.");
    LOG_WARN("The custom names file was NOT cleaned.", string_q(50, ' '));

    // Bin files will get rebuilt if the ascii file changed

    return false;  // don't proceed
}
