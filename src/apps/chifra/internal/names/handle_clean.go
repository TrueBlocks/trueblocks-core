// New tests -- chifra names --autoname (address in the environment)
// New tests -- chifra names --autoname <address>
package namesPkg

import (
	"strings"

	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/base"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/names"
	"github.com/TrueBlocks/trueblocks-core/src/apps/chifra/pkg/types"
)

func (opts *NamesOptions) HandleClean() error {
	allNames, err := names.LoadNamesMap(opts.Globals.Chain, names.Regular|names.Custom, []string{})
	if err != nil {
		return err
	}
	prefundMap, err := preparePrefunds(opts.Globals.Chain)
	if err != nil {
		return err
	}

	var regularNamesUpdated bool
	for _, name := range allNames {
		modified := cleanName(&name)
		if isPrefund := prefundMap[name.Address]; isPrefund != name.IsPrefund {
			name.IsPrefund = isPrefund
			modified = true
		}
		if modified && !regularNamesUpdated {
			regularNamesUpdated = !name.IsCustom
		}
	}

	return nil
}

func cleanName(name *types.SimpleName) (modified bool) {
	if name.Tags >= "8" {
		return false
	}

	lowerCaseSource := strings.ToLower(name.Source)
	if lowerCaseSource == "etherscan" {
		name.Source = "EtherScan.io"
		modified = true
	} else if lowerCaseSource == "trueblocks" {
		name.Source = "TrueBlocks.io"
		modified = true
	}

	if strings.Contains(lowerCaseSource, "  ") {
		name.Source = strings.ReplaceAll(name.Source, "  ", " ")
		modified = true
	}

	if len(name.Petname) == 0 {
		name.Petname = names.AddrToPetname(name.Address.Hex(), "-")
		modified = true
	}

	return
}

func preparePrefunds(chain string) (results map[base.Address]bool, err error) {
	prefunds, err := names.LoadPrefunds(
		chain,
		names.GetPrefundPath(chain),
	)
	if err != nil {
		return
	}

	results = make(map[base.Address]bool, len(prefunds))
	for _, prefund := range prefunds {
		results[prefund.Address] = true
	}
	return
}

func cleanContract(name *types.SimpleName) (modified bool) {
	if !name.IsContract {
		name.IsContract = true
		modified = true
	}
	// name, err := token.GetState(chain, name.Address, token.TokenStateName,
}

func cleanNonContract(name *types.SimpleName, wasContract bool) (modified bool) {
	if name.Tags == "30-Contracts:Humanity DAO" {
		name.Tags = "90-Individuals:Humanity DAO"
		modified = true
	}

	tagsEmpty := len(name.Tags) == 0
	tagContract := strings.Contains(name.Tags, "Contracts")
	tagToken := strings.Contains(name.Tags, "Tokens")

	if wasContract {
		name.IsContract = true
		name.Tags = "37-SelfDestructed"
		return true
	}

	if tagsEmpty || tagContract || tagToken {
		name.Tags = "90-Individuals:Other"
		modified = true
	}
	return
}

// Finish clean
//
// Prequisite:
//		if tag is >= 8 (as a string), return without modification noting that tags over '8' character are reserved
//		latestBlock = testMode ? 10800000 : getLatestBlock_client()
//
// Source:
//		if contains (ignore case) 'etherscan' then the entire string becomes Etherscan.io
//		if contains (ignore case) 'trueblocks' then the entire string becomes TrueBlocks.io
//		change any white space to spaces, change double spaces to single spaces
//
// Petname:
//		based on address assign it without asking (never user assigned)
//
// IsPrefund:
//		is the address a prefund for this chain?
//
// IsContract:
//		'wasContract' (is there a current record, and does that current record have isContract set?)
//		'isContract' (is the address a contract at the current block?)
//		'isAirdrop' (does the account's name contain the word "airdrop"

// static const string_q erc721QueryBytes = "0x" + padRight(substitute(_INTERFACE_ID_ERC721, "0x", ""), 64, '0');
// inline bool isErc721(const address_t& addr, const CAbi& abi_spec, blknum_t latest) {
//     string_q val = getTokenState(addr, "supportsInterface", abi_spec, latest, erc721QueryBytes);
//     return val == "T" || val == "true";
// }

//     bool isAirdrop = containsI(account.name, "airdrop");
//     if (account.tags == "60-Airdrops")
//         account.tags = "";

//     if (!isContract) {
//         bool isEmpty = account.tags.empty();
//         bool isContract = contains(account.tags, "Contracts");
//         bool isToken = contains(account.tags, "Tokens");
//         account.tags = !isEmpty && !isContract && !isToken ? account.tags : "90-Individuals:Other";
//         if (wasContract) {
//             // This used to be a contract and now is not, so it must be a self destruct
//             account.isContract = true;
//             account.tags = "37-SelfDestructed";
//         }

//     } else {
//         // This is a contract...
//         account.isContract = true;

//         string_q name = getTokenState(account.address, "name", opts->abi_spec, latestBlock);
//         string_q symbol = getTokenState(account.address, "symbol", opts->abi_spec, latestBlock);
//         uint64_t decimals = str_2_Uint(getTokenState(account.address, "decimals", opts->abi_spec, latestBlock));
//         if (!name.empty() || !symbol.empty() || decimals > 0) {
//             account.isErc20 = true;
//             account.source =
//                 (account.source.empty() || account.source == "TrueBlocks.io" || account.source == "EtherScan.io")
//                     ? "On chain"
//                     : account.source;
//             // Use the values from on-chain if we can...
//             account.name = (!name.empty() ? name : account.name);
//             account.symbol = (!symbol.empty() ? symbol : account.symbol);
//             account.decimals = decimals ? decimals : (account.decimals ? account.decimals : 18);
//             account.isErc721 = isErc721(account.address, opts->abi_spec, latestBlock);
//             if (account.isErc721) {
//                 account.tags = "50-Tokens:ERC721";

//             } else {
//                 // This is an ERC20, so if we've not tagged it specifically, make it thus
//                 if (account.tags.empty() || containsI(account.tags, "token") ||
//                     containsI(account.tags, "30-contracts") || containsI(account.tags, "55-defi") || isAirdrop) {
//                     account.tags = "50-Tokens:ERC20";
//                 }
//             }

//         } else {
//             account.isErc20 = false;
//             account.isErc721 = false;
//         }
//         if (account.tags.empty())
//             account.tags = "30-Contracts";
//     }

//     if (isAirdrop && !containsI(account.name, "Airdrop")) {
//         replaceAll(account.name, " airdrop", "");
//         replaceAll(account.name, " Airdrop", "");
//         account.name = account.name + " Airdrop";
//     }

//     // Clean up name and symbol
//     account.name = trim(substitute(account.name, "  ", " "));
//     account.symbol = trim(substitute(account.symbol, "  ", " "));

//     return !account.name.empty();
// }

// 1) There are five files:
//		binary database
//		names.tab in $configPath
//		names_custom.tab in $configPath
//		the original source for names.tab (../src/other/install/names/)
//		the original source for custom_names.tab (in my case ~/Desktop, but generally ../src/other/install/names/)
//
// 2) Clean changes the binary database and updates the files in $configPath
// 3) If [settings]source=<path> and/or [settings]custom=<path> is set in $configPath/ethNames.toml, then
//      re-write these files too.
// 4) We can probably use Update quite easily
// 5) Clean - high level:
// 		Config paths should handle ~ and $HOME
// 		If the source doesn't exist (misconfig?), do nothing
// 		We edit the source and copy it to the destination, then the next time it runs it updates the binary
// 		Message the user that we are updateing source into dest
// 		Read entire source file
// 			for each name
// 				cleanName (use the same function we would use regularly)
// 				report progress on each clean of each name
// 		sort resulting array by address
// 		remove dups if there are any
// 		output the header
// 		output the array of data
//
// Open issues:
//
// 1) Should names span chains?
// 2) How does ENS interplay?
// 3) Can we extend to query ENS for registered names for the given address?
//

// establishFolder(cacheFolder_names);
// if ((contains(item.tags, "Kickback") || contains(item.tags, "Humanity")))  // don't expose people during testing
// Last name in wins in case the name is customized and overlays an existing name -- update if
//      - the new name is not empty
//      - the new name is different or if the new name is a Prefund
//      - only update the name
